//#include "vld.h"
#include "mainwindow.h"
#include "dbutils.h"
#include "emdilib.h"
#include "documents.h"
#include "menudocvisitor.h"
#include "numberemitter.h"
#include "filesavevisitor.h"
#include "filesavecopyasvisitor.h"
#include "extensiondocvisitor.h"

#include <QAction>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QFuture>
#include <QFutureWatcher>
#include <QDockWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QThreadPool>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <sstream>

// TODO: multi-thread load and save
// TODO: QML
// TODO: TCL, ECL, Lua
// TODO: menus



using docVec_t = std::list<std::unique_ptr<IDocument> >;
using dispatchMap_t = std::map<QString, std::function<void(const QVariant &)>>;

void quitSequence(Emdi &);
QWidget *buttonWindow(Emdi &, docVec_t &);
dispatchMap_t dispatchMap(Emdi &, docVec_t &);
auto makeDispatch(const dispatchMap_t &);
auto mainCtor(const dispatchMap_t &);
void updateMenus(const Emdi &, const QMdiSubWindow *);
void setActionChecked(const QWidget *, const std::string & act, bool checked);
void fileSave(const Emdi &, const QMainWindow *);
auto fileSaveParams(const Emdi &, const QMainWindow *);
void fileSaveAs(const Emdi &, const QMainWindow *);
void fileSaveCopyAs(const Emdi &, const QMainWindow *);

template <typename T> std::string docString() {return "undefined";}
template <> std::string docString<SymbolLibDocument>() {return "SymLibDocument_";}
template <> std::string docString<FootprintLibDocument>() {return "FootprintLibDocument_";}
template <> std::string docString<SchDocument>() {return "SchDocument_";}
template <> std::string docString<PCBDocument>() {return "PCBDocument_";}

template <typename T> std::string docExt() {return "undefined";}
template <> std::string docExt<SymbolLibDocument>() {return ".SymLib";}
template <> std::string docExt<FootprintLibDocument>() {return ".FootLib";}
template <> std::string docExt<SchDocument>() {return ".SchDoc";}
template <> std::string docExt<PCBDocument>() {return ".PcbDoc";}

template<typename T> std::string docName() {
    static int idx = 0;
    std::stringstream ss;
    ss << docString<T>() << std::setfill('0') <<std::setw(2) << idx++ << docExt<T>();
    return ss.str();
}

template<typename T>
void newDoc(std::string userType, Emdi & emdi, docVec_t & docVec) {
    std::string docname = docName<T>();
    auto p = std::make_unique<T>(docname);
    emdi.openDocument(p.get());
    emdi.newMdiFrame(docname, userType);
    docVec.push_back(std::move(p));
}

void quitSequence(Emdi &emdi) {
    emdi.closeAll();
    qApp->quit();
}

// TODO: Create something like defaultMenus()
// TODO: and call this when doc is closed
void updateMenus(const Emdi & emdi, const QMdiSubWindow *sw) {
    // Use visitor pattern to get list of menus
    // This is static so menus don't disappear.  This is tied
    // in with the destructor deletions of menu pointers,
    // but is bad practice probably.
    static MenuDocVisitor mdv;
    const IDocument *doc = emdi.document(sw);
    doc->accept(&mdv);
    const QList<QMenu *> & menus = mdv.menus();

    MainWindow *mw = static_cast<MainWindow *>(sw->window());
    //mw->menuBar()->clear();
    //mw->setupDefaultMenus();
    for (QMenu *qm : menus) {
        mw->menuBar()->addMenu(qm);
    }
}

void setActionChecked(const QWidget *mw, const std::string & userType, bool checked) {
    // Iterate through actions in menus
    for (const QMenu *qm : mw->findChildren<QMenu *>()) {
        for (QAction *act : qm->actions()) {
            if (act->text() == QString::fromStdString(userType)) {
                act->setChecked(checked);
            }
        }
    }
}
auto fileSaveParams(const Emdi & emdi, const MainWindow *mw) {
    // Collect doc and new filename
    const QMdiArea *mdi = static_cast<QMdiArea *>(mw->centralWidget());
    const QMdiSubWindow *sw = mdi->activeSubWindow();
    IDocument *doc = emdi.document(sw);
    ExtensionDocVisitor extvisitor;
    doc->accept(&extvisitor);
    QString extension = QString::fromStdString(extvisitor.extension());
    std::string filename = QFileDialog::getSaveFileName(nullptr, "", extension).toStdString();
    struct {
        IDocument *doc;
        std::string filename;
    } retval {doc, filename};
    return retval;
}
void fileSave(const Emdi & emdi, MainWindow *mw) {
    // Use visitor pattern to save file
    FileSaveVisitor fsv;
    const QMdiArea *mdi = static_cast<QMdiArea *>(mw->centralWidget());
    const QMdiSubWindow *sw = mdi->activeSubWindow();
    if (!sw) return;

    // Access global numberEmitter in dbutils and connect it to progress slot
    NumberEmitter &ne = dbutils::numberEmitter;
    QObject::connect(&ne, &NumberEmitter::emitDouble, mw, &MainWindow::chunkSaved);
    // Signal is disconnected at end of _dbSaveFromTo
    // Otherwise if we do it at the end here, the signal is disconnected before
    // thread is finished.

    IDocument *doc = emdi.document(sw);
    QFuture<void> f = QtConcurrent::run([doc,fsv]{
        doc->accept(&fsv);
    });

    // Keep this around so we can wait for results
    mw->addFuture(f);
}
void fileSaveAs(const Emdi & emdi, const MainWindow *mw) {
    // Collect parameters, then save and rename
    auto [doc, filename] = fileSaveParams(emdi, mw);
    if (filename.empty())
        return;

    NumberEmitter &ne = dbutils::numberEmitter;
    FileSaveCopyAsVisitor fsv(filename);
    QObject::connect(&ne, &NumberEmitter::emitDouble, mw, &MainWindow::chunkSaved);
    doc->accept(&fsv);
    //QObject::disconnect(&ne, &NumberEmitter::emitDouble, mw, &MainWindow::chunkSaved);
    emdi.renameDocument(doc, filename);
}
void fileSaveCopyAs(const Emdi & emdi, const MainWindow *mw) {
    // Collect parameters, then save.  Same as fileSaveAs, except without the rename
    auto [doc, filename] = fileSaveParams(emdi, mw);
    if (filename.empty())
        return;

    NumberEmitter &ne = dbutils::numberEmitter;
    QObject::connect(&ne, &NumberEmitter::emitDouble, mw, &MainWindow::chunkSaved);
    FileSaveCopyAsVisitor fsv(filename);
    //QObject::disconnect(&ne, &NumberEmitter::emitDouble, mw, &MainWindow::chunkSaved);
    doc->accept(&fsv);
}

QWidget *buttonWindow(Emdi & emdi, docVec_t & docVec) {
    QWidget *w = new QWidget();
    QVBoxLayout *vb = new QVBoxLayout();
    QPushButton *pb;

    pb = new QPushButton("New Symbol Library");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        newDoc<SymbolLibDocument>("Main Editor", emdi, docVec);});

    pb = new QPushButton("New Footprint Library");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        newDoc<SymbolLibDocument>("Main Editor", emdi, docVec);});

    pb = new QPushButton("New Schematic");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        newDoc<SchDocument>("Main Editor", emdi, docVec);});

    pb = new QPushButton("New PCB");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        newDoc<SchDocument>("Main Editor", emdi, docVec);});

    pb = new QPushButton("Close Current Doc");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.closeDocument();});

    pb = new QPushButton("Duplicate Current MDI");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.duplicateMdiFrame();});

    pb = new QPushButton("Popout Current MDI");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.popoutMdiFrame();});

    pb = new QPushButton("Duplicate and popout Current MDI");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.duplicateAndPopoutMdiFrame();});

    pb = new QPushButton("Move Current MDI");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.moveMdiToPrevious();});

    pb = new QPushButton("Properties Dock");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.showDockFrame("Properties");});

    pb = new QPushButton("Hierarchy Dock");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.showDockFrame("Hierarchy");});

    pb = new QPushButton("Quit");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        quitSequence(emdi);
    });

    vb->addStretch();
    w->setLayout(vb);
    return w;
}

dispatchMap_t dispatchMap(Emdi & emdi, docVec_t & docVec) {
    // Each entry in dispatch map is a lambda taking a QVariant
    // Typically the QVariant arg is carried over by the action which got triggered
    dispatchMap_t dm;
    dm["actionNewSymbolLibrary"] = [&](const QVariant &){newDoc<SymbolLibDocument>("Main Editor", emdi, docVec);};
    dm["actionNewFootprintLibrary"] = [&](const QVariant &){newDoc<FootprintLibDocument>("Main Editor", emdi, docVec);};
    dm["actionNewSchematic"] = [&](const QVariant &){newDoc<SchDocument>("Main Editor", emdi, docVec);};
    dm["actionNewPCB"] = [&](const QVariant &){newDoc<PCBDocument>("Main Editor", emdi, docVec);};
    dm["actionOpen"] = [&](const QVariant &){};
    dm["actionSave"] = [&](const QVariant & v){fileSave(emdi, v.value<MainWindow *>());};
    dm["actionSaveAs"] = [&](const QVariant & v){fileSaveAs(emdi, v.value<MainWindow *>());};
    dm["actionSaveCopyAs"] = [&](const QVariant & v){fileSaveCopyAs(emdi, v.value<MainWindow *>());};
    dm["actionCloseDoc"] = [&](const QVariant &){emdi.closeDocument();};
    dm["actionExit"] = [&](const QVariant &){quitSequence(emdi);};
    dm["actionViewProperties"] = [&](const QVariant & qv){
        if (qv.toBool()) {
            emdi.showDockFrame("Properties");
        } else {
            emdi.closeDockFrame("Properties");
        }};
    dm["actionViewHierarchy"] = [&](const QVariant & qv){
        if (qv.toBool()) {
            emdi.showDockFrame("Hierarchy");
        } else {
            emdi.closeDockFrame("Hierarchy");
        }};
    dm["actionDuplicateMDI"] = [&](const QVariant &){emdi.duplicateMdiFrame();};
    dm["actionDupAndPopoutMDI"] = [&](const QVariant &){emdi.duplicateAndPopoutMdiFrame();};
    return dm;
}

auto makeDispatch(const dispatchMap_t & dm) {
    // Returns the fn which calls the dispatch map
    return [&](QAction *act, const QVariant & qv) {
        auto fn = dm.at(act->objectName());
        fn(qv);
    };
}

auto mainCtor(const dispatchMap_t & dm) {
    return [&]() {
        MainWindow *mw = new MainWindow;
        auto dfn = makeDispatch(dm);
        QObject::connect(mw, &MainWindow::actionTriggered, dfn);
        return mw;
    };
}



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Enhanced MDI, list of documents, dispatch map
    Emdi emdi;
    docVec_t docVec;
    dispatchMap_t dm = dispatchMap(emdi, docVec);

    // Set up constructors
    emdi.setMainWindowCtor(mainCtor(dm)); // attaches action signal to dispatcher
    emdi.setMdiWindowCtor([](){return new QMdiSubWindow;});
    emdi.setDockWidgetCtor([](){return new QDockWidget;});

    // The document close signal should be exposed at this level
    // since this is where we keep the documents
    QObject::connect(&emdi, &Emdi::subWindowActivated,
        [&emdi](const QMdiSubWindow *sw) {
            updateMenus(emdi, sw);});

    QObject::connect(&emdi, &Emdi::docClosed,
        [&docVec](void *p) {
            docVec.remove_if([&](const std::unique_ptr<IDocument> & up) {
                return up.get() == static_cast<IDocument *>(p);});});
    QObject::connect(&emdi, &Emdi::dockShown, setActionChecked);

    // Main window and external toolbar
    emdi.newMainWindow();
    //QWidget *buttWindow = buttonWindow(emdi, docVec);
    //buttWindow->show();
    a.exec();

    //delete buttWindow;
    //bool dones = QThreadPool::globalInstance()->waitForDone(15000);
    //qDebug() << "All threads done?" << dones;
    qDebug("Done");

}

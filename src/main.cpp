//#include "vld.h"
#include "mainwindow.h"
#include "emdilib.h"
#include "documents.h"
#include "menuadder.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDockWidget>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <sstream>




using docVec_t = std::list<std::unique_ptr<IDocument> >;
using dispatchMap_t = std::map<QString, std::function<void()>>;

template <typename T> std::string docString() {return "undefined";}
template <> std::string docString<SymbolLibDocument>() {return "SchLibDocument_";}
template <> std::string docString<FootprintLibDocument>() {return "FootprintLibDocument_";}
template <> std::string docString<SchDocument>() {return "SchDocument_";}
template <> std::string docString<PCBDocument>() {return "PCBDocument_";}

template<typename T> std::string docName() {
    static int idx = 0;
    std::stringstream ss;
    ss << docString<T>() << std::setfill('0') <<std::setw(2) << idx++;
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

void quitSequence(Emdi &);
void quitSequence(Emdi &emdi) {
    emdi.closeAll();
    qApp->quit();
}


QWidget *buttonWindow(Emdi &, docVec_t &);
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


auto makeDispatch(const dispatchMap_t &);
auto makeDispatch(const dispatchMap_t & dm) {
    return [&](QAction *act) {
        dm.at(act->objectName())();
    };
}


auto mainCtor(const dispatchMap_t &);
auto mainCtor(const dispatchMap_t & dm) {
    return [&]() {
        MainWindow *mw = new MainWindow;
        QObject::connect(mw, &MainWindow::actionTriggered, makeDispatch(dm));
        return mw;
    };
}

void updateMenus(const Emdi &, const QMdiSubWindow *);
void updateMenus(const Emdi & emdi, const QMdiSubWindow *sw) {
    //const IMenuSource *src = reinterpret_cast<const IMenuSource *>(emdi.document(sw));
    //qDebug() << src;
    //auto m = src->menus();
    qDebug() << emdi.document(sw);
    // TODO: documents themselves are not menu sources, so this should be cast this way
    // TODO: The menu source comes from the userType
    // TODO: So there should be some standalone object associated with the subwindow's
    // TODO: usertype.

}

dispatchMap_t dispatchMap(Emdi &, docVec_t &);
dispatchMap_t dispatchMap(Emdi & emdi, docVec_t & docVec) {
    dispatchMap_t dm;
    dm["actionNewSymbolLibrary"] = [&](){newDoc<SymbolLibDocument>("Main Editor", emdi, docVec);};
    dm["actionNewFootprintLibrary"] = [&](){newDoc<FootprintLibDocument>("Main Editor", emdi, docVec);};
    dm["actionNewSchematic"] = [&](){newDoc<SchDocument>("Main Editor", emdi, docVec);};
    dm["actionNewPCB"] = [&](){newDoc<PCBDocument>("Main Editor", emdi, docVec);};
    dm["actionOpen"] = [&](){};
    dm["actionSave"] = [&](){};
    dm["actionSaveAs"] = [&](){};
    dm["actionCloseDoc"] = [&](){emdi.closeDocument();};
    dm["actionExit"] = [&](){quitSequence(emdi);};
    dm["actionViewProperties"] = [&](){emdi.showDockFrame("Properties");};
    dm["actionViewHierarchy"] = [&](){emdi.showDockFrame("Hierarchy");};
    dm["actionDuplicateMDI"] = [&](){emdi.duplicateMdiFrame();};
    dm["actionDupAndPopoutMDI"] = [&](){emdi.duplicateAndPopoutMdiFrame();};
    return dm;
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

    // Main window and external toolbar
    emdi.newMainWindow();
    //QWidget *buttWindow = buttonWindow(emdi, docVec);
    //buttWindow->show();
    a.exec();

    //delete buttWindow;
    qDebug("Done");

}

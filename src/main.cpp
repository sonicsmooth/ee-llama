//#include "vld.h"
#include "mainwindow.h"
#include "emdilib.h"
#include "documents.h"
#include "schlibdoc.h"

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




using docVec_t = std::list<std::unique_ptr<Document> >;
using dispatchMap_t = std::map<QString, std::function<void()>>;

template <typename T> std::string docString() {return "undefined";}
template <> std::string docString<SchLibDocument>() {return "SchLibDocument_";}
template <> std::string docString<SchDocument>() {return "SchDocument_";}
template <> std::string docString<TxtDocument>() {return "TxtDocument_";}

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

    pb = new QPushButton("New Sch. Library Doc");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        newDoc<SchLibDocument>("Main Editor", emdi, docVec);});

    pb = new QPushButton("New Schematic Doc");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        newDoc<SchDocument>("Main Editor", emdi, docVec);});

    pb = new QPushButton("Close Current Doc");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        emdi.closeDocument();});

    pb = new QPushButton("Close Doc by Name");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        bool ok;
        QString text = QInputDialog::getText(nullptr,
                        "Close document by name",  "Document name:",
                        QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            if (!emdi.closeDocument(text.toStdString())) {
                QMessageBox msgBox;
                msgBox.setText(QString("%1 cannot be found.").arg(text));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
            }
        }
    });

    pb = new QPushButton("Close Doc by ptr");
    vb->addWidget(pb);
    QObject::connect(pb, &QPushButton::clicked, [&](){
        if (!docVec.size())
            return;
        if (!emdi.closeDocument(docVec.front().get())) {
            QMessageBox msgBox;
            msgBox.setText("First doc cannot be found.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    });

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
        emdi.showDockFrame("Doc Properties");});

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
    return [&](QAction *act) {dm.at(act->objectName())();};
}

auto makeMainCtor(const dispatchMap_t &);
auto makeMainCtor(const dispatchMap_t & dm) {
    return [&]() {
        MainWindow *mw = new MainWindow;
        QObject::connect(mw, &MainWindow::actionTriggered, makeDispatch(dm));
        return mw;
    };
}

dispatchMap_t dispatchMap(Emdi &, docVec_t &);
dispatchMap_t dispatchMap(Emdi & emdi, docVec_t & docVec) {
    dispatchMap_t gdm;
    gdm["actionNewSymbolLibrary"] = [&](){newDoc<SchLibDocument>("Main Editor", emdi, docVec);};
    gdm["actionNewFootprintLibrary"] = [&](){};
    gdm["actionNewSchematic"] = [&](){newDoc<SchDocument>("Main Editor", emdi, docVec);};
    gdm["actionNewPCB"] = [&](){};
    gdm["actionOpen"] = [&](){};
    gdm["actionSave"] = [&](){};
    gdm["actionSaveAs"] = [&](){};
    gdm["actionCloseDoc"] = [&](){emdi.closeDocument();};
    gdm["actionExit"] = [&](){quitSequence(emdi);};
    gdm["actionViewProperties"] = [&](){emdi.showDockFrame("Properties");};
    gdm["actionViewHierarchy"] = [&](){emdi.showDockFrame("Hierarchy");};
    gdm["actionDuplicateMDI"] = [&](){emdi.duplicateMdiFrame();};
    gdm["actionDupAndPopoutMDI"] = [&](){emdi.duplicateAndPopoutMdiFrame();};
    return gdm;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Emdi emdi;
    docVec_t docVec;
    dispatchMap_t dm = dispatchMap(emdi, docVec);

    // Set up constructors.
    // Main constructor attaches action signal to dispatcher
    emdi.setMainWindowCtor(makeMainCtor(dm));
    emdi.setMdiWindowCtor([](){return new QMdiSubWindow;});
    emdi.setDockWidgetCtor([](){return new QDockWidget;});


    MainWindow *mw = static_cast<MainWindow *>(emdi.newMainWindow());
    mw->menuBar()->addAction("fromMain");

    QObject::connect(&emdi, &Emdi::docClosed, [&docVec](void *p) {
        docVec.remove_if([&](const std::unique_ptr<Document> & up) {
            return up.get() == static_cast<Document *>(p);});});

    QWidget *buttWindow = buttonWindow(emdi, docVec);
    buttWindow->show();
    a.exec();

    delete buttWindow;
    qDebug("Done");

}

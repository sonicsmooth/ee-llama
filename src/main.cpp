//#include "vld.h"
#include "buttonwindow.h"
#include "dbutils.h"
#include "documents.h"
#include "emdilib.h"
#include "emitters.h"
#include "eellama_types.h"
#include "filedialogs.h"
#include "mainwindow.h"
#include "maindispatch.h"
#include "menudocvisitor.h"
#include "task.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDockWidget>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QThreadPool>
#include <QVariant>

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <thread>

// TODO: QML
// TODO: TCL, ECL, Lua
// TODO: menus


void updateMenus(const Emdi &, const QMdiSubWindow *);
void setActionChecked(const QWidget *, const std::string & act, bool checked);


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

struct innerjunk {
    innerjunk() {qDebug() << "inner ctor";}
    ~innerjunk() {qDebug() << "inner dtor";}
};
struct outerjunk {
    innerjunk ij;
    outerjunk() {qDebug() << "outerjunk ctor";}
    ~outerjunk() {qDebug() << "outerjunk dtor";}
};

int main(int argc, char *argv[]) {

    {
        outerjunk oj;
    }

    qRegisterMetaType<IDocument *>("IDocument *");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::string>("std::string &");

    QApplication app(argc, argv);

    // Declare docVec first, then emdi, so emdi is destroyed
    // first on exit which closes out all the docs, then
    // docVec closes and destroys doc objects.
    docVec_t docVec;
    Emdi emdi;
    dispatchMap_t dm = dispatchMap(emdi, docVec);

    // Set up constructors
    emdi.setMainWindowCtor(mainCtor(dm)); // attaches action signal to dispatcher
    emdi.setMdiWindowCtor([](){return new QMdiSubWindow;});
    emdi.setDockWidgetCtor([](){return new QDockWidget;});

    // The document close signal should be exposed at this level
    // since this is where we keep the documents
    QObject::connect(&emdi, &Emdi::sigSubWindowActivated,
        [&emdi](const QMdiSubWindow *sw) {
            updateMenus(emdi, sw);});

    QObject::connect(&emdi, &Emdi::sigCloseDocument,
        [&docVec](IDocument *doc) {
            // Removing from docvec should also close document
            // Needs to go in background
            docVec.remove_if([doc](const std::unique_ptr<DocThreadWrapper> & dtw) {
                return dtw->doc() == doc;
            });});
    QObject::connect(&emdi, &Emdi::sigDockShown, setActionChecked);

    emdi.newMainWindow();

    app.exec();
    qDebug() << "Waiting for pool threads";
    QThreadPool::globalInstance()->waitForDone();
    qDebug() << "exiting";

}

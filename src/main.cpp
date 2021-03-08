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

    // Enhanced MDI, list of documents, dispatch map
    // Issue -- when everyhing shuts down, docVec is
    // destroyed first, which closes and destroyes the
    // docs, but doesn't remove them from emdi.  Then
    // emdi is destroyed, which attempts to close open
    // docs.  The docs are already closed, which would
    // be fine, but they have also been destroyed,
    // which is not fine since emdi uses pointers to
    // reference them.  So probably declare emdi then
    // docVec.
    docVec_t docVec;
    Emdi emdi;
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
            IDocument *dp = static_cast<IDocument *>(p);
            docVec.remove_if([dp](const std::unique_ptr<DocThreadWrapper> & dtw) {
                return dtw->doc() == dp;
            });});
    QObject::connect(&emdi, &Emdi::dockShown, setActionChecked);

    emdi.newMainWindow();

    app.exec();
    qDebug() << "Waiting for pool threads";
    QThreadPool::globalInstance()->waitForDone();
    qDebug() << "exiting";

}

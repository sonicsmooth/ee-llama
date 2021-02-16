//#include "vld.h"
#include "buttonwindow.h"
#include "dbutils.h"
#include "documents.h"
#include "emdilib.h"
#include "eellama_types.h"
#include "filedialogs.h"
#include "guilauncher.h"
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

// TODO: multi-thread load and save
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
            IDocument *dp = static_cast<IDocument *>(p);
            docVec.remove_if([dp](const std::unique_ptr<IDocument> & up) {
                return up.get() == dp;
            });});
    QObject::connect(&emdi, &Emdi::dockShown, setActionChecked);

    emdi.newMainWindow();

    TestClass worker("Tc1");
    QThread thr;

    QObject::connect(&thr, &QThread::started, &worker, &TestClass::doWork);
    //QObject::connect(&worker, &TestClass::progress, [](int x, int y){qDebug() << x << "out of" << y;});
    QObject::connect(&worker, &TestClass::done, []{qDebug() << "Done";});
    QObject::connect(&thr, &QThread::finished, []{qDebug() << "finished";});

    worker.moveToThread(&thr);

    //thr.start();
    a.exec();
    //thr.wait();
    //thr.quit();
    QThreadPool::globalInstance()->waitForDone();
    qDebug("Done");

}

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

    qRegisterMetaType<SymbolLibDocument>("IDocument *");
    qRegisterMetaType<SymbolLibDocument>("const std::string &");
    //qRegisterMetaType<SymbolLibDocument>("SymbolLibDocument");
    //qRegisterMetaType<SymbolLibDocument *>("SymbolLibDocument *");

    QApplication a(argc, argv);



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

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


//    // This block works well
//    TestWorker *worker1 = new TestWorker;
//    ThreadWithWaiter *thr1 = new ThreadWithWaiter(worker1);
//    QObject::connect(thr1, &QThread::started, []{qDebug()<< "thr1 started";});
//    QObject::connect(thr1, &QThread::finished, []{qDebug()<< "thr1 finished";});
//    thr1->start();
//    for (int i = 0; i < 10; i++) {
//        qDebug() << i << "main loop" << app.thread();
//        std::this_thread::sleep_for(std::chrono::milliseconds(50));
//    }
//    qDebug() << "Waiting";
//    thr1->wait();
//    delete thr1;

//    qDebug() << "Waiting 5 seconds";
//    std::this_thread::sleep_for(std::chrono::seconds(5));

    // This block works well
    TestWorker *worker2 = new TestWorker;
    QThread *thr2 = new QThread;
    QObject::connect(thr2, &QThread::started, []{qDebug()<< "thr2 started";});
    QObject::connect(thr2, &QThread::finished, []{qDebug()<< "thr2 finished";});
    QObject::connect(thr2, &QThread::finished, worker2, &QObject::deleteLater);
    QObject::connect(thr2, &QThread::finished, thr2, &QObject::deleteLater);
    worker2->moveToThread(thr2);
    thr2->start();
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            QMetaObject::invokeMethod(worker2, &Worker::work);
        }
        qDebug() << i << "main loop" << app.thread();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    qDebug() << "Waiting 8 seconds";
    std::this_thread::sleep_for(std::chrono::seconds(8));
    qDebug() << "Killing thread";
    thr2->quit();
    qDebug() << "Waiting";
    thr2->wait();





    emdi.newMainWindow();

    //TestClass worker("Tc1");
    //QThread thr;

    //QObject::connect(&thr, &QThread::started, &worker, &TestClass::doWork);
    //QObject::connect(&worker, &TestClass::progress, [](int x, int y){qDebug() << x << "out of" << y;});
    //QObject::connect(&worker, &TestClass::done, []{qDebug() << "Done";});
    //QObject::connect(&thr, &QThread::finished, []{qDebug() << "finished";});

    //worker.moveToThread(&thr);

    //thr.start();
    app.exec();
    //thr.wait();
    //thr.quit();
    QThreadPool::globalInstance()->waitForDone();

}

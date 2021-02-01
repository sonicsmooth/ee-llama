#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dbutils.h>

#include <QAction>
#include <QDebug>
#include <QDialog>
#include <QProgressDialog>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Normal actions just trigger with no further argument
    std::vector<QAction *> normalActions =
       {ui->actionExit,
        ui->actionNewSymbolLibrary,
        ui->actionNewFootprintLibrary,
        ui->actionNewSchematic,
        ui->actionNewPCB,
        ui->actionOpen,
        ui->actionExit,
        ui->actionDuplicateMDI,
        ui->actionDupAndPopoutMDI };

    // Named actions send the action itself as argument
    std::vector<QAction *> namedActions =
       {ui->actionSave,
        ui->actionSaveAs,
        ui->actionSaveCopyAs,
        ui->actionCloseDoc,
        };

    // Checked actions send whether the action's new state is checked
    std::vector<QAction *> checkedActions =
       {ui->actionViewProperties,
        ui->actionViewHierarchy };

    // For each action emit the signal with action as argument
    for (auto action : normalActions ) {
         connect(action, &QAction::triggered,
             [this, action](){
             emit actionTriggered(action, QVariant(QVariant::Bool));});
    }

    // ...and an additional argument
    for (auto action : namedActions ) {
         connect(action, &QAction::triggered,
             [this, action](){
             QVariant v = QVariant::fromValue(this);
             emit actionTriggered(action, v);});
    }

    for (auto action : checkedActions ) {
         connect(action, &QAction::triggered,
             [this, action](bool checked){
             emit actionTriggered(action, QVariant(checked));});
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDefaultMenus() {
    ui->setupUi(this);
}
void MainWindow::chunkSaved(double) {
    qDebug() << "Saving chunk!";
}
void MainWindow::addThread(QThread *thread) {
    auto p = std::unique_ptr<QThread>(thread);
    m_threads.push_back(std::move(p));
}
void MainWindow::removeThread(QThread *thread) {
    m_threads.remove(std::unique_ptr<QThread>(thread));
}

void MainWindow::closeEvent(QCloseEvent *event) {
    // Wait until all futures done
//    for (auto f : m_futures) {
//        QProgressDialog *qp = new QProgressDialog(this);
//        qp->setMinimum(0);
//        qp->setMaximum(5);
//        qp->setValue(0);
//        QObject::connect(&dbutils::numberEmitter, &NumberEmitter::emitInt,
//                         [qp](int x){qp->setValue(x);});
//        qp->show();
//        QApplication::processEvents();
//        f.waitForFinished();
//        }
    event->accept();
}

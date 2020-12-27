#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>

#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    std::vector<QAction *> normalActions =
       {ui->actionExit,
        ui->actionNewSymbolLibrary,
        ui->actionNewFootprintLibrary,
        ui->actionNewSchematic,
        ui->actionNewPCB,
        ui->actionOpen,
        ui->actionSave,
        ui->actionSaveAs,
        ui->actionCloseDoc,
        ui->actionExit,
        ui->actionDuplicateMDI,
        ui->actionDupAndPopoutMDI };
   // For each action emit the signal with action as argument
    for (auto action : normalActions ) {
         connect(action, &QAction::triggered,
             [this, action](){
             emit actionTriggered(action, QVariant(QVariant::Bool));});
    }

    std::vector<QAction *> checkedActions =
       {ui->actionViewProperties,
        ui->actionViewHierarchy };
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>

#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::vector<QAction *> actions =
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
        ui->actionViewProperties,
        ui->actionViewHierarchy,
        ui->actionDuplicateMDI,
        ui->actionDupAndPopoutMDI };
   // For each action emit the signal with action as argument
   for (auto action : actions ) {
        connect(action, &QAction::triggered, 
            [this,action](){actionTriggered(action);});
   }
}

MainWindow::~MainWindow()
{
    delete ui;
}

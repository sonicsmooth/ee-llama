#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionExit,             &QAction::triggered, [this](){emit actionTriggered(ui->actionExit);});
    connect(ui->actionNewSymbolLibrary, &QAction::triggered, [this](){emit actionTriggered(ui->actionNewSymbolLibrary);});
}

MainWindow::~MainWindow()
{
    delete ui;
}


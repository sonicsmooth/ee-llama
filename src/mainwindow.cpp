#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDispatcher(ActionDispatcher *ad) {
    m_dispatch = ad;
    QObject::connect(ui->actionExit, &QAction::triggered,
                     m_dispatch,     &ActionDispatcher::onApplicationExit);

}

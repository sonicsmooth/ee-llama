#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <actiondispatcher.h>
#include <QAction>
#include <QDebug>
#include <QMainWindow>
#include <QMenu>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
signals:
    void actionTriggered(QAction *);


};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <actiondispatcher.h>
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
    void setDispatcher(ActionDispatcher *);

private:
    Ui::MainWindow *ui;
    ActionDispatcher *m_dispatch;

};
#endif // MAINWINDOW_H

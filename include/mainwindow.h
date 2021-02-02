#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QMainWindow>
#include <QMenu>
#include <QProgressBar>
#include <QThread>
#include <QVariant>

#include <map>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void setupDefaultMenus();

private:
    Ui::MainWindow *ui;

signals:
    void actionTriggered(QAction *, const QVariant &);
public slots:
    void startProgress();
    void stopProgress();
    void setProgressValue(int, int);

};
#endif // MAINWINDOW_H

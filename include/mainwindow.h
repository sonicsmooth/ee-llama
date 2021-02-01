#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QMainWindow>
#include <QMenu>
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
    void closeEvent(QCloseEvent *) override;
    void addThread(QThread *);
    void removeThread(QThread *);

private:
    Ui::MainWindow *ui;
    std::list<std::unique_ptr<QThread>> m_threads;
signals:
    void actionTriggered(QAction *, const QVariant &);
public slots:
    void chunkSaved(double);

};
#endif // MAINWINDOW_H

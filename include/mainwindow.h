#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QCloseEvent>
#include <QDebug>
#include <QFuture>
#include <QMainWindow>
#include <QMenu>
#include <QVariant>
#include <map>

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
    void addFuture(QFuture<void>);
    void removeFuture(QFuture<void>);

private:
    Ui::MainWindow *ui;
    std::list<QFuture<void>> m_futures;
signals:
    void actionTriggered(QAction *, const QVariant &);
public slots:
    void chunkSaved(double);

};
#endif // MAINWINDOW_H

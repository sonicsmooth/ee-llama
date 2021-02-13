#ifndef GUILAUNCHER_H
#define GUILAUNCHER_H

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QObject>

// Allows you to start a gui item from another thread
template<typename T>
class GuiLauncher : public QObject {
private:
    T *w;
public:
    ~GuiLauncher() {
        qDebug() << "GuiLauncher destructor";
    }
    void addToGui() {
        moveToThread(QApplication::instance()->thread());
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }
    virtual bool event(QEvent *event) override {
        if (event->type() == QEvent::User) {
            w = new T;
            w->show();
            return true;
        }
        return false;
    }
    T *widget() const {return w;}
};


#endif // GUILAUNCHER_H

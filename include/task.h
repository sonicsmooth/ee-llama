#ifndef TASK_H
#define TASK_H

#include "guilauncher.h"
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QObject>
#include <QProgressBar>
#include <QProgressDialog>
#include <QString>
#include <QThread>
#include <functional>
#include <chrono>

static const QEvent::Type SET_VALUE =
        static_cast<QEvent::Type>(QEvent::User + 1);

// Wraps either QProgressBar or QProgressDialog
// Create an instance, then move to another thread
class ProgressDialogWrapper : public QObject {
    Q_OBJECT
private:
    QProgressDialog *mWidget;
    QString mLabelText;
    QString mCancelButtonText;
    int mMinimum;
    int mMaximum;
    QWidget *mParent;
    const Qt::WindowFlags mF;
    void create() {mWidget = new QProgressDialog(mLabelText, mCancelButtonText,
                                                 mMinimum, mMaximum, mParent, mF);}
public:
    ProgressDialogWrapper(const ProgressDialogWrapper &) {}
    ~ProgressDialogWrapper() override {
        qDebug() << "ProgressDialogWrapper destructor";
    }
    ProgressDialogWrapper(const QString &labelText,
                          const QString &cancelButtonText,
                          int minimum,
                          int maximum,
                          QWidget *parent = nullptr,
                          Qt::WindowFlags f = Qt::WindowFlags()) :
        mWidget(nullptr),
        mLabelText(labelText),
        mCancelButtonText(cancelButtonText),
        mMinimum(minimum),
        mMaximum(maximum),
        mParent(parent),
        mF(f) {}
    ProgressDialogWrapper(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) :
        mWidget(nullptr),
        mParent(parent),
        mF(f){}
    QProgressDialog *widget() {return mWidget;}
    bool event(QEvent *e) override {
        switch(e->type()) {
            case QEvent::User:
                create();
                mWidget->show();
                return true;
            case SET_VALUE:
                mWidget->setValue(mWidget->value() + 1);
                return true;
            default:
                return false;
        }
    }

public slots:
    void show() {mWidget->show();}
    void setMaximum(int x) {mWidget->setMaximum(x);}
    void setMinimum(int x) {mWidget->setMinimum(x);}
    void setValue(int x) {
        qDebug() << "setValue";
        //mWidget->setValue(x);
    }
    void requestValue() {emit requestedValue(mWidget->value());}
signals:
    void requestedValue(int);
};

Q_DECLARE_METATYPE(ProgressDialogWrapper)

// Things to try:
// Thread with dumb class in it
// Class which derives from Thread

class TestClass : public QObject {
    Q_OBJECT
private:
    QString name;
public:
    TestClass() {}
    TestClass(QString n) : name(n) {}
    ~TestClass() override {
        qDebug() << "TestClass destructor";}
public slots:
    void doWork() {
        int loopcount = 10;
        ProgressDialogWrapper pdw("LabelText", "TheButton", 0, loopcount, nullptr);
        pdw.moveToThread(QApplication::instance()->thread());
        bool okc = QObject::connect(this, qOverload<int>(&TestClass::progress),
                         &pdw, &ProgressDialogWrapper::setValue
                         ,Qt::QueuedConnection);
        qDebug() << "connect:" << okc;
        pdw.dumpObjectInfo();
        dumpObjectInfo();
        emit progress(5);
        QApplication::postEvent(&pdw, new QEvent(QEvent::User));

        std::chrono::milliseconds t(500);
        for (int i = 0; i < loopcount; i++) {
            emit progress(i);
            //emit progress(i, loopcount);
            //QApplication::postEvent(&pdw, new QEvent(SET_VALUE));
            std::this_thread::sleep_for(t);
        }
        emit done();
    }
signals:
    void progress(int);
    void progress(int, int);
    void done();
};


#endif // TASK_H

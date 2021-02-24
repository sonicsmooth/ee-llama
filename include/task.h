#ifndef TASK_H
#define TASK_H

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

static const QEvent::Type CREATE = static_cast<QEvent::Type>(QEvent::User + 0);
static const QEvent::Type SET_VALUE = static_cast<QEvent::Type>(QEvent::User + 1);


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

    public:
        ~ProgressDialogWrapper() override {
            qDebug() << "ProgressDialogWrapper destructor";
            mWidget->close(); // closes anyway when value == maximum
            delete mWidget;
    }
        ProgressDialogWrapper(const QString &labelText, const QString &cancelButtonText,
                              int minimum, int maximum, QWidget *parent = nullptr,
                              Qt::WindowFlags f = Qt::WindowFlags()) :
            mWidget(nullptr),
            mLabelText(labelText), mCancelButtonText(cancelButtonText),
            mMinimum(minimum), mMaximum(maximum), mParent(parent), mF(f) {}
        ProgressDialogWrapper(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) :
            mWidget(nullptr), mParent(parent), mF(f){}
        QProgressDialog *widget() {return mWidget;}
        // Receiving events works across thread boundary
        bool event(QEvent *e) override {
            switch(e->type()) {
                case QEvent::User:
                    create();
                    qDebug() << "user event: create";
                    return true;
                case SET_VALUE:
                    mWidget->setValue(mWidget->value() + 1);
                    qDebug() << "user event: set value";
                    return true;
                default:
                    return QObject::event(e);
            }
        }

    public slots:
        void create() {
            mWidget = new QProgressDialog(mLabelText, mCancelButtonText, mMinimum, mMaximum, mParent, mF);
            //mWidget->setAttribute(Qt::WA_DeleteOnClose);
            mWidget->show();
        }
        void setValue(int x, int y) {
            mWidget->setMaximum(y);
            mWidget->setValue(x);
        }
};


class TestClass : public QObject {
    Q_OBJECT
private:
    QString name;
public:
    TestClass(QString n) : name(n) {}
    ~TestClass() override {
        qDebug() << "TestClass destructor";
    }
public slots:
    void doWork() {
        int loopcount = 1000;
        auto pdw = new ProgressDialogWrapper("LabelText", "TheButton", 1, loopcount, nullptr);

        pdw->moveToThread(QApplication::instance()->thread());
        QObject::connect(this, &TestClass::starting, pdw, &ProgressDialogWrapper::create);
        QObject::connect(this, &TestClass::progress, pdw, &ProgressDialogWrapper::setValue);
        QObject::connect(thread(), &QThread::finished, pdw, &QObject::deleteLater);

        emit starting();
        for (int i = 0; i < loopcount+1; i++) {
            std::this_thread::sleep_for(std::chrono::microseconds(1500));
            emit progress(i, loopcount);
        }
        //emit done();
        //thread()->quit();
    }
signals:
    void starting();
    void progress(int, int);
    void done();
};

class Worker : public QObject {
    Q_OBJECT
public:
    virtual ~Worker() override {}
    // If you want normal processing, don't do infinite loop and
    // don't call thread()->quit()
    virtual void work() = 0;
};


// Use this when you want to execute a Worker's
// work() method and then stay in the loop.
// This is limited to a single function
class ThreadWithWaiter : public QThread {
private:
    Worker *w;
public:
    ThreadWithWaiter(Worker *_w) : w(_w) {
        QObject::connect(this, &QThread::finished, w, &QObject::deleteLater);
        w->moveToThread(this);}
    ~ThreadWithWaiter() override {
        qDebug() << "Thread destroyed";}
protected:
    void run() override {w->work();}
};


class TestWorker : public Worker {
    Q_OBJECT
public:
    TestWorker() {}
    ~TestWorker() override {qDebug() << "Destroyed worker";}
public slots:
    void work() override {
        qDebug() << "Work and wait";
        for (int i = 0; i < 20; ++i) {
            qDebug() << i << "working in thread" << thread();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
};





#endif // TASK_H

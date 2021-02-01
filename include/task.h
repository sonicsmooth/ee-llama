#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QThread>
#include <functional>


// Runs a function f in a separate thread
// f takes no arguments and returns nothing.

class Task : public QThread {
    Q_OBJECT
private:
    const std::function<void(void)> m_function;
public:
    Task(QObject *parent, const std::function<void(void)> &);
    void run() override;

};

#endif // TASK_H

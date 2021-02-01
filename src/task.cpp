#include "task.h"

Task::Task(QObject *parent, const std::function<void(void)> & f) :
    QThread(parent),
    m_function(f)
{

}

void Task::run() {
    m_function();
}

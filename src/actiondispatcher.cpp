#include "actiondispatcher.h"
#include <QDebug>

ActionDispatcher::ActionDispatcher(QObject *parent) : QObject(parent) {
}

void ActionDispatcher::onApplicationExit() {
    qDebug() << "action dispatch exit";
}

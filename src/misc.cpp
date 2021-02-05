#include "misc.h"
#include <QApplication>

void quitSequence(Emdi &emdi) {
    emdi.closeAll();
    qApp->quit();
}

#ifndef NUMBEREMITTER_H
#define NUMBEREMITTER_H

#include <QObject>

class IntEmitter : public QObject {
    Q_OBJECT
signals:
    void emitInt(int, int);

};


#endif // NUMBEREMITTER_H

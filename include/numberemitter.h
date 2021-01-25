#ifndef NUMBEREMITTER_H
#define NUMBEREMITTER_H

#include <QObject>


class NumberEmitter : public QObject {
    Q_OBJECT
signals:
    void emitDouble(double);
    void emitInt(int);
    void emitUInt(unsigned int);

};


#endif // NUMBEREMITTER_H

#ifndef EMITTERS_H
#define EMITTERS_H


#include <QObject>
#include <string>

class Emitter : public QObject {
    Q_OBJECT
signals:
    void sig();

};


class IntEmitter : public Emitter {
    Q_OBJECT
signals:
    //void emitInt(int);
    void emitInt(int, int);

};


class StringEmitter : public Emitter {
    Q_OBJECT
signals:
    //void stringsig(const std::string & s1);
    void stringsig(const std::string & s1, const std::string & s2);
};



#endif // EMITTERS_H

#ifndef FILEDIALOGS_H
#define FILEDIALOGS_H


#include "emdilib.h"

namespace filedialogs {
    void fileSave(const Emdi &, QObject *);
    void fileSaveAs(const Emdi &, QObject *);
    void fileSaveCopyAs(const Emdi &, QObject *);
}




#endif // FILEDIALOGS_H

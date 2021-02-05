#ifndef MAINDISPATCH_H
#define MAINDISPATCH_H

#include "eellama_types.h"
#include "emdilib.h"
#include "mainwindow.h"

#include <QAction>

dispatchMap_t dispatchMap(Emdi &, docVec_t &);

auto makeDispatchFn(const dispatchMap_t & dm) {
    // Returns the fn which calls the dispatch map
    return [&](QAction *act, const QVariant & qv) {
        auto fn = dm.at(act->objectName());
        fn(qv);
    };
}

auto mainCtor(const dispatchMap_t & dm) {
    return [&]() {
        MainWindow *mw = new MainWindow;
        auto dfn = makeDispatchFn(dm);
        QObject::connect(mw, &MainWindow::actionTriggered, dfn);
        return mw;
    };
}

#endif // MAINDISPATCH_H

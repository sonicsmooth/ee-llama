#ifndef NEWDOCS_H
#define NEWDOCS_H

#include "eellama_types.h"
#include "emdilib.h"
#include "emitters.h"
#include "documents.h"
#include "docthreadwrapper.h"


#include <QApplication>
#include <QMetaType>
#include <QThread>
#include <QThreadPool>

#include <iomanip>
#include <mutex>
#include <qdebug.h>
#include <sstream>
#include <string>
#include <thread>

namespace newdocs {



template <typename T> std::string docString() {return "undefined";}
template <> inline std::string docString<SymbolLibDocument>() {return "SymLibDocument_";}
template <> inline std::string docString<FootprintLibDocument>() {return "FootprintLibDocument_";}
template <> inline std::string docString<SchDocument>() {return "SchDocument_";}
template <> inline std::string docString<PCBDocument>() {return "PCBDocument_";}

template <typename T> std::string docExt() {return "undefined";}
template <> inline std::string docExt<SymbolLibDocument>() {return ".SymLib";}
template <> inline std::string docExt<FootprintLibDocument>() {return ".FootLib";}
template <> inline std::string docExt<SchDocument>() {return ".SchDoc";}
template <> inline std::string docExt<PCBDocument>() {return ".PcbDoc";}

template<typename T>
inline std::string docName() {
    static int idx = 0;
    std::stringstream ss;
    ss << docString<T>() << std::setfill('0') <<std::setw(2) << idx++ << docExt<T>();
    return ss.str();
}

template<typename T>
inline void _localOpen(Emdi & , const DocThreadWrapper *) {
    qDebug() << "Not specialized";
}
template<>
inline void _localOpen<SymbolLibDocument>(Emdi & emdi, const DocThreadWrapper * dtw) {
    SymbolLibDocument *doc = static_cast<SymbolLibDocument *>(dtw->get());
    IDocument *idoc = dtw->get();
    QMetaObject::invokeMethod(&emdi, "openDocument",
                              Qt::QueuedConnection,
                              Q_ARG(IDocument *, idoc));
}



template<typename T>
inline void newDoc(std::string userType, Emdi & emdi, docVec_t & docVec) {
    // Typically newDoc runs in the main thread as is it called from the menu
    // and returns quickly.
    // The doOpen is run in another thread as it may take a long time
    std::string docname = docName<T>();
    auto doOpen = [&emdi, docname, userType, &docVec]{
        auto dtw = new DocThreadWrapper(std::make_unique<T>(docname));
        auto thr = new QThread;
        qDebug() << "Main thread " << QApplication::instance()->thread();
        qDebug() << "This thread " << QThread::currentThread();
        qDebug() << "SubSubthread" << thr;
        dtw->moveToThread(thr);
        thr->start();

        // todo: try emitter again!


        qDebug() << "starting open";
//        _localOpen<T>(emdi, dtw);
        T *doc = static_cast<T *>(dtw->get());
        QMetaObject::invokeMethod(&emdi, "openDocument",
                                  Qt::DirectConnection,
                                  Q_ARG(IDocument *, doc));
        //qDebug() << "Starting sleep for 3s";
        //std::this_thread::sleep_for(std::chrono::seconds(3));

        Signal

        QMetaObject::invokeMethod(&emdi, "newMdiFrame",
                                  Qt::QueuedConnection,
                                  Q_ARG(const std::string &, docname),
                                  Q_ARG(const std::string &, userType));
        qDebug() << "opened";

        qDebug() << "Starting sleep again for 3000s";
        std::this_thread::sleep_for(std::chrono::seconds(3000));

//        static std::mutex mutex;
//        std::lock_guard<std::mutex> guard(mutex);
//        qDebug() << "Opened, pushing";
//        //docVec.push_back(std::move(p));
//        qDebug() << "Pushed";
    };
    QThreadPool::globalInstance()->start(doOpen);
}

} // namespace

#endif // NEWDOCS_H

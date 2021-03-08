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
inline void newDoc(std::string userType, Emdi & emdi, docVec_t & docVec) {
    // Typically newDoc runs in the main thread as is it called from the menu
    // and returns quickly.
    // The doOpen is run in another thread as it may take a long time
    std::string docname = docName<T>();
    auto doOpen = [&emdi, docname, userType, &docVec]{
        auto dtw = std::make_unique<DocThreadWrapper>(std::make_unique<T>(docname));
        qDebug() << "Main thread " << QApplication::instance()->thread();
        qDebug() << "Pool thread " << QThread::currentThread();
        qDebug() << "starting open synchronously";

        // Figure out how to send an exception from the child
        // thread back to here so the wrapper can be removed
        // if there is a problem opening

        T *doc = static_cast<T *>(dtw->doc());
        QMetaObject::invokeMethod(&emdi, "addWrapper",
                                  Qt::BlockingQueuedConnection,
                                  Q_ARG(IDocument *, doc),
                                  Q_ARG(DocThreadWrapper *, dtw.get()));

        QMetaObject::invokeMethod(&emdi, "openDocument",
                                  Qt::BlockingQueuedConnection,
                                  Q_ARG(IDocument *, doc));

//        QMetaObject::invokeMethod(&emdi, "newMdiFrame",
//                                  Qt::BlockingQueuedConnection,
//                                  Q_ARG(const std::string &, docname),
//                                  Q_ARG(const std::string &, userType));
        qDebug() << "opened";


        static std::mutex mutex;
        std::lock_guard<std::mutex> guard(mutex);
        docVec.push_back(std::move(dtw));
        qDebug() << "Done with pool thread";
    };
    QThreadPool::globalInstance()->start(doOpen);
}

} // namespace

#endif // NEWDOCS_H

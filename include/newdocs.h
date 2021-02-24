#ifndef NEWDOCS_H
#define NEWDOCS_H

#include "eellama_types.h"
#include "emdilib.h"
#include "emitters.h"
#include "documents.h"
#include "docthreadwrapper.h"

#include <QThreadPool>

#include <iomanip>
#include <mutex>
#include <qdebug.h>
#include <sstream>
#include <string>

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
    StringEmitter *se = new StringEmitter;

    QThread *thread = QThread::create([userType, &emdi, &docVec,
                                       docname, se]{
        auto p = std::make_unique<T>(docname);
        {
        auto dtw = DocThreadWrapper(std::move(p));
        }
        qDebug() << "starting new doc";
        //p->init();
        //emdi.openDocument(p.get());
        qDebug() << "opened";

        static std::mutex mutex;
        std::lock_guard<std::mutex> guard(mutex);
        //docVec.push_back(std::move(p));
        emit se->stringsig(docname, userType);
    });
    QObject::connect(se, &StringEmitter::stringsig,
                     &emdi, &Emdi::_newMdiFrameSlot,
                     Qt::BlockingQueuedConnection);
    QObject::connect(thread, &QThread::finished,[]{qDebug() << "thread done";});
    thread->start();
    qDebug() << "exiting";

}

} // namespace

#endif // NEWDOCS_H

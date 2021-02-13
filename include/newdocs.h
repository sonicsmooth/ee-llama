#ifndef NEWDOCS_H
#define NEWDOCS_H

#include "eellama_types.h"
#include "emdilib.h"
#include "documents.h"

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
    std::string docname = docName<T>();
    auto doOpen = [&emdi, docname, userType, &docVec]{
        auto p = std::make_unique<T>(docname);
        qDebug() << "starting open";
        emdi.openDocument(p.get());
        //emdi.newMdiFrame(docname, userType);

        static std::mutex mutex;
        std::lock_guard<std::mutex> guard(mutex);
        qDebug() << "Opened, pushing";
        docVec.push_back(std::move(p));
        qDebug() << "Pushed";
    };
    QThreadPool::globalInstance()->start(doOpen);
}

} // namespace

#endif // NEWDOCS_H

#ifndef EELLAMA_TYPES_H
#define EELLAMA_TYPES_H

#include "idocument.h"
#include "docthreadwrapper.h"
#include <QString>
#include <list>

//using docVec_t = std::list<std::unique_ptr<IDocument>>;
using docVec_t = std::list<std::unique_ptr<DocThreadWrapper>>;
using dispatchMap_t = std::map<QString, std::function<void(const QVariant &)>>;



#endif // EELLAMA_TYPES_H

#ifndef IDOCVISITOR_H
#define IDOCVISITOR_H

#include <QVariant>

// Forward declarations
class SymbolLibDocument;
class FootprintLibDocument;
class SchDocument;
class PCBDocument;

class IDocVisitor {
public:
    virtual ~IDocVisitor() = default;
    virtual const QVariant & visit(const SymbolLibDocument *) = 0;
    virtual const QVariant & visit(const FootprintLibDocument *) = 0;
    virtual const QVariant & visit(const SchDocument *) = 0;
    virtual const QVariant & visit(const PCBDocument *) = 0;
};


#endif // IDOCVISITOR_H

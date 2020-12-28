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
    ~IDocVisitor() = default;
    virtual const QVariant & visit(const SymbolLibDocument *) const = 0;
    virtual const QVariant & visit(const FootprintLibDocument *) const = 0;
    virtual const QVariant & visit(const SchDocument *) const = 0;
    virtual const QVariant & visit(const PCBDocument *) const = 0;
};


#endif // IDOCVISITOR_H

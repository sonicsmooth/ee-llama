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
    virtual void visit(SymbolLibDocument *) = 0;
    virtual void visit(FootprintLibDocument *) = 0;
    virtual void visit(SchDocument *) = 0;
    virtual void visit(PCBDocument *) = 0;
    virtual void visit(const SymbolLibDocument *) = 0;
    virtual void visit(const FootprintLibDocument *) = 0;
    virtual void visit(const SchDocument *) = 0;
    virtual void visit(const PCBDocument *) = 0;
    virtual void visit(SymbolLibDocument *) const = 0;
    virtual void visit(FootprintLibDocument *) const = 0;
    virtual void visit(SchDocument *) const = 0;
    virtual void visit(PCBDocument *) const = 0;
    virtual void visit(const SymbolLibDocument *) const = 0;
    virtual void visit(const FootprintLibDocument *) const = 0;
    virtual void visit(const SchDocument *) const = 0;
    virtual void visit(const PCBDocument *) const = 0;
};


#endif // IDOCVISITOR_H

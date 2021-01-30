#ifndef FILESAVEVISITOR_H
#define FILESAVEVISITOR_H

#include "idocvisitor.h"
#include "documents.h"

class FileSaveVisitor : public IDocVisitor
{
public:
    FileSaveVisitor();
    void visit(SymbolLibDocument *) override;
    void visit(FootprintLibDocument *) override;
    void visit(SchDocument *) override;
    void visit(PCBDocument *) override;
    void visit(const SymbolLibDocument *) override {}
    void visit(const FootprintLibDocument *) override {}
    void visit(const SchDocument *) override {}
    void visit(const PCBDocument *) override {}
    void visit(SymbolLibDocument *) const override;
    void visit(FootprintLibDocument *) const override;
    void visit(SchDocument *) const override;
    void visit(PCBDocument *) const override;
    void visit(const SymbolLibDocument *) const override {}
    void visit(const FootprintLibDocument *) const override {}
    void visit(const SchDocument *) const override {}
    void visit(const PCBDocument *) const override {}
};

#endif // FILESAVEVISITOR_H

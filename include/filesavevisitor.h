#ifndef FILESAVEVISITOR_H
#define FILESAVEVISITOR_H

#include "idocvisitor.h"
#include "documents.h"

class FileSaveVisitor : public IDocVisitor
{
public:
    FileSaveVisitor();
    void visit(const SymbolLibDocument *) override {}
    void visit(const FootprintLibDocument *) override {}
    void visit(const SchDocument *) override {}
    void visit(const PCBDocument *) override {}
    void visit(SymbolLibDocument *) override;
    void visit(FootprintLibDocument *) override;
    void visit(SchDocument *) override;
    void visit(PCBDocument *) override;
};

#endif // FILESAVEVISITOR_H

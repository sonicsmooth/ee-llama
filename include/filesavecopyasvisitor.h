#ifndef FILESAVECOPYASVISITOR_H
#define FILESAVECOPYASVISITOR_H

#include "idocvisitor.h"
#include "documents.h"

#include <string>

class FileSaveCopyAsVisitor : public IDocVisitor
{
private:
    const std::string m_newName;
    const void *m_context;
public:
    FileSaveCopyAsVisitor(const std::string &);
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

#endif // FILESAVECOPYASVISITOR_H

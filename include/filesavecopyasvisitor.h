#ifndef FILESAVECOPYASVISITOR_H
#define FILESAVECOPYASVISITOR_H

#include "idocvisitor.h"
#include "documents.h"

#include <string>

class FileSaveCopyAsVisitor : public IDocVisitor
{
private:
    const std::string m_newName;
public:
    FileSaveCopyAsVisitor(const std::string &);
    void visit(const SymbolLibDocument *) override {}
    void visit(const FootprintLibDocument *) override {}
    void visit(const SchDocument *) override {}
    void visit(const PCBDocument *) override {}
    void visit(SymbolLibDocument *) override;
    void visit(FootprintLibDocument *) override;
    void visit(SchDocument *) override;
    void visit(PCBDocument *) override;
};

#endif // FILESAVECOPYASVISITOR_H

#ifndef EXTENSIONDOCVISITOR_H
#define EXTENSIONDOCVISITOR_H

#include "idocvisitor.h"
#include <QList>
#include <QVariant>


// Concrete visitor class
// For any document, returns a generic variant
// to be specified by the function in the cpp file

class ExtensionDocVisitor : public IDocVisitor {
private:
    const std::string m_symbolLibExtension = ".SymLib";
    const std::string m_footprintLibExtension = ".FPLib";
    const std::string m_schExtension = ".SchDoc";
    const std::string m_PCBExtension = ".PCBDoc";
    const std::string * m_extension;

public:
    virtual ~ExtensionDocVisitor() override;
    ExtensionDocVisitor();
    void visit(const SymbolLibDocument *) override;
    void visit(const FootprintLibDocument *) override;
    void visit(const SchDocument *) override;
    void visit(const PCBDocument *) override;
    void visit(SymbolLibDocument *) override;
    void visit(FootprintLibDocument *) override;
    void visit(SchDocument *) override;
    void visit(PCBDocument *) override;
    const std::string & extension() const {return *m_extension;}

};


#endif // EXTENSIONDOCVISITOR_H

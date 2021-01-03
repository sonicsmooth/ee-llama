#ifndef MENUDOCVISITOR_H
#define MENUDOCVISITOR_H

#include "idocvisitor.h"
#include <QList>
#include <QMenu>
#include <QVariant>


// Concrete visitor class
// For any document, returns a generic variant
// to be specified by the function in the cpp file

class MenuDocVisitor : public IDocVisitor {
private:
    QVariant m_SymbolLibDocMenus;
    QVariant m_FootprintLibDocMenus;
    QVariant m_SchDocMenus;
    QVariant m_PCBDocMenus;

public:
    virtual ~MenuDocVisitor() override;
    MenuDocVisitor();
    const QVariant & visit(const SymbolLibDocument *) override;
    const QVariant & visit(const FootprintLibDocument *) override;
    const QVariant & visit(const SchDocument *) override;
    const QVariant & visit(const PCBDocument *) override;
};


#endif // MENUDOCVISITOR_H

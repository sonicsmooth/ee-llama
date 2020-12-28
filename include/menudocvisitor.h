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
    ~MenuDocVisitor();
    MenuDocVisitor();
    const QVariant & visit(const SymbolLibDocument *) const;
    const QVariant & visit(const FootprintLibDocument *) const;
    const QVariant & visit(const SchDocument *) const;
    const QVariant & visit(const PCBDocument *) const;
};


#endif // MENUDOCVISITOR_H

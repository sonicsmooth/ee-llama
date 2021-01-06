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
    QList<QMenu *> m_symbolLibDocMenus;
    QList<QMenu *> m_footprintLibDocMenus;
    QList<QMenu *> m_schDocMenus;
    QList<QMenu *> m_PCBDocMenus;
    QList<QMenu *> *m_returnMenus;

public:
    virtual ~MenuDocVisitor() override;
    MenuDocVisitor();
    void visit(const SymbolLibDocument *) override;
    void visit(const FootprintLibDocument *) override;
    void visit(const SchDocument *) override;
    void visit(const PCBDocument *) override;
    void visit(SymbolLibDocument *) override {}
    void visit(FootprintLibDocument *) override {}
    void visit(SchDocument *) override {}
    void visit(PCBDocument *) override {}
    const QList<QMenu *> & menus() const {return *m_returnMenus;}

};


#endif // MENUDOCVISITOR_H

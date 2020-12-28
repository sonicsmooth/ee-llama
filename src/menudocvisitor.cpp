

#include "menudocvisitor.h"
#include <QMenu>
#include <QDebug>

MenuDocVisitor::MenuDocVisitor()
{
    // We define all the extra menus for each document type here
    QList<QMenu *> symbolLibDocMenus;
    QList<QMenu *> footprintLibDocMenus;
    QList<QMenu *> schDocMenus;
    QList<QMenu *> pcbDocMenus;

    QMenu *m1 = new QMenu("menu1");
    m1->addAction("What1");
    m1->addAction("what2");
    QMenu *m2 = new QMenu("menu2");
    m2->addAction("What3");
    m2->addAction("what4");
    symbolLibDocMenus.push_back(m1);
    symbolLibDocMenus.push_back(m2);
    m_SymbolLibDocMenus.setValue(symbolLibDocMenus);

}

MenuDocVisitor::~MenuDocVisitor() {
    // TODO: Verify that deleting here doesn't
    // interfere with what happens when one of these menu items
    // is actually in use
    for (QMenu *m : m_SymbolLibDocMenus.value<QList<QMenu *>>())
        delete m;
    for (QMenu *m : m_FootprintLibDocMenus.value<QList<QMenu *>>())
        delete m;
    for (QMenu *m : m_SchDocMenus.value<QList<QMenu *>>())
        delete m;
    for (QMenu *m : m_PCBDocMenus.value<QList<QMenu *>>())
        delete m;
}

const QVariant & MenuDocVisitor::visit(const SymbolLibDocument *doc) const {
    qDebug() << "SymbolLibDocument visiting" << doc;
    return m_SymbolLibDocMenus;
}
const QVariant & MenuDocVisitor::visit(const FootprintLibDocument *doc) const {
    qDebug() << "FootprintLibDocument visiting" << doc;
    return m_FootprintLibDocMenus;
}
const QVariant & MenuDocVisitor::visit(const SchDocument *doc) const {
    qDebug() << "SchDocument visiting" << doc;
    return m_SchDocMenus;
}
const QVariant & MenuDocVisitor::visit(const PCBDocument *doc) const {
    qDebug() << "PCBDocument visiting" << doc;
    return m_PCBDocMenus;
}

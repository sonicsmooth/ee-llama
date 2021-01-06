

#include "menudocvisitor.h"
#include <QMenu>
#include <QDebug>

MenuDocVisitor::MenuDocVisitor()
{
}

MenuDocVisitor::~MenuDocVisitor() {
    // TODO: Verify that deleting here doesn't
    // interfere with what happens when one of these menu items
    // is actually in use
    for (QMenu *m : m_symbolLibDocMenus)
        delete m;
    for (QMenu *m : m_footprintLibDocMenus)
        delete m;
    for (QMenu *m : m_schDocMenus)
        delete m;
    for (QMenu *m : m_PCBDocMenus)
        delete m;
}

void MenuDocVisitor::visit(const SymbolLibDocument *doc) {
    // Mutates state of this class by adding menu items at runtime
    // This could conceivably be done in constructor and stored statically
    // But if this grows to support many document types, then we are
    // creating a bunch of menus each time the visitor is constructed
    // even though many/most of those won't be used for any given visitor
    // instance.
    // How to get actions defined here as entries in main dispatch map?
    qDebug() << "SymbolLibDocument visiting" << doc;
    if (m_symbolLibDocMenus.size() == 0) {
        QMenu *m1 = new QMenu("menu1");
        m1->addAction("What1");
        m1->addAction("what2");
        QMenu *m2 = new QMenu("menu2");
        m2->addAction("What3");
        m2->addAction("what4");
        m_symbolLibDocMenus.push_back(m1);
        m_symbolLibDocMenus.push_back(m2);
    }
    m_returnMenus = &m_symbolLibDocMenus;
}
void MenuDocVisitor::visit(const FootprintLibDocument *doc) {
    qDebug() << "FootprintLibDocument visiting" << doc;
    m_returnMenus = &m_footprintLibDocMenus;
}
void MenuDocVisitor::visit(const SchDocument *doc) {
    qDebug() << "SchDocument visiting" << doc;
    m_returnMenus = &m_schDocMenus;
}
void MenuDocVisitor::visit(const PCBDocument *doc) {
    qDebug() << "PCBDocument visiting" << doc;
    m_returnMenus = &m_PCBDocMenus;
}

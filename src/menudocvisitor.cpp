

#include "menudocvisitor.h"
#include <QMenu>
#include <QDebug>

MenuDocVisitor::MenuDocVisitor()
{
    // We define all the extra menus for each document type here
    // TODO: Maybe move these to the visit function and dynamically
    // create these.  Problem is how to delete

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

const QVariant & MenuDocVisitor::visit(const SymbolLibDocument *doc) {
    // How to get actions defined here as entries in main dispatch map?
    qDebug() << "SymbolLibDocument visiting" << doc;
    if (m_SymbolLibDocMenus.toList().size() == 0) {
        QList<QMenu *> symbolLibDocMenus;
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
    return m_SymbolLibDocMenus;
}
const QVariant & MenuDocVisitor::visit(const FootprintLibDocument *doc) {
    qDebug() << "FootprintLibDocument visiting" << doc;
    return m_FootprintLibDocMenus;
}
const QVariant & MenuDocVisitor::visit(const SchDocument *doc) {
    qDebug() << "SchDocument visiting" << doc;
    return m_SchDocMenus;
}
const QVariant & MenuDocVisitor::visit(const PCBDocument *doc) {
    qDebug() << "PCBDocument visiting" << doc;
    return m_PCBDocMenus;
}

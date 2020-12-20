#include "documents.h"

#include <QTextEdit>
#include <QDebug>

#include <string>

SymbolLibDocument::SymbolLibDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
    QMenu *m1 = new QMenu("SymbolLibMenu1");
    m1->addAction("Open");
    QMenu *m2 = new QMenu("SymbolLibMenu2");
    m2->addAction("File");
    m_menus.push_back(m1);
    m_menus.push_back(m2);

}
SymbolLibDocument::~SymbolLibDocument() {
    done();
    qDebug() << QString("SymbolLibDocument::~SymbolLibDocument() (%1)").arg(m_name.c_str());
    for (auto m : m_menus)
        delete m;
}
void SymbolLibDocument::init() {
    if (m_activeState)
        return;
    m_activeState = true;
}
void SymbolLibDocument::done() {
    if (!m_activeState)
        return;
    m_activeState = false;
}
bool SymbolLibDocument::isActive() {
    return m_activeState;
}
bool SymbolLibDocument::supportsUserType(const std::string & userType) const {
    return userType == "Main Editor" ||
           userType == "Properties";
}
QWidget *SymbolLibDocument::newView(const std::string & userType) const {
    if (supportsUserType(userType)) {
        return new QTextEdit(QString("SymbolLibDocument/%1/%2").
                            arg(m_name.c_str()).arg(userType.c_str()));
    } else {
        return nullptr;
    }
}
const std::string & SymbolLibDocument::name() const {
    return m_name;
}
const menus_t & SymbolLibDocument::menus() const {
    return m_menus;
}


FootprintLibDocument::FootprintLibDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
    QMenu *m1 = new QMenu("SymbolLibMenu1");
    m1->addAction("Open");
    QMenu *m2 = new QMenu("SymbolLibMenu2");
    m2->addAction("File");
    m_menus.push_back(m1);
    m_menus.push_back(m2);

}
FootprintLibDocument::~FootprintLibDocument() {
    done();
    qDebug() << QString("FootprintLibDocument::~FootprintLibDocument() (%1)").arg(m_name.c_str());
    for (auto m : m_menus)
        delete m;
}
void FootprintLibDocument::init() {
    if (m_activeState)
        return;
    m_activeState = true;
}
void FootprintLibDocument::done() {
    if (!m_activeState)
        return;
    m_activeState = false;
}
bool FootprintLibDocument::isActive() {
    return m_activeState;
}
bool FootprintLibDocument::supportsUserType(const std::string & userType) const {
    return userType == "Main Editor" ||
           userType == "Properties";
}
QWidget *FootprintLibDocument::newView(const std::string & userType) const {
    if (supportsUserType(userType)) {
        return new QTextEdit(QString("FootprintLibDocument/%1/%2").
                            arg(m_name.c_str()).arg(userType.c_str()));
    } else {
        return nullptr;
    }
}
const std::string & FootprintLibDocument::name() const {
    return m_name;
}
const menus_t & FootprintLibDocument::menus() const {
    return m_menus;
}


SchDocument::SchDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
    QMenu *m1 = new QMenu("SchMenu1");
    m1->addAction("Rice");
    QMenu *m2 = new QMenu("SchMenu2");
    m2->addAction("aroni");
    m_menus.push_back(m1);
    m_menus.push_back(m2);
}
SchDocument::~SchDocument() {
    done();
    qDebug() << QString("SchDocument::~SchDocument() (%1)").arg(m_name.c_str());
    for (auto m : m_menus)
        delete m;
}
void SchDocument::init() {
    if (m_activeState)
        return;
    m_activeState = true;
}
void SchDocument::done() {
    if (!m_activeState)
        return;
    m_activeState = false;
}
bool SchDocument::isActive() {
    return m_activeState;
}
bool SchDocument::supportsUserType(const std::string & userType) const {
    return userType == "Main Editor"    ||
           userType == "Properties" ||
           userType == "Hierarchy";
}
QWidget *SchDocument::newView(const std::string & userType) const {
    if (supportsUserType(userType)) {
        return new QTextEdit(QString("SchDocument/%1/%2").
                             arg(m_name.c_str()).arg(userType.c_str()));
    } else {
        return nullptr;
    }

}
const std::string & SchDocument::name() const {
    return m_name;
}
const menus_t & SchDocument::menus() const {
    return m_menus;
}


PCBDocument::PCBDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
    QMenu *m1 = new QMenu("SchMenu1");
    m1->addAction("Rice");
    QMenu *m2 = new QMenu("SchMenu2");
    m2->addAction("aroni");
    m_menus.push_back(m1);
    m_menus.push_back(m2);
}
PCBDocument::~PCBDocument() {
    done();
    qDebug() << QString("PCBDocument::~PCBDocument() (%1)").arg(m_name.c_str());
    for (auto m : m_menus)
        delete m;
}
void PCBDocument::init() {
    if (m_activeState)
        return;
    m_activeState = true;
}
void PCBDocument::done() {
    if (!m_activeState)
        return;
    m_activeState = false;
}
bool PCBDocument::isActive() {
    return m_activeState;
}
bool PCBDocument::supportsUserType(const std::string & userType) const {
    return userType == "Main Editor"    ||
           userType == "Properties";
}
QWidget *PCBDocument::newView(const std::string & userType) const {
    if (supportsUserType(userType)) {
        return new QTextEdit(QString("PCBDocument/%1/%2").
                             arg(m_name.c_str()).arg(userType.c_str()));
    } else {
        return nullptr;
    }

}
const std::string & PCBDocument::name() const {
    return m_name;
}
const menus_t & PCBDocument::menus() const {
    return m_menus;
}


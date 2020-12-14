#include "documents.h"

#include <QTextEdit>
#include <QDebug>

#include <string>

TxtDocument::TxtDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
    QMenu *m1 = new QMenu("TxtMenu1");
    m1->addAction("What");
    QMenu *m2 = new QMenu("TxtMenu2");
    m2->addAction("Again?");
    m_menus.push_back(m1);
    m_menus.push_back(m2);
}
TxtDocument::~TxtDocument() {
    done();
    qDebug() << QString("TxtDocument::~TxtDocument() (%1)").arg(m_name.c_str());
    for (auto m : m_menus)
        delete m;
}
void TxtDocument::init() {
    if (m_activeState)
        return;
    m_activeState = true;
}
void TxtDocument::done() {
    if (!m_activeState)
        return;
    m_activeState = false;
}
bool TxtDocument::isActive() {
    return m_activeState;
}
bool TxtDocument::supportsUserType(const std::string & userType) const {
    return userType == "Main Editor" ||
           userType == "Doc Properties";
}
QWidget *TxtDocument::newView(const std::string & userType) const {
    if (supportsUserType(userType)) {
        return new QTextEdit(QString("TxtDocument/%1/%2").
                            arg(m_name.c_str()).arg(userType.c_str()));
    } else {
        return nullptr;
    }
}
const std::string & TxtDocument::name() const {
    return m_name;
}
const menus_t & TxtDocument::menus() const {
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
           userType == "Doc Properties" ||
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

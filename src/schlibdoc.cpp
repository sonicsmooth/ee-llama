#include "schlibdoc.h"

#include <QTextEdit>
#include <QDebug>

SchLibDocument::SchLibDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
    QMenu *m1 = new QMenu("SchLibMenu1");
    m1->addAction("Open");
    QMenu *m2 = new QMenu("SchLibMenu2");
    m2->addAction("File");
    m_menus.push_back(m1);
    m_menus.push_back(m2);

}
SchLibDocument::~SchLibDocument() {
    done();
    qDebug() << QString("SchLibDocument::~SchLibDocument() (%1)").arg(m_name.c_str());
    for (auto m : m_menus)
        delete m;
}
void SchLibDocument::init() {
    if (m_activeState)
        return;
    m_activeState = true;
}
void SchLibDocument::done() {
    if (!m_activeState)
        return;
    m_activeState = false;
}
bool SchLibDocument::isActive() {
    return m_activeState;
}
bool SchLibDocument::supportsUserType(const std::string & userType) const {
    return userType == "Main Editor" ||
           userType == "Doc Properties";
}
QWidget *SchLibDocument::newView(const std::string & userType) const {
    if (supportsUserType(userType)) {
        return new QTextEdit(QString("SchLibDocument/%1/%2").
                            arg(m_name.c_str()).arg(userType.c_str()));
    } else {
        return nullptr;
    }
}
const std::string & SchLibDocument::name() const {
    return m_name;
}
const menus_t & SchLibDocument::menus() const {
    return m_menus;
}

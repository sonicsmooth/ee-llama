#include "documents.h"
#include "dbutils.h"

#include <QTextEdit>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <string>



SymbolLibDocument::SymbolLibDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
}
SymbolLibDocument::~SymbolLibDocument() {
    done();
    qDebug() << QString("SymbolLibDocument::~SymbolLibDocument() (%1)").arg(m_name.c_str());
}
void SymbolLibDocument::init() {
    if (m_activeState) {
        return;
    } else {
        QString name = QString::fromStdString(m_name);
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", name);
        db.setDatabaseName(name);
        db.open();
        QSqlQuery query(db);
        const
        QStringList qsl = {"CREATE TABLE hello (ID  INTEGER PRIMARY KEY AUTOINCREMENT, \n"
                           "                   name TEXT CHECK(length(name) > 0));       "};
        executeList(query, qsl, "Could not init", __LINE__);
    }
    m_activeState = true;
}
void SymbolLibDocument::done() {
    if (!m_activeState) {
        return;
    } else {
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(m_name));
            db.close();
        }
        QSqlDatabase::removeDatabase(QString::fromStdString(m_name));
    }
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
const QVariant & SymbolLibDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}


FootprintLibDocument::FootprintLibDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
}
FootprintLibDocument::~FootprintLibDocument() {
    done();
    qDebug() << QString("FootprintLibDocument::~FootprintLibDocument() (%1)").arg(m_name.c_str());
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
const QVariant & FootprintLibDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}

SchDocument::SchDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
}
SchDocument::~SchDocument() {
    done();
    qDebug() << QString("SchDocument::~SchDocument() (%1)").arg(m_name.c_str());
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
const QVariant & SchDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}

PCBDocument::PCBDocument(const std::string & name) :
    m_name(name),
    m_activeState(false)
{
}
PCBDocument::~PCBDocument() {
    done();
    qDebug() << QString("PCBDocument::~PCBDocument() (%1)").arg(m_name.c_str());
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
const QVariant & PCBDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}


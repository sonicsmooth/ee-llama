#include "documents.h"
#include "dbutils.h"

#include <QTextEdit>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QThread>

#include <cstdio>
#include <string>



SymbolLibDocument::SymbolLibDocument(const std::string & name) :
    m_name(name),
    m_connName(dbutils::connName("SLCONN")),
    m_activeState(false)
{
    qDebug() << m_connName.c_str();
}
SymbolLibDocument::~SymbolLibDocument() {
    done();
    qDebug() << QString("SymbolLibDocument::~SymbolLibDocument() (%1)").arg(m_name.c_str());
}
void SymbolLibDocument::init() {
    // Todo: capture or store the thread which called this
    // Hopefully the thread doesn't die before the doc is done()
    qDebug() << "Starting doc init in" << QThread::currentThread();
    if (m_activeState) {
        return;
    } else {
        // Creates temp database to prime save, save as, etc.
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(m_connName));
#if defined(QT_DEBUG)
        db.setDatabaseName("tmp_" + QString::fromStdString(m_name));
#elif defined(QT_NO_DEBUG)
        QString dbName = "tmp_" + QString::fromStdString(m_name);
        QString fileName = QString("file:%1?mode=memory&cache=shared").arg(dbName);
        db.setDatabaseName(fileName);
#endif
        if(!db.open()) {
            qDebug() << "Can't create database";
        }

        QSqlQuery query(db);
        const
        QStringList qsl = {"DROP TABLE IF EXISTS SymbolTable;",
                           "CREATE TABLE SymbolTable (ID  INTEGER PRIMARY KEY AUTOINCREMENT, \n"
                           "                          name TEXT CHECK(length(name) > 0));       ",
                           "INSERT INTO SymbolTable (name) VALUES ('giraffe');"};
        dbutils::executeList(query, qsl, "Could not init", __LINE__);

        query.exec("BEGIN");
        QString s = QString("INSERT INTO SymbolTable (name) VALUES (:v);");
        query.prepare(s);

        for (int i = 0; i < 100000; i++) {
            query.bindValue(":v", QVariant(i));
            query.exec();
        }
        qDebug() << "Doc init done";
        query.exec("COMMIT");
    }
    m_activeState = true;
}
void SymbolLibDocument::done() {
    if (!m_activeState) {
        return;
    } else {
        // Removes and deletes temp database
        qDebug() << "Done in" << QThread::currentThread();
        QString dbname = QString::fromStdString(m_connName);
        QString filename = "tmp_" + QString::fromStdString(m_name);
        {
            // Todo: find the thread which opened this database
            QSqlDatabase db = QSqlDatabase::database(dbname);
            db.close();
        }
        QSqlDatabase::removeDatabase(dbname);
        std::remove(filename.toLatin1());
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
void SymbolLibDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}
void SymbolLibDocument::accept(IDocVisitor *dv) const {
    return dv->visit(this);
}
void SymbolLibDocument::accept(const IDocVisitor *dv) {
    return dv->visit(this);
}
void SymbolLibDocument::accept(const IDocVisitor *dv) const {
    return dv->visit(this);
}
void SymbolLibDocument::setName(const std::string & newName) {
    m_name = newName;
}
void SymbolLibDocument::save() const {
    // Saves the document using connName
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, m_name);
    // todo: dirty bit
}
void SymbolLibDocument::saveCopyAs(const std::string & name) const {
    // Save current database connection m_name as file name
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, name);
}

FootprintLibDocument::FootprintLibDocument(const std::string & name) :
    m_name(name),
    m_connName(dbutils::connName("FLCONN")),
    m_activeState(false)
{
    qDebug() << m_connName.c_str();
}
FootprintLibDocument::~FootprintLibDocument() {
    done();
    qDebug() << QString("FootprintLibDocument::~FootprintLibDocument() (%1)").arg(m_name.c_str());
}
void FootprintLibDocument::init() {
    if (m_activeState) {
        return;
    } else {
        // Creates temp database to prime save, save as, etc.
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(m_connName));
#if defined(QT_DEBUG)
        db.setDatabaseName("tmp_" + QString::fromStdString(m_name));
#elif defined(QT_NO_DEBUG)
        QString dbName = "tmp_" + QString::fromStdString(m_name);
        QString fileName = QString("file:%1?mode=memory&cache=shared").arg(dbName);
        db.setDatabaseName(fileName);
#endif
        if(!db.open()) {
            qDebug() << "Can't create database";
        }

        QSqlQuery query(db);
        const
        QStringList qsl = {"DROP TABLE IF EXISTS FootprintTable;",
                           "CREATE TABLE FootprintTable (ID  INTEGER PRIMARY KEY AUTOINCREMENT, \n"
                           "                             name TEXT CHECK(length(name) > 0));       ",
                           "INSERT INTO FootprintTable (name) VALUES ('giraffe');"};
        dbutils::executeList(query, qsl, "Could not init", __LINE__);

        query.exec("BEGIN");
        QString s = QString("INSERT INTO FootprintTable (name) VALUES (:v);");
        query.prepare(s);

        for (int i = 0; i < 1000; i++) {
            query.bindValue(":v", QVariant(i));
            query.exec();
        }
        query.exec("COMMIT");
        qDebug() << "done filling db";
    }
    m_activeState = true;
}
void FootprintLibDocument::done() {
    if (!m_activeState) {
        return;
    } else {
        // Removes and deletes temp database
        QString dbname = QString::fromStdString(m_connName);
        QString filename = "tmp_" + QString::fromStdString(m_name);
        {
            QSqlDatabase db = QSqlDatabase::database(dbname);
            db.close();
        }
        QSqlDatabase::removeDatabase(dbname);
        std::remove(filename.toLatin1());
    }
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
void FootprintLibDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}
void FootprintLibDocument::accept(const IDocVisitor *dv) {
    return dv->visit(this);
}
void FootprintLibDocument::accept(IDocVisitor *dv) const {
    return dv->visit(this);
}
void FootprintLibDocument::accept(const IDocVisitor *dv) const {
    return dv->visit(this);
}
void FootprintLibDocument::setName(const std::string & newName) {
    m_name = newName;
}
void FootprintLibDocument::save() const {
    // Saves the document using connName
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, m_name);
}

void FootprintLibDocument::saveCopyAs(const std::string & name) const {
    // Save current database connection m_name as file name
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, name);
}

SchDocument::SchDocument(const std::string & name) :
    m_name(name),
    m_connName(dbutils::connName("SCHCONN")),
    m_activeState(false)
{
    qDebug() << m_connName.c_str();
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
void SchDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}
void SchDocument::accept(const IDocVisitor *dv) {
    return dv->visit(this);
}
void SchDocument::accept(IDocVisitor *dv) const {
    return dv->visit(this);
}
void SchDocument::accept(const IDocVisitor *dv) const {
    return dv->visit(this);
}
void SchDocument::setName(const std::string & newName) {
    m_name = newName;
}
void SchDocument::save() const {
    // Saves the document using connName
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, m_name);
}
void SchDocument::saveCopyAs(const std::string & name) const {
    // Save current database connection m_name as file name
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, name);
}


PCBDocument::PCBDocument(const std::string & name) :
    m_name(name),
    m_connName(dbutils::connName("PCONN")),
    m_activeState(false)
{
    qDebug() << m_connName.c_str();
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
void PCBDocument::accept(IDocVisitor *dv) {
    return dv->visit(this);
}
void PCBDocument::accept(const IDocVisitor *dv) {
    return dv->visit(this);
}
void PCBDocument::accept(IDocVisitor *dv) const {
    return dv->visit(this);
}
void PCBDocument::accept(const IDocVisitor *dv) const {
    return dv->visit(this);
}
void PCBDocument::setName(const std::string & newName) {
    m_name = newName;
}
void PCBDocument::save() const {
    // Saves the document using connName
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, m_name);
}
void PCBDocument::saveCopyAs(const std::string & name) const {
    // Save current database connection m_name as file name
    if (m_activeState)
        dbutils::dbSaveFromTo(m_connName, name);
}


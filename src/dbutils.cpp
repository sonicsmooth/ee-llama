
#include "dbutils.h"
#include "include/sqlite3/sqlite3.h"

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <stdexcept>
#include <iomanip>
#include <sstream>

// This is largely/mostly a copy of what's in emdilib.cpp.


namespace dbutils {

void fatalStr(const QString & inftxt, int line) {
    qDebug(inftxt.toLatin1());
    QMessageBox mb;
    mb.setIcon(QMessageBox::Critical);
    mb.setWindowTitle("DB Error");
    if (line)
        mb.setText("Line #" + QString::number(line));
    mb.setInformativeText(inftxt);
    mb.exec();
    throw(std::logic_error(inftxt.toLatin1()));
}


QString querr(const QString & comment, const QSqlQuery & query) {
    // Concatenates comment, last error, and query text with \n
    return QString("%1\n%2\n%3").
                   arg(comment).
                   arg(query.lastError().text()).
                   arg(query.lastQuery());
}

void executeList(QSqlQuery & query, const QStringList & qsl, const QString & errstr, int linenum) {
    for (const QString & qs: qsl)
        if (!query.exec(qs))
            fatalStr(querr(errstr, query), linenum);
}
void dbSaveFromTo(const std::string & connFrom, const std::string & fileTo) {
    // Uses sqlite3 backup mechanism to write database connFrom to fileTo
    QSqlDatabase db = QSqlDatabase::database(QString::fromStdString(connFrom));
    QVariant qvhandle = db.driver()->handle();
    if (qvhandle.isValid() && qstrcmp(qvhandle.typeName(), "sqlite3*") == 0) {
        sqlite3 *pFrom = *static_cast<sqlite3 **>(qvhandle.data());
        sqlite3 *pTo;
        sqlite3_open(fileTo.c_str(), &pTo);
        sqlite3_backup *pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
        if (pBackup) {
            (void) sqlite3_backup_step(pBackup, -1);
            (void) sqlite3_backup_finish(pBackup);
            if (sqlite3_errcode(pFrom) != SQLITE_OK) {
                qDebug() << sqlite3_errmsg(pFrom);
            }
            if (sqlite3_errcode(pTo) != SQLITE_OK) {
                qDebug() << sqlite3_errmsg(pTo);
            }
        } else {
            throw std::logic_error("sqlite3_backup_init(...) failed");
        }
        // causes error 21 bad parameter or other API misuse,
        // But this occurs even if open and close are called back-to-back
        // with no operations in between
        sqlite3_close(pTo);
        if (sqlite3_errcode(pFrom) != SQLITE_OK) {
            qDebug() << sqlite3_errmsg(pFrom);
        }
        if (sqlite3_errcode(pTo) != SQLITE_OK) {
            qDebug() << sqlite3_errmsg(pTo);
        }
    } else {
        throw std::logic_error("invalid driver handle");
    }
}

std::string connName(const std::string & prefix) {
    // Returns sequential name incremented each time
    static int n = 0;
    std::stringstream oss;
    oss << prefix << std::setw(3) << std::setfill('0') << n++;
    return oss.str();
}

}; // namespace

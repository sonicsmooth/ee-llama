
//#include "dbclone.h"
#include "dbutils.h"
#include "sqlite3/sqlite3.h"
#include "numberemitter.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QRunnable>
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QThreadPool>
#include <QVariant>

#include <chrono>
#include <future>
#include <functional>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <thread>

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
                   arg(comment,
                       query.lastError().text(),
                       query.lastQuery());
}

void executeList(QSqlQuery & query, const QStringList & qsl, const QString & errstr, int linenum) {
    for (const QString & qs: qsl)
        if (!query.exec(qs))
            fatalStr(querr(errstr, query), linenum);
}

std::string dbNameFromConnName(const std::string & conn) {
    // Returns database name from connection name.
    // Since this fn may be called from another thread, we need
    // to clone the connection and cleanup.
    // Get filename from connection name.  Since this fn is
    // probably called from another thread, we need to clone first
    QString dbName;
    QString qsConn = QString::fromStdString(conn);
    {
        // Todo: find a way to guarantee unique clone name
        QSqlDatabase db = QSqlDatabase::cloneDatabase(qsConn, "CloneDb");
        if (!db.open()) {
            throw std::logic_error("Could not open cloned database");
        }
        dbName = db.databaseName();
        db.close();
    }
    QSqlDatabase::removeDatabase("CloneDb");
    return dbName.toStdString();
}

// Available to other modules as global dbutils::numberEmitter
IntEmitter intEmitter;
void dbSaveFromTo(const std::string & connFrom, const std::string & fileTo) {
    // Uses sqlite3 backup mechanism to write database connFrom to fileTo

    // Could be in-memory or tmp file
    std::string fileFrom = dbNameFromConnName(connFrom);

    sqlite3 *pFrom;
    if (sqlite3_open(fileFrom.c_str(), &pFrom) != SQLITE_OK) {
        qDebug() << sqlite3_errmsg(pFrom);
        throw std::logic_error("Could not open 'from' database");
    }

    sqlite3 *pTo;
    if (sqlite3_open(fileTo.c_str(), &pTo) != SQLITE_OK) {
        qDebug() << sqlite3_errmsg(pTo);
        throw std::logic_error("Could not open 'to' database");
    }

    sqlite3_backup *pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
    if (!pBackup) {
        throw std::logic_error("sqlite3_backup_init(...) failed");
    }

    int pagesPerCycle = 50;
    int pageCount = 0;
    int pagesCopied = 0;
    int pagesRemaining = 0;
    do {
        sqlite3_backup_step(pBackup, pagesPerCycle);
        if (sqlite3_errcode(pFrom) != SQLITE_OK) {
            qDebug() << sqlite3_errmsg(pFrom);
        }
        if (sqlite3_errcode(pTo) != SQLITE_OK) {
            qDebug() << sqlite3_errmsg(pTo);
        }
        pageCount = sqlite3_backup_pagecount(pBackup);
        pagesRemaining = sqlite3_backup_remaining(pBackup);
        pagesCopied = pageCount - pagesRemaining;
        emit intEmitter.emitInt(pagesCopied, pageCount);
    } while(pagesRemaining > 0);
    (void) sqlite3_backup_finish(pBackup);

    // causes error 21 bad parameter or other API misuse,
    // But this occurs even if open and close are called back-to-back
    // with no operations in between
    sqlite3_close(pFrom);
    if (sqlite3_errcode(pFrom) != SQLITE_OK) {
        qDebug() << sqlite3_errmsg(pFrom);
    }
    sqlite3_close(pTo);
    if (sqlite3_errcode(pTo) != SQLITE_OK) {
        qDebug() << sqlite3_errmsg(pTo);
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

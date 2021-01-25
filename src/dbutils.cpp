
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

// Available to other modules as global
NumberEmitter numberEmitter;
static void _dbSaveFromTo(const std::string & connFrom, const std::string & fileTo) {
    // Uses sqlite3 backup mechanism to write database connFrom to fileTo

    auto secs = std::chrono::seconds(1);
    for (int i = 0; i < 5; i++) {
        qDebug() << "in loop";
        std::this_thread::sleep_for(secs);
        double fragment = 1.0 / 5;
        try {
            emit numberEmitter.emitDouble(fragment * i);
        }
        catch (...) {
            qDebug() << "error caught";
            // TODO: do something here?
        }
    }

    QString qsConnFrom = QString::fromStdString(connFrom);
    // Need to clone db so it can be used from this function, which is callable
    // as another thread
    QSqlDatabase db = QSqlDatabase::cloneDatabase(qsConnFrom, "CloneDb");
    db.open();
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
    numberEmitter.disconnect();
}

class SaveRunnable : public QRunnable {
private:
    const std::string m_connFrom;
    const std::string m_fileTo;
public:
    SaveRunnable(const std::string & connFrom, const std::string & fileTo) :
        m_connFrom(connFrom),
        m_fileTo(fileTo){}
    void run() override {
        qDebug() << "Running from other thread.run()";
        _dbSaveFromTo(m_connFrom, m_fileTo);
    }
};

void dbSaveFromTo(const std::string & connFrom, const std::string & fileTo) {
    // Forward call to potentially long-running function
    // Can't let future be desroyed at end of function, so we assign it to a static global
    //saveFuture = std::async(std::launch::async, _dbSaveFromTo, connFrom, fileTo);
    //std::thread(_dbSaveFromTo, connFrom, fileTo).detach();
    //QThread::create(_dbSaveFromTo, connFrom, fileTo)->start(); // ~equivalent
    SaveRunnable *sr = new SaveRunnable(connFrom, fileTo);
    // TODO: skip making a class and its instance and use lambda instead
    QThreadPool::globalInstance()->start(sr);
}

std::string connName(const std::string & prefix) {
    // Returns sequential name incremented each time
    static int n = 0;
    std::stringstream oss;
    oss << prefix << std::setw(3) << std::setfill('0') << n++;
    return oss.str();
}

}; // namespace


#include "dbutils.h"

#include <QMessageBox>
#include <QString>
#include <QSqlError>
#include <QSqlQuery>

// This is largely/mostly a copy of what's in emdilib.cpp.


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

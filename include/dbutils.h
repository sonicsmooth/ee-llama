#ifndef DBUTILS_H
#define DBUTILS_H

#include <QString>
#include <QSqlQuery>
#include <string>

namespace dbutils {

void fatalStr(const QString & inftxt, int line);
QString querr(const QString & comment, const QSqlQuery & query);
void executeList(QSqlQuery & query, const QStringList & qsl, const QString & errstr, int linenum);
void dbSaveFromTo(const std::string & connFrom, const std::string & fileTo);
std::string connName(const std::string & = "");
};

#endif // DBUTILS_H

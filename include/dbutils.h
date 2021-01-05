#ifndef DBUTILS_H
#define DBUTILS_H

#include <QString>
#include <QSqlQuery>

void fatalStr(const QString & inftxt, int line);
QString querr(const QString & comment, const QSqlQuery & query);
void executeList(QSqlQuery & query, const QStringList & qsl, const QString & errstr, int linenum);



#endif // DBUTILS_H

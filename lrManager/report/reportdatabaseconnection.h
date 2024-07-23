#ifndef REPORTDATABASECONNECTION_H
#define REPORTDATABASECONNECTION_H

#include <QtSql/QSqlDatabase>

/*!
    @ClassName   : ReportDatabaseConnection
    @Description : 单张报表数据连接
*/
class ReportDatabaseConnection
{
private:
    QSqlDatabase *m_reportDB;
    QString m_lastError;

public:
    ReportDatabaseConnection();
    ~ReportDatabaseConnection();

    QSqlDatabase *getReportDB() { return m_reportDB; }
    QString getLastError() { return m_lastError; }
};

#endif // REPORTDATABASECONNECTION_H

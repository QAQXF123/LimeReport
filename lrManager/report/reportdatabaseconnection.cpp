#include "reportdatabaseconnection.h"
#include <QSqlError>

ReportDatabaseConnection::ReportDatabaseConnection()
{
    m_reportDB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_reportDB->setDatabaseName(":memory:");
    if (!m_reportDB->open())
        m_lastError = m_reportDB->lastError().text();
}

ReportDatabaseConnection::~ReportDatabaseConnection()
{
    if (m_reportDB) {
        if (m_reportDB->isOpen())
            m_reportDB->close();
        delete m_reportDB;
    }
}

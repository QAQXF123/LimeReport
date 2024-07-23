#include "reportfilemgr.h"
#include <QDataStream>
#include <QDebug>
#include "utils/pmquazipcompress.h"

// 报表包：配置文件
const QString cst_ReportPackage_ConfigFile = "reportConfig.xml";
// 报表包：格式文件
const QString cst_ReportPackage_FormatFile = "reportFormat.lrxml";
// Copyright
const QString cst_ReportPackage_Copyright = "Copyright  2024 The PMSS All rights reserved ";

ReportFileMgr::ReportFileMgr()
{
    m_reportConfigMgr = new ReportConfigMgr();
    m_reportFormatMgr = new ReportFormatMgr();
}

ReportFileMgr::~ReportFileMgr()
{
    if (m_reportConfigMgr)
        delete m_reportConfigMgr;
    if (m_reportFormatMgr)
        delete m_reportFormatMgr;
}

bool ReportFileMgr::openReport(const QString &fileName)
{
    // 1、解压
    QDataStream configStream;

    QByteArray byteConfig;
    if (!PMQuaZipCompress::pmExtractFile(fileName,
                                         cst_ReportPackage_ConfigFile,
                                         byteConfig,
                                         cst_ReportPackage_Copyright)) {
        qDebug() << "open Report extract config file error.";
        return false;
    }
    // 2、读报表配置文件

    QByteArray byteFormat;
    if (!PMQuaZipCompress::pmExtractFile(fileName,
                                         cst_ReportPackage_FormatFile,
                                         byteFormat,
                                         cst_ReportPackage_Copyright)) {
        qDebug() << "open Report extract format file error.";
        return false;
    }
    // 3、读报表格式文件

    return true;
}

bool ReportFileMgr::saveReport(const QString &fileName)
{
    QStringList names;
    names.push_back(cst_ReportPackage_ConfigFile);
    names.push_back(cst_ReportPackage_FormatFile);
    QStringList filePaths;
    filePaths.push_back("");
    filePaths.push_back("");
    // 1、保存报表配置文件
    // 2、保存报表格式文件
    // 3、压缩
    return PMQuaZipCompress::pmCompressFiles(fileName,
                                             names,
                                             filePaths,
                                             cst_ReportPackage_Copyright);
}

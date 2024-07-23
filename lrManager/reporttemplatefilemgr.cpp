#include "reporttemplatefilemgr.h"
#include <QDir>
#include <QTemporaryDir>
#include "utils/commonutils.h"
#include "utils/pmquazipcompress.h"

// 报表模板包：配置文件
const QString cst_ReportTemplatePackage_ConfigFile = "entryConfig.xml";
// Copyright
const QString cst_ReportTemplatePackage_Copyright = "Copyright  2024 The PMSS All rights reserved ";

ReportTemplateFileMgr::ReportTemplateFileMgr()
{
    m_templateConfigMgr = new TemplateConfigMgr();
}

ReportTemplateFileMgr::~ReportTemplateFileMgr()
{
    if (m_templateConfigMgr)
        delete m_templateConfigMgr;
    PMDeleteFileOrPath(workTempDir());
}

QString ReportTemplateFileMgr::workTempDir()
{
    return QDir::cleanPath(m_workPath) + "/";
}

void ReportTemplateFileMgr::loadFromFile(const QString &fileName)
{
    QTemporaryDir tempDir(GetAppTempPath() + "/rpt-XXXXXX");
    tempDir.setAutoRemove(false);
    m_workPath = tempDir.path();
    qDebug() << m_workPath;
    // 解压至工作目录
    if (PMQuaZipCompress::pmExtractFiles(fileName, workTempDir())) {
        // 读取入口配置，加载数据至对象
        m_templateConfigMgr->loadFromFile(workTempDir() + cst_ReportTemplatePackage_ConfigFile);
    }
}

bool ReportTemplateFileMgr::saveToFile(const QString &fileName)
{
    // 入口配置对象保存为文件
    if (!m_templateConfigMgr->saveToFile(workTempDir() + cst_ReportTemplatePackage_ConfigFile))
        return false;
    // 报表id
    QList<int> reportIds;
    m_templateConfigMgr->reportsMgr()->obtainReportIds(reportIds);

    QStringList names;
    QStringList files;
    names.push_back(cst_ReportTemplatePackage_ConfigFile);
    files.push_back(workTempDir() + cst_ReportTemplatePackage_ConfigFile);
    for (int i = 0; i < reportIds.size(); i++) {
        QString name = "rpt" + QString::number(reportIds.at(i)) + ".zip";
        if (QFile::exists(workTempDir() + name)) {
            names.push_back(name);
            files.push_back(workTempDir() + name);
        }
    }
    // 和报表文件一起压缩成一个包
    return PMQuaZipCompress::pmCompressFiles(fileName, names, files);
}

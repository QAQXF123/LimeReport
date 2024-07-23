#ifndef REPORTFILEMGR_H
#define REPORTFILEMGR_H

#include "reportconfigmgr.h"
#include "reportformatmgr.h"

/*!
    @ClassName   : ReportFileMgr
    @Description : 单张报表文件(压缩包)管理：报表配置文件、报表格式文件
*/
class ReportFileMgr
{
private:
    ReportConfigMgr *m_reportConfigMgr; // 报表配置文件
    ReportFormatMgr *m_reportFormatMgr; // 报表格式文件

public:
    ReportFileMgr();
    ~ReportFileMgr();

    // 打开一张报表
    bool openReport(const QString &fileName);
    // 保存一张报表
    bool saveReport(const QString &fileName);

    // 报表配置文件
    ReportConfigMgr *reportConfigMgr() { return m_reportConfigMgr; }
    // 报表格式文件
    ReportFormatMgr *reportFormatMgr() { return m_reportFormatMgr; }
};

#endif // REPORTFILEMGR_H

#ifndef REPORTTEMPLATEFILEMGR_H
#define REPORTTEMPLATEFILEMGR_H

#include "templateconfigmgr.h"

class ReportTemplateFileMgr
{
private:
    TemplateConfigMgr *m_templateConfigMgr;
    QString m_workPath;

public:
    ReportTemplateFileMgr();
    ~ReportTemplateFileMgr();

    TemplateConfigMgr *templateConfigMgr() { return m_templateConfigMgr; }
    QString workTempDir();

    void loadFromFile(const QString &fileName);
    bool saveToFile(const QString &fileName);
};

#endif // REPORTTEMPLATEFILEMGR_H

#ifndef REPORTTEMPLATEPAGEMGR_H
#define REPORTTEMPLATEPAGEMGR_H

#include "lrTabWidget/lrTabWidget.h"

class ReportTemplatePageMgr : public lrTabWidget
{
public:
    ReportTemplatePageMgr(QWidget *parent = nullptr);
    ~ReportTemplatePageMgr();

public:
    virtual void onTabCloseRequested(int index) override;
};

#endif // REPORTTEMPLATEPAGEMGR_H

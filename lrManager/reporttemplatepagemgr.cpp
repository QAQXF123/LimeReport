#include "reporttemplatepagemgr.h"
#include <QDebug>
#include "reporttemplatepage.h"
#include "utils/commonutils.h"

ReportTemplatePageMgr::ReportTemplatePageMgr(QWidget *parent)
    : lrTabWidget(parent)
{
    //
}

ReportTemplatePageMgr::~ReportTemplatePageMgr()
{
    //
}

void ReportTemplatePageMgr::onTabCloseRequested(int index)
{
    qDebug() << "ReportTemplateMgr onTabCloseRequested";
    ReportTemplatePage *cur = dynamic_cast<ReportTemplatePage *>(getPage(index));
    if (cur == nullptr)
        return;
    if (PMQuestion("是否保存：" + cur->getFileName(), this)) {
        //
    } else {
        //
    }
    lrTabWidget::onTabCloseRequested(index);
}

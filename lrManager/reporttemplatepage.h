#ifndef REPORTTEMPLATEPAGE_H
#define REPORTTEMPLATEPAGE_H

#include <QComboBox>
#include <QTreeWidget>
#include "./../include/LimeReport"
#include "lrTabWidget/lrTabPage.h"
#include "reporttemplatefilemgr.h"

class ReportTemplatePage : public lrTabPage
{
private:
    ReportTemplateFileMgr *m_templateMgr;

    ReportTemplateFileMgr *templateMgr() { return m_templateMgr; }

private:
    QTreeWidget *m_projectIndex;    // left area
    QTreeWidget *m_projectCategory; // middle area
    QTreeWidget *m_projectReport;   // right area
    QComboBox *m_cmbCategory;
    LimeReport::ReportEngine m_report;

    // left area
    QAction *act_index_edit;
    QAction *act_index_append;
    QAction *act_index_remove;

    // middle area
    QAction *act_category_edit;
    QAction *act_categoryReport_moveUp;
    QAction *act_categoryReport_moveDown;
    QAction *act_categoryReport_add;
    QAction *act_categoryReport_remove;
    // right area

private:
    QTreeWidgetItem *createProjectItem(const QString &name, int zy);
    QTreeWidgetItem *createCategoryItem(int category, const QString &name, int id);
    QTreeWidgetItem *createReportItem(int id, const QString &name);
    void createProjectIndex();

    // 刷新分类下报表列表：根据专业ID、分类ID
    void refreshCategoryReports(int specialId, int categoryId);
    void refreshCurrentCategoryReports();

    // 刷新报表分类列表
    void refreshCategorys();

    // 刷新报表列表
    void refreshReports();
protected slots:
    // 工程索引
    void onIndexEditTriggered(bool checked = false);
    void onIndexAppendDxTriggered(bool checked = false);
    void onIndexRemoveDxTriggered(bool checked = false);

    // 报表分类
    void onCategoryEditTriggered(bool checked = false);
    void onCategoryReportMoveUpTriggered(bool checked = false);
    void onCategoryReportMoveDownTriggered(bool checked = false);
    void onCategoryReportAddTriggered(bool checked = false);
    void onCategoryReportRemoveTriggered(bool checked = false);

    // 报表列表
    void onReportEditClicked(bool checked = false);
    void onReportAddClicked(bool checked = false);
    void onReportDeleteClicked(bool checked = false);

    // 其它
    void onProjectIndexCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onReportCategoryCurrentIndexChanged(int index);

public:
    ReportTemplatePage(const QString &fileName, QWidget *parent = nullptr);
    ~ReportTemplatePage();
};

#endif // REPORTTEMPLATEPAGE_H

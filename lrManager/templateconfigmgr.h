#ifndef TEMPLATECONFIGMGR_H
#define TEMPLATECONFIGMGR_H

#include <QDebug>
#include <QDomComment>
#include <QList>

struct CategoryItem
{
    CategoryItem() {}
    int id;
    QString name;    
};

/*!
    @ClassName   : CategoryMgr
    @Description : 报表分类列表管理
*/
class CategoryMgr
{
private:
    QList<CategoryItem *> m_category;

    void clear()
    {
        for (int i = m_category.size() - 1; i >= 0; i--) {
            delete m_category.at(i);
        }
        m_category.clear();
    }

public:
    CategoryMgr() {}
    ~CategoryMgr() { clear(); }

    int getCategoryCount() { return m_category.size(); }
    CategoryItem *getCategoryByIndex(int index)
    {
        if ((index >= 0) && (index < m_category.size()))
            return m_category.at(index);
        return nullptr;
    }
    CategoryItem *getCategoryById(int id)
    {
        for (int i = 0; i < m_category.size(); i++) {
            CategoryItem *item = m_category.at(i);
            if (item->id == id)
                return item;
        }
        return nullptr;
    }
    void remove(int id)
    {
        for (int i = 0; i < m_category.size(); i++) {
            CategoryItem *item = m_category.at(i);
            if (item->id == id) {
                delete item;
                m_category.removeAt(i);
                return;
            }
        }
    }
    CategoryItem *append(int categoryId, const QString &categoryName)
    {
        CategoryItem *item = getCategoryById(categoryId);
        if (item)
            return item;
        CategoryItem *newItem = new CategoryItem();
        newItem->id = categoryId;
        newItem->name = categoryName;
        m_category.push_back(newItem);
        return newItem;
    }

    void obtainCategoryIds(QList<int> &ids)
    {
        ids.clear();
        for (int i = 0; i < m_category.size(); i++) {
            CategoryItem *item = m_category.at(i);
            ids.push_back(item->id);
        }
    }

    void loadData(QDomElement &categorysNode);
    void saveData(QDomDocument &xmlDoc, QDomElement &categorysNode);
};

struct ReportItem
{
    ReportItem() {}
    int id;
    QString name;
};

/*!
    @ClassName   : ReportFileMgr
    @Description : 模板中报表文件管理
*/
class ReportsMgr
{
private:
    QList<ReportItem *> m_reports;

    void clear()
    {
        qDebug() << "reports:" << m_reports.size();
        for (int i = m_reports.size() - 1; i >= 0; i--) {
            delete m_reports.at(i);
        }
        m_reports.clear();
    }

public:
    ReportsMgr() {}
    ~ReportsMgr() { clear(); }

    int getReportCount() { return m_reports.size(); }
    ReportItem *getReportByIndex(int index)
    {
        if ((index >= 0) && (index < m_reports.size()))
            return m_reports.at(index);
        return nullptr;
    }
    ReportItem *getReportById(int id)
    {
        for (int i = 0; i < m_reports.size(); i++) {
            ReportItem *item = m_reports.at(i);
            if (item->id == id)
                return item;
        }
        return nullptr;
    }
    void remove(int id)
    {
        for (int i = 0; i < m_reports.size(); i++) {
            ReportItem *item = m_reports.at(i);
            if (item->id == id) {
                delete item;
                m_reports.removeAt(i);
                return;
            }
        }
    }
    int getMaxId()
    {
        int iMax = 0;
        for (int i = 0; i < m_reports.size(); i++) {
            ReportItem *item = m_reports.at(i);
            if (item->id > iMax)
                iMax = item->id;
        }
        return iMax;
    }
    ReportItem *append(int reportId, const QString &name)
    {
        ReportItem *item = getReportById(reportId);
        if (item)
            return item;
        ReportItem *newItem = new ReportItem();
        newItem->id = reportId;
        newItem->name = name;
        m_reports.push_back(newItem);
        return newItem;
    }

    void obtainReportIds(QList<int> &ids)
    {
        ids.clear();
        for (int i = 0; i < m_reports.size(); i++) {
            ReportItem *item = m_reports.at(i);
            ids.push_back(item->id);
        }
    }

    void loadData(QDomElement &reportsNode);
    void saveData(QDomDocument &xmlDoc, QDomElement &reportsNode);
};

struct SpecialReportItem
{
    SpecialReportItem() {}
    int categoryId;
    int reportId;
    QString reportName;
};

class SpcialItem
{
private:
    int m_specialId;
    QString m_name;
    QList<SpecialReportItem *> m_specialReports;

    void clear()
    {
        for (int i = m_specialReports.size() - 1; i >= 0; i--) {
            delete m_specialReports.at(i);
        }
        m_specialReports.clear();
    }

public:
    SpcialItem() {}
    ~SpcialItem() { clear(); }

    QString getName() { return m_name; }
    void setName(const QString &name) { m_name = name; }
    int getSpecialId() { return m_specialId; }
    void setSpecialId(int zy) { m_specialId = zy; }

    SpecialReportItem *getSpecialReportItem(int categoryId, int reportId)
    {
        for (int i = 0; i < m_specialReports.size(); i++) {
            SpecialReportItem *item = m_specialReports.at(i);
            if ((item->categoryId == categoryId) && (item->reportId == reportId))
                return item;
        }
        return nullptr;
    }

    void remove(int categoryId, int reportId)
    {
        for (int i = 0; i < m_specialReports.size(); i++) {
            SpecialReportItem *item = m_specialReports.at(i);
            if ((item->categoryId == categoryId) && (item->reportId == reportId)) {
                delete item;
                m_specialReports.removeAt(i);
                return;
            }
        }
    }
    SpecialReportItem *append(int categoryId, int reportId, const QString &reportName)
    {
        SpecialReportItem *item = getSpecialReportItem(categoryId, reportId);
        if (item)
            return item;
        SpecialReportItem *newItem = new SpecialReportItem();
        newItem->reportId = reportId;
        newItem->categoryId = categoryId;
        newItem->reportName = reportName;
        m_specialReports.push_back(newItem);
        return newItem;
    }

    void obtainSpecialReports(int categoryId, QList<SpecialReportItem *> &resultList)
    {
        resultList.clear();
        for (int i = 0; i < m_specialReports.size(); i++) {
            SpecialReportItem *item = m_specialReports.at(i);
            if (categoryId == 0) // 等于0时显示全部
                resultList.push_back(item);
            else {
                if (item->categoryId == categoryId)
                    resultList.push_back(item);
            }
        }
    }
};

/*!
    @ClassName   : SpecialMgr
    @Description : 报表模板中专业及专业下报表管理
*/
class SpecialMgr
{
private:
    QList<SpcialItem *> m_specials;

    void clear()
    {
        qDebug() << "specials:" << m_specials.size();
        for (int i = m_specials.size() - 1; i >= 0; i--) {
            delete m_specials.at(i);
        }
        m_specials.clear();
    }

public:
    SpecialMgr() {}
    ~SpecialMgr() { clear(); }

    int getSpecialCount() { return m_specials.size(); }
    SpcialItem *getSpecialByIndex(int index)
    {
        if ((index >= 0) && (index < m_specials.size()))
            return m_specials.at(index);
        return nullptr;
    }
    SpcialItem *getSpecialById(int zy)
    {
        for (int i = 0; i < m_specials.size(); i++) {
            SpcialItem *item = m_specials.at(i);
            if (item->getSpecialId() == zy)
                return item;
        }
        return nullptr;
    }
    void remove(int zy)
    {
        for (int i = 0; i < m_specials.size(); i++) {
            SpcialItem *item = m_specials.at(i);
            if (item->getSpecialId() == zy) {
                delete item;
                m_specials.removeAt(i);
            }
        }
    }
    SpcialItem *append(int zy, const QString &name)
    {
        SpcialItem *item = getSpecialById(zy);
        if (item)
            return item;
        SpcialItem *newItem = new SpcialItem();
        newItem->setName(name);
        newItem->setSpecialId(zy);
        m_specials.push_back(newItem);
        return newItem;
    }

    void loadData(QDomElement &specialsNode);
    void saveData(QDomDocument &xmlDoc, QDomElement &specialsNode, QList<int> &categoryIds);
};

/*!
    @ClassName   : TemplateConfigMgr
    @Description : 报表模板配置管理
*/
class TemplateConfigMgr
{
private:
    double m_version = 1.0;
    QString m_company;
    QString m_author;

    CategoryMgr *m_categoryMgr = nullptr;
    ReportsMgr *m_reportsMgr = nullptr;
    SpecialMgr *m_specialMgr = nullptr;

public:
    TemplateConfigMgr();
    ~TemplateConfigMgr();

    void loadFromFile(const QString &fileName);
    bool saveToFile(const QString &fileName);

    double getVersion() { return m_version; }
    void setVersion(double ver) { m_version = ver; }
    QString getCompany() { return m_company; }
    void setCompany(const QString &company) { m_company = company; }
    QString getAuthor() { return m_author; }
    void setAuthor(const QString &author) { m_author = author; }

    CategoryMgr *categoryMgr() { return m_categoryMgr; }
    ReportsMgr *reportsMgr() { return m_reportsMgr; }
    SpecialMgr *specialMgr() { return m_specialMgr; }
};

#endif // TEMPLATECONFIGMGR_H

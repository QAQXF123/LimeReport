#ifndef REPORTCONFIGMGR_H
#define REPORTCONFIGMGR_H

#include <QDomComment>
#include <QList>
#include <QString>

// 报表参数选项
struct Options
{
    Options() {}
    bool isFullPaper = false;
    QString refBand;
    QString blankRowPos;
    bool isBlobFrame = false;
};

// 报表页面选项
struct PageSetup
{
    PageSetup() {}
    int orientation = 1;
    int pageMarginLeft = 150;
    int pageMarginTop = 130;
    int pageMarginRight = 150;
    int pageMarginBottom = 130;
};

// 报表水印参数
struct Water
{
    Water() {}
    QString text;
    int angle = 315;
    QString fontName;
    int fontSize = 24;
    QString fontColor;
    int foregroundOpacity = 20;
};

// 报表logo参数
struct Logo
{
    Logo() {}
    QString image;
    int position = 0;
    int align = 0;
};

// 报表附注参数
struct EndNote
{
    EndNote() {}
    QString text;
    int align = 0;
    QString fontName;
    int fontSize = 10;
    QString fontColor;
};

struct DataSourceItem
{
    DataSourceItem() {}
    QString tableName;
    QString variable;
};

class DataSourceMgr;

/*!
    @ClassName   : ReportConfigMgr
    @Description : 报表配置文件管理
*/
class ReportConfigMgr
{
private:
    // 报表类别
    int m_typeId;
    // 报表名称
    QString m_name;
    double m_version = 1.0;
    QString m_author;

    Options *m_options;             // 报表参数选项
    PageSetup *m_pageSetup;         // 报表页面选项
    Water *m_water;                 // 报表水印参数
    Logo *m_logo;                   // 报表logo参数
    EndNote *m_endNote;             // 报表附注参数
    DataSourceMgr *m_dataSourceMgr; // 报表数据源

public:
    ReportConfigMgr();
    ~ReportConfigMgr();

    void loadFromFile(const QString &fileName);
    bool saveToFile(const QString &fileName);

    int getTypeId() { return m_typeId; }
    void setTypeId(int typeId) { m_typeId = typeId; }
    QString getName() { return m_name; }
    void setName(const QString &name) { m_name = name; }
    double getVersion() { return m_version; }
    void setVersion(double ver) { m_version = ver; }
    QString getAuthor() { return m_author; }
    void setAuthor(const QString author) { m_author = author; }

    Options *options() { return m_options; }
    PageSetup *pageSetup() { return m_pageSetup; }
    Water *water() { return m_water; }
    Logo *logo() { return m_logo; }
    EndNote *endNote() { return m_endNote; }
    DataSourceMgr *dataSourceMgr() { return m_dataSourceMgr; }
};

class DataSourceMgr
{
private:
    QList<DataSourceItem *> m_dataSources;
    void clear();

public:
    DataSourceMgr();
    ~DataSourceMgr();

    void loadData(QDomElement &dataSourcesNode);
    void saveData(QDomDocument &xmlDoc, QDomElement &dataSourcesNode);

    int getDataSourceCount() { return m_dataSources.size(); }
    DataSourceItem *getDataSourceByIndex(int index)
    {
        if ((index >= 0) && (index < m_dataSources.size()))
            return m_dataSources.at(index);
        return nullptr;
    }

    void addDataSourceItem();
    void removeDataSourceItem();
};

#endif // REPORTCONFIGMGR_H

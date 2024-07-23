#ifndef DATASOURCECONFIGMGR_H
#define DATASOURCECONFIGMGR_H

#include <QDomComment>
#include <QList>
#include <QMap>
#include <QString>

// 节点类别：整体、单位、单项
enum NodeType { ntZT = 0, ntDW, ntDX };

// 表中列
struct ColumnItem
{
    ColumnItem() {}
    QString columnName;
    QString columnCaption;
    QString columnType;
    QString sourceColumn;
};
class TableItem;    // 单个表
class NodeTableMgr; // 节点所有表管理

/*!
    @ClassName   : DataSourceConfigMgr
    @Description : 数据源配置管理文件读取类
*/
class DataSourceConfigMgr
{
private:
    double m_version = 1.0;
    QMap<int, NodeTableMgr *> m_nodeCategory;
    void clear();
    bool loadConfigFile(const QString &fileName);

public:
    DataSourceConfigMgr();
    ~DataSourceConfigMgr();

    double getVersion() { return m_version; }
    void setVersion(double ver) { m_version = ver; }
    NodeTableMgr *getNodeTableMgr(NodeType type)
    {
        int key = static_cast<int>(type);
        if (m_nodeCategory.contains(key))
            return m_nodeCategory.value(key);
        return nullptr;
    }
};

/*!
    @ClassName   : TableItem
    @Description : 单个表
        tableType = 1：tableName 为报表中数据集名称，根据 variable 取对应数据集
        tableType = 2：tableName 为报表中数据集名称，工料汇总数据集
        tableType = 3：tableName 为报表中数据集名称，计日工拆分3个数据集，根据 variable 定位对应变量，取其所有子结点
        tableType = 4：tableName 为报表中数据集名称， 措施项目拆分2个数据集，根据 variable 定位对应费用类型，取其所有子结点
        tableType > 100：大于100均为自定义数据集类别，一个类别对应不同的数据处理
            tableType = 101：工程项目费用汇总
        
        filterType = 1：分部分项过滤
        filterType = 2：组织措施过滤
        filterType = 3：费用表过滤
        filterType = 4：工料汇总过滤
*/
class TableItem
{
private:
    QString m_tableName;
    QString m_caption;
    int m_tableType; // 表格类型标记：1...，根据不同类型的表进行相应的数据处理
    QString m_variable;
    int m_filterType;

    QList<ColumnItem *> m_columns;
    void clear()
    {
        for (int i = m_columns.size() - 1; i >= 0; i--) {
            delete m_columns.at(i);
        }
        m_columns.clear();
    }

public:
    TableItem(const QString &tableName,
              const QString &caption,
              int tableType,
              const QString &variable,
              int filterType)
        : m_tableName(tableName)
        , m_caption(caption)
        , m_tableType(tableType)
        , m_variable(variable)
        , m_filterType(filterType)
    {
        //
    }
    ~TableItem() { clear(); }

    void appendColumnItem(QDomElement &columnNode)
    {
        ColumnItem *item = new ColumnItem();
        item->columnName = columnNode.attribute("columnName");
        item->columnCaption = columnNode.attribute("columnCaption");
        item->columnType = columnNode.attribute("columnType");
        item->sourceColumn = columnNode.attribute("sourceColumn");
        m_columns.push_back(item);
    }
    QString getTableName() { return m_tableName; }
    int getTableType() { return m_tableType; }

    bool isColumnExist(const QString &columnName)
    {
        for (int i = 0; i < m_columns.size(); i++)
            if (m_columns.at(i)->columnName == columnName)
                return true;
        return false;
    }
    int getColumnIndex(const QString &columnName)
    {
        for (int i = 0; i < m_columns.size(); i++)
            if (m_columns.at(i)->columnName == columnName)
                return i;
        return -1;
    }
    int getColumnCount() { return m_columns.size(); }
    ColumnItem *getColumnByIndex(int index)
    {
        if ((index >= 0) && (index < m_columns.size()))
            return m_columns.at(index);
        return nullptr;
    }
    ColumnItem *getColumnByName(const QString &columnName)
    {
        for (int i = 0; i < m_columns.size(); i++)
            if (m_columns.at(i)->columnName == columnName)
                return m_columns.at(i);
        return nullptr;
    }
};

/*!
    @ClassName   : NodeTableMgr
    @Description : 单个节点表列表管理：整体、单位、单项下的表管理
*/
class NodeTableMgr
{
private:
    QList<TableItem *> m_tables;
    void clear();

public:
    NodeTableMgr();
    ~NodeTableMgr();

    void appendTableItem(QDomElement &itemNode);

    int getTableCount() { return m_tables.size(); }
    TableItem *getTableItemByIndex(int index)
    {
        if ((index >= 0) && (index < m_tables.size()))
            return m_tables.at(index);
        return nullptr;
    }
    TableItem *getTableItemByName(const QString &tableName)
    {
        for (int i = 0; i < m_tables.size(); i++)
            if (m_tables.at(i)->getTableName() == tableName)
                return m_tables.at(i);
        return nullptr;
    }
};

#endif // DATASOURCECONFIGMGR_H

#include "datasourceconfigmgr.h"
#include "utils/commonutils.h"
#include "utils/pmfileutils.h"

DataSourceConfigMgr::DataSourceConfigMgr()
{
    loadConfigFile(GetAppPath() + "/projectDataSource.xml");
}

DataSourceConfigMgr::~DataSourceConfigMgr()
{
    clear();
}

void DataSourceConfigMgr::clear()
{
    QList<int> keys = m_nodeCategory.keys();
    for (int i = keys.size() - 1; i >= 0; i--) {
        delete m_nodeCategory.value(keys.at(i));
        m_nodeCategory[i] = nullptr;
    }
    m_nodeCategory.clear();
}

bool DataSourceConfigMgr::loadConfigFile(const QString &fileName)
{
    clear();
    QDomDocument m_xmlDoc;
    if (!PMFileReader::readXmlFromFile(fileName, m_xmlDoc))
        return false;
    QDomElement root = m_xmlDoc.documentElement();
    if (!(root.tagName() == QStringLiteral("DataSource")))
        return false;
    setVersion(root.attribute("version").toDouble());
    QDomNodeList nodeList = root.childNodes();
    for (int i = 0; i < nodeList.size(); i++) {
        QDomElement node = nodeList.item(i).toElement();
        if (node.tagName() != QStringLiteral("nodeCategory"))
            continue;
        int type = node.attribute("nodeType").toInt();
        if (m_nodeCategory.contains(type))
            continue;

        NodeTableMgr *tableMgr = new NodeTableMgr();

        QDomNodeList itemList = node.childNodes();
        for (int j = 0; j < itemList.size(); j++) {
            QDomElement tableNode = itemList.item(j).toElement();
            // 单个数据表节点
            if (tableNode.tagName() == QStringLiteral("table"))
                tableMgr->appendTableItem(tableNode);
        }

        m_nodeCategory[type] = tableMgr;
    }
    return true;
}

NodeTableMgr::NodeTableMgr()
{
    //
}

NodeTableMgr::~NodeTableMgr()
{
    clear();
}

void NodeTableMgr::clear()
{
    for (int i = m_tables.size() - 1; i >= 0; i--) {
        delete m_tables.at(i);
    }
    m_tables.clear();
}

void NodeTableMgr::appendTableItem(QDomElement &tableNode)
{
    TableItem *table = new TableItem(tableNode.attribute("tableName"),
                                     tableNode.attribute("caption"),
                                     tableNode.attribute("tableType").toInt(),
                                     tableNode.attribute("variable"),
                                     tableNode.attribute("filterType").toInt());
    QDomNodeList childNodes = tableNode.childNodes();
    for (int i = 0; i < childNodes.size(); i++) {
        QDomElement childNode = childNodes.item(i).toElement();
        if (childNode.tagName() == "columns") {
            QDomNodeList columns = childNode.childNodes();
            for (int j = 0; j < columns.size(); j++) {
                QDomElement column = columns.item(j).toElement();
                if (column.tagName() == "column")
                    table->appendColumnItem(column);
            }
        }
    }
    m_tables.push_back(table);
}

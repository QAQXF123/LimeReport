#include "templateconfigmgr.h"
#include "utils/pmfileutils.h"

void CategoryMgr::loadData(QDomElement &categorysNode)
{
    clear();
    QDomNodeList items = categorysNode.childNodes();
    for (int i = 0; i < items.size(); i++) {
        QDomElement node = items.item(i).toElement();
        append(node.attribute("categoryId").toInt(), node.attribute("name"));
    }
}

void CategoryMgr::saveData(QDomDocument &xmlDoc, QDomElement &categorysNode)
{
    for (int i = 0; i < m_category.size(); i++) {
        auto item = m_category.at(i);
        QDomElement itemNode = xmlDoc.createElement("item");
        itemNode.setAttribute("categoryId", item->id);
        itemNode.setAttribute("name", item->name);
        categorysNode.appendChild(itemNode);
    }
}

void ReportsMgr::loadData(QDomElement &reportsNode)
{
    clear();
    QDomNodeList items = reportsNode.childNodes();
    for (int i = 0; i < items.size(); i++) {
        QDomElement node = items.item(i).toElement();
        append(node.attribute("reportId").toInt(), node.attribute("name"));
    }
}
void ReportsMgr::saveData(QDomDocument &xmlDoc, QDomElement &reportsNode)
{
    for (int i = 0; i < m_reports.size(); i++) {
        auto item = m_reports.at(i);
        QDomElement itemNode = xmlDoc.createElement("item");
        itemNode.setAttribute("reportId", item->id);
        itemNode.setAttribute("name", item->name);
        reportsNode.appendChild(itemNode);
    }
}

void SpecialMgr::loadData(QDomElement &specialsNode)
{
    clear();
    QDomNodeList items = specialsNode.childNodes();
    for (int i = 0; i < items.size(); i++) {
        QDomElement node = items.item(i).toElement();
        if (node.tagName() != "special")
            continue;
        SpcialItem *specialItem = append(node.attribute("specialId").toInt(),
                                         node.attribute("name"));
        QDomNodeList reports = node.childNodes();
        for (int j = 0; j < reports.size(); j++) {
            QDomElement report = reports.item(j).toElement();
            specialItem->append(report.attribute("reportId").toInt(),
                                report.attribute("categoryId").toInt(),
                                report.attribute("reportName"));
        }
    }
}

void SpecialMgr::saveData(QDomDocument &xmlDoc, QDomElement &specialsNode, QList<int> &categoryIds)
{
    for (int i = 0; i < m_specials.size(); i++) {
        auto specialItem = m_specials.at(i);
        QDomElement specNode = xmlDoc.createElement("special");
        specNode.setAttribute("specialId", specialItem->getSpecialId());
        specNode.setAttribute("name", specialItem->getName());
        specialsNode.appendChild(specNode);
        // 专业下报表：按分类获取
        for (int j = 0; j < categoryIds.size(); j++) {
            int categoryId = categoryIds.at(j);
            QList<SpecialReportItem *> list;
            specialItem->obtainSpecialReports(categoryId, list);
            for (int k = 0; k < list.size(); k++) {
                SpecialReportItem *reportItem = list.at(k);
                QDomElement itemNode = xmlDoc.createElement("item");
                itemNode.setAttribute("reportId", reportItem->reportId);
                itemNode.setAttribute("categoryId", reportItem->categoryId);
                itemNode.setAttribute("reportName", reportItem->reportName);
                specNode.appendChild(itemNode);
            }
        }
    }
}

TemplateConfigMgr::TemplateConfigMgr()
{
    m_categoryMgr = new CategoryMgr();
    m_reportsMgr = new ReportsMgr();
    m_specialMgr = new SpecialMgr();
}

TemplateConfigMgr::~TemplateConfigMgr()
{
    if (m_categoryMgr)
        delete m_categoryMgr;
    if (m_reportsMgr)
        delete m_reportsMgr;
    if (m_specialMgr)
        delete m_specialMgr;
}

void TemplateConfigMgr::loadFromFile(const QString &fileName)
{
    QDomDocument m_xmlDoc;
    if (!PMFileReader::readXmlFromFile(fileName, m_xmlDoc))
        return;
    QDomElement root = m_xmlDoc.documentElement();
    if (!(root.tagName() == QStringLiteral("ReportTemplate")))
        return;
    setVersion(root.attribute("version").toDouble());
    setCompany(root.attribute("company"));
    setAuthor(root.attribute("author"));
    QDomNodeList nodeList = root.childNodes();
    for (int i = 0; i < nodeList.size(); i++) {
        QDomElement node = nodeList.item(i).toElement();
        if (node.tagName() == "categorys") {
            categoryMgr()->loadData(node);
        } else if (node.tagName() == "reports") {
            reportsMgr()->loadData(node);
        } else if (node.tagName() == "specials") {
            specialMgr()->loadData(node);
        }
    }
}

bool TemplateConfigMgr::saveToFile(const QString &fileName)
{
    QDomDocument xmlDoc;
    QDomElement root = xmlDoc.createElement("ReportTemplate");
    root.setAttribute("version", getVersion());
    root.setAttribute("company", getCompany());
    root.setAttribute("author", getAuthor());
    xmlDoc.appendChild(root);

    QDomElement categorysNode = xmlDoc.createElement("categorys");
    categoryMgr()->saveData(xmlDoc, categorysNode);
    root.appendChild(categorysNode);

    QDomElement reportsNode = xmlDoc.createElement("reports");
    reportsMgr()->saveData(xmlDoc, reportsNode);
    root.appendChild(reportsNode);

    QList<int> categoryIds;
    categoryMgr()->obtainCategoryIds(categoryIds);
    QDomElement specialsNode = xmlDoc.createElement("specials");
    specialMgr()->saveData(xmlDoc, specialsNode, categoryIds);
    root.appendChild(specialsNode);

    return PMFileWriter::saveXmlToFile(xmlDoc, fileName);
}

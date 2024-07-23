#include "reportconfigmgr.h"
#include "utils/pmfileutils.h"

ReportConfigMgr::ReportConfigMgr()
{
    // 文件结构：
    // 报表类型Id、报表名称
    // options：报表选项
    // pageSetup：页面设置
    // water：水印
    // logo：表头logo
    // endNote：报尾附注
    // dataSources：报表数据源
    // cells：单元格参数
    // bands：band参数
    // scripts: 报表脚本
    m_options = new Options();             // 报表参数选项
    m_pageSetup = new PageSetup();         // 报表页面选项
    m_water = new Water();                 // 报表水印参数
    m_logo = new Logo();                   // 报表logo参数
    m_endNote = new EndNote();             // 报表附注参数
    m_dataSourceMgr = new DataSourceMgr(); // 报表数据源
}

ReportConfigMgr::~ReportConfigMgr()
{
    if (m_dataSourceMgr)
        delete m_dataSourceMgr;
    if (m_endNote)
        delete m_endNote;
    if (m_logo)
        delete m_logo;
    if (m_water)
        delete m_water;
    if (m_pageSetup)
        delete m_pageSetup;
    if (m_options)
        delete m_options;
}

void ReportConfigMgr::loadFromFile(const QString &fileName)
{
    QDomDocument m_xmlDoc;
    if (!PMFileReader::readXmlFromFile(fileName, m_xmlDoc))
        return;
    QDomElement root = m_xmlDoc.documentElement();
    if (!(root.tagName() == QStringLiteral("Report")))
        return;
    setTypeId(root.attribute("typeId").toInt());
    setName(root.attribute("name"));
    setVersion(root.attribute("version").toDouble());
    setAuthor(root.attribute("author"));
    QDomNodeList nodeList = root.childNodes();
    for (int i = 0; i < nodeList.size(); i++) {
        QDomElement node = nodeList.item(i).toElement();
        if (node.tagName() == "options") {
            options()->isFullPaper = node.attribute("isFullPaper") == "true";
            options()->refBand = node.attribute("refBand");
            options()->blankRowPos = node.attribute("blankRowPos");
            options()->isBlobFrame = node.attribute("isBlobFrame") == "true";
        } else if (node.tagName() == "pageSetup") {
            pageSetup()->orientation = node.attribute("orientation").toInt();
            pageSetup()->pageMarginLeft = node.attribute("pageMarginLeft").toInt();
            pageSetup()->pageMarginTop = node.attribute("pageMarginTop").toInt();
            pageSetup()->pageMarginRight = node.attribute("pageMarginRight").toInt();
            pageSetup()->pageMarginBottom = node.attribute("pageMarginBottom").toInt();
        } else if (node.tagName() == "water") {
            water()->text = node.attribute("text");
            water()->angle = node.attribute("angle").toInt();
            water()->fontName = node.attribute("fontName");
            water()->fontSize = node.attribute("fontSize").toInt();
            water()->fontColor = node.attribute("fontColor");
            water()->foregroundOpacity = node.attribute("foregroundOpacity").toInt();
        } else if (node.tagName() == "logo") {
            logo()->image = node.attribute("image");
            logo()->position = node.attribute("position").toInt();
            logo()->align = node.attribute("align").toInt();
        } else if (node.tagName() == "endNote") {
            endNote()->text = node.attribute("text");
            endNote()->align = node.attribute("align").toInt();
            endNote()->fontName = node.attribute("fontName");
            endNote()->fontSize = node.attribute("fontSize").toInt();
            endNote()->fontColor = node.attribute("fontColor");
        } else if (node.tagName() == "dataSources") {
            dataSourceMgr()->loadData(node);
        } else if (node.tagName() == "bands") {
            //
        } else if (node.tagName() == "cells") {
            //
        } else if (node.tagName() == "scripts") {
            //
        }
    }
}

bool ReportConfigMgr::saveToFile(const QString &fileName)
{
    QDomDocument xmlDoc;
    QDomElement root = xmlDoc.createElement("Report");
    root.setAttribute("typeId", getTypeId());
    root.setAttribute("name", getName());
    root.setAttribute("version", getVersion());
    root.setAttribute("author", getAuthor());
    xmlDoc.appendChild(root);

    QDomElement optionsNode = xmlDoc.createElement("options");
    optionsNode.setAttribute("isFullPaper", options()->isFullPaper ? "true" : "false");
    optionsNode.setAttribute("refBand", options()->refBand);
    optionsNode.setAttribute("blankRowPos", options()->blankRowPos);
    optionsNode.setAttribute("isBlobFrame", options()->isBlobFrame ? "true" : "false");
    root.appendChild(optionsNode);

    QDomElement pageSetupNode = xmlDoc.createElement("pageSetup");
    pageSetupNode.setAttribute("orientation", pageSetup()->orientation);
    pageSetupNode.setAttribute("pageMarginLeft", pageSetup()->pageMarginLeft);
    pageSetupNode.setAttribute("pageMarginTop", pageSetup()->pageMarginTop);
    pageSetupNode.setAttribute("pageMarginRight", pageSetup()->pageMarginRight);
    pageSetupNode.setAttribute("pageMarginBottom", pageSetup()->pageMarginBottom);
    root.appendChild(pageSetupNode);

    QDomElement waterNode = xmlDoc.createElement("water");
    waterNode.setAttribute("text", water()->text);
    waterNode.setAttribute("angle", water()->angle);
    waterNode.setAttribute("fontName", water()->fontName);
    waterNode.setAttribute("fontSize", water()->fontSize);
    waterNode.setAttribute("fontColor", water()->fontColor);
    waterNode.setAttribute("foregroundOpacity", water()->foregroundOpacity);
    root.appendChild(waterNode);

    QDomElement logoNode = xmlDoc.createElement("logo");
    logoNode.setAttribute("image", logo()->image);
    logoNode.setAttribute("position", logo()->position);
    logoNode.setAttribute("align", logo()->align);
    root.appendChild(logoNode);

    QDomElement endNoteNode = xmlDoc.createElement("endNote");
    endNoteNode.setAttribute("text", endNote()->text);
    endNoteNode.setAttribute("align", endNote()->align);
    endNoteNode.setAttribute("fontName", endNote()->fontName);
    endNoteNode.setAttribute("fontSize", endNote()->fontSize);
    endNoteNode.setAttribute("fontColor", endNote()->fontColor);
    root.appendChild(endNoteNode);

    QDomElement dataSourcesNode = xmlDoc.createElement("dataSources");
    dataSourceMgr()->saveData(xmlDoc, dataSourcesNode);
    root.appendChild(dataSourcesNode);

    //bands
    QDomElement bandsNode = xmlDoc.createElement("bands");
    // 后续补充...
    root.appendChild(bandsNode);

    //cells
    QDomElement cellsNode = xmlDoc.createElement("cells");
    // 后续补充...
    root.appendChild(cellsNode);

    //scripts
    QDomElement scriptsNode = xmlDoc.createElement("scripts");
    // 后续补充...
    root.appendChild(scriptsNode);

    return PMFileWriter::saveXmlToFile(xmlDoc, fileName);
}

DataSourceMgr::DataSourceMgr()
{
    //
}

DataSourceMgr::~DataSourceMgr()
{
    clear();
}

void DataSourceMgr::clear()
{
    for (int i = m_dataSources.size() - 1; i >= 0; i--)
        delete m_dataSources.at(i);
    m_dataSources.clear();
}

void DataSourceMgr::loadData(QDomElement &dataSourcesNode)
{
    QDomNodeList childs = dataSourcesNode.childNodes();
    for (int i = 0; i < childs.size(); i++) {
        QDomElement itemNode = childs.item(i).toElement();
        //
    }
}

void DataSourceMgr::saveData(QDomDocument &xmlDoc, QDomElement &dataSourcesNode)
{
    QDomElement itemNode = xmlDoc.createElement("item");
    // 后续补充...
    dataSourcesNode.appendChild(itemNode);
}

void DataSourceMgr::addDataSourceItem()
{
    //
}

void DataSourceMgr::removeDataSourceItem()
{
    //
}

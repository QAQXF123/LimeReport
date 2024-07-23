#include "reporttemplatepage.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>

ReportTemplatePage::ReportTemplatePage(const QString &fileName, QWidget *parent)
    : lrTabPage(fileName, parent)
{
    m_templateMgr = new ReportTemplateFileMgr();
    m_templateMgr->loadFromFile(fileName);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    // 水平分裂器
    QSplitter *sp = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(sp);

    // 最左侧区域
    QWidget *indexWidget = new QWidget(this);
    sp->addWidget(indexWidget);

    QVBoxLayout *indexLayout = new QVBoxLayout();
    indexLayout->setMargin(0);
    indexLayout->setSpacing(0);
    indexLayout->setContentsMargins(0, 0, 0, 0);

    indexWidget->setLayout(indexLayout);

    QToolBar *indexTool = new QToolBar(this);
    indexLayout->addWidget(indexTool, 0, Qt::AlignTop);

    act_index_edit = indexTool->addAction("编辑");
    connect(act_index_edit, &QAction::triggered, this, &ReportTemplatePage::onIndexEditTriggered);

    indexTool->addSeparator();
    act_index_append = indexTool->addAction("添加单项");
    connect(act_index_append,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onIndexAppendDxTriggered);
    act_index_remove = indexTool->addAction("删除单项");
    connect(act_index_remove,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onIndexRemoveDxTriggered);

    m_projectIndex = new QTreeWidget(this);
    indexLayout->addWidget(m_projectIndex);

    m_projectIndex->setColumnCount(2);
    m_projectIndex->setHeaderLabels({"ID", "节点名称"});
    m_projectIndex->setColumnWidth(0, 100);
    m_projectIndex->header()->setDefaultAlignment(Qt::AlignCenter);
    m_projectIndex->header()->setHighlightSections(true); // 设置选中高亮
    m_projectIndex->setAlternatingRowColors(true);        // 设置隔行颜色间隔
    createProjectIndex();

    // 中间区域
    QWidget *categoryWidget = new QWidget(this);
    sp->addWidget(categoryWidget);

    QVBoxLayout *catLayout = new QVBoxLayout();
    catLayout->setMargin(0);
    catLayout->setSpacing(0);
    catLayout->setContentsMargins(0, 0, 0, 0);
    categoryWidget->setLayout(catLayout);

    QToolBar *catTool = new QToolBar(this);
    catLayout->addWidget(catTool, 0, Qt::AlignTop);

    QLabel *lbl = new QLabel("按类别过滤：", this);
    catTool->addWidget(lbl);

    m_cmbCategory = new QComboBox(this);
    m_cmbCategory->setEditable(false);
    refreshCategorys();
    catTool->addWidget(m_cmbCategory);

    act_category_edit = catTool->addAction("编辑类别");
    connect(act_category_edit,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onCategoryEditTriggered);
    catTool->addSeparator();
    act_categoryReport_moveUp = catTool->addAction("上移");
    connect(act_categoryReport_moveUp,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onCategoryReportMoveUpTriggered);
    act_categoryReport_moveDown = catTool->addAction("下移");
    connect(act_categoryReport_moveDown,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onCategoryReportMoveDownTriggered);
    catTool->addSeparator();
    act_categoryReport_add = catTool->addAction("添加报表");
    connect(act_categoryReport_add,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onCategoryReportAddTriggered);
    act_categoryReport_remove = catTool->addAction("移除报表");
    connect(act_categoryReport_remove,
            &QAction::triggered,
            this,
            &ReportTemplatePage::onCategoryReportRemoveTriggered);

    m_projectCategory = new QTreeWidget(this);
    catLayout->addWidget(m_projectCategory);

    m_projectCategory->setColumnCount(3);
    m_projectCategory->setColumnWidth(0, 100);
    m_projectCategory->setColumnWidth(1, 100);
    m_projectCategory->setHeaderLabels({"报表类别", "报表ID", "节点报表显示名称"});
    m_projectCategory->header()->setDefaultAlignment(Qt::AlignCenter);
    m_projectCategory->header()->setHighlightSections(true); // 设置选中高亮
    m_projectCategory->setAlternatingRowColors(true);        // 设置隔行颜色间隔
    m_projectCategory->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_projectCategory->addTopLevelItem(createCategoryItem(1, "报表1", 1));
    m_projectCategory->addTopLevelItem(createCategoryItem(1, "报表2", 2));
    m_projectCategory->addTopLevelItem(createCategoryItem(1, "报表3", 3));

    // 最右侧区域
    QWidget *reportWidget = new QWidget(this);
    sp->addWidget(reportWidget);

    QVBoxLayout *reportLayout = new QVBoxLayout();
    reportLayout->setMargin(0);
    reportLayout->setSpacing(0);
    reportLayout->setContentsMargins(0, 0, 0, 0);
    reportWidget->setLayout(reportLayout);

    QToolBar *reportTool = new QToolBar(this);
    reportLayout->addWidget(reportTool, 0, Qt::AlignTop);

    QPushButton *editBtn = new QPushButton(this);
    editBtn->setText("编辑报表");
    reportTool->addWidget(editBtn);
    connect(editBtn, &QPushButton::clicked, this, &ReportTemplatePage::onReportEditClicked);
    reportTool->addSeparator();

    QPushButton *addBtn = new QPushButton(this);
    addBtn->setText("增加报表");
    reportTool->addWidget(addBtn);
    connect(addBtn, &QPushButton::clicked, this, &ReportTemplatePage::onReportAddClicked);

    QPushButton *delBtn = new QPushButton(this);
    delBtn->setText("删除报表");
    reportTool->addWidget(delBtn);
    connect(delBtn, &QPushButton::clicked, this, &ReportTemplatePage::onReportDeleteClicked);

    m_projectReport = new QTreeWidget(this);
    reportLayout->addWidget(m_projectReport);

    m_projectReport->setColumnCount(2);
    m_projectReport->setColumnWidth(0, 100);
    m_projectReport->setHeaderLabels({"报表ID", "报表描述"});
    m_projectReport->header()->setDefaultAlignment(Qt::AlignCenter);
    m_projectReport->header()->setHighlightSections(true); // 设置选中高亮
    m_projectReport->setAlternatingRowColors(true);        // 设置隔行颜色间隔
    m_projectReport->setSelectionMode(QAbstractItemView::ExtendedSelection);
    refreshReports();

    QList<int> sizes;
    sizes << 3000 << 6000 << 6000;
    sp->setSizes(sizes);

    connect(m_projectIndex,
            &QTreeWidget::currentItemChanged,
            this,
            &ReportTemplatePage::onProjectIndexCurrentItemChanged);
    connect(m_cmbCategory,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &ReportTemplatePage::onReportCategoryCurrentIndexChanged);
    refreshCurrentCategoryReports();
}

void ReportTemplatePage::onIndexEditTriggered(bool checked)
{
    //
}

void ReportTemplatePage::onIndexAppendDxTriggered(bool checked)
{
    //
}

void ReportTemplatePage::onIndexRemoveDxTriggered(bool checked)
{
    //
}

void ReportTemplatePage::onCategoryEditTriggered(bool checked)
{
    //
}
void ReportTemplatePage::onCategoryReportMoveUpTriggered(bool checked)
{
    //
}
void ReportTemplatePage::onCategoryReportMoveDownTriggered(bool checked)
{
    //
}
void ReportTemplatePage::onCategoryReportAddTriggered(bool checked)
{
    //
}
void ReportTemplatePage::onCategoryReportRemoveTriggered(bool checked)
{
    //
}

void ReportTemplatePage::onReportEditClicked(bool checked)
{
    // m_report.dataManager()->setReportVariable("xmxx.p_mc", "品品");
    // qDebug() << "clicked";
    // m_report.designReport();
}

void ReportTemplatePage::onReportAddClicked(bool checked)
{
    //
}
void ReportTemplatePage::onReportDeleteClicked(bool checked)
{
    //
}

QTreeWidgetItem *ReportTemplatePage::createProjectItem(const QString &name, int zy)
{
    QStringList list;
    list.push_back(QString::number(zy));
    list.push_back(name);
    return new QTreeWidgetItem(list);
}

QTreeWidgetItem *ReportTemplatePage::createCategoryItem(int category, const QString &name, int id)
{
    QStringList list;
    list.push_back(QString::number(category));
    list.push_back(QString::number(id));
    list.push_back(name);
    return new QTreeWidgetItem(list);
}

QTreeWidgetItem *ReportTemplatePage::createReportItem(int id, const QString &name)
{
    QStringList list;
    list.push_back(QString::number(id));
    list.push_back(name);
    return new QTreeWidgetItem(list);
}

void ReportTemplatePage::createProjectIndex()
{
    m_projectIndex->clear();
    SpcialItem *zItem = templateMgr()->templateConfigMgr()->specialMgr()->getSpecialById(99);
    QString name = "整体节点";
    if (zItem)
        name = zItem->getName();
    // 整体
    QTreeWidgetItem *rootItem = createProjectItem(name, 99);
    m_projectIndex->addTopLevelItem(rootItem);
    // 单项
    SpcialItem *dItem = templateMgr()->templateConfigMgr()->specialMgr()->getSpecialById(100);
    name = "单项节点";
    if (dItem)
        name = dItem->getName();
    QTreeWidgetItem *dxItem = createProjectItem(name, 100);
    rootItem->addChild(dxItem);
    for (int i = 0; i < templateMgr()->templateConfigMgr()->specialMgr()->getSpecialCount(); i++) {
        SpcialItem *item = templateMgr()->templateConfigMgr()->specialMgr()->getSpecialByIndex(i);
        if (item && (item->getSpecialId() != 99) && (item->getSpecialId() != 100)) {
            // 单位
            dxItem->addChild(createProjectItem(item->getName(), item->getSpecialId()));
        }
    }
    m_projectIndex->setCurrentItem(rootItem);
    m_projectIndex->expandAll();
}

void ReportTemplatePage::refreshCategoryReports(int specialId, int categoryId)
{
    m_projectCategory->clear();
    SpcialItem *nodeItem = templateMgr()->templateConfigMgr()->specialMgr()->getSpecialById(
        specialId);
    if (nodeItem == nullptr)
        return;
    QList<SpecialReportItem *> list;
    nodeItem->obtainSpecialReports(categoryId, list);
    for (int i = 0; i < list.size(); i++) {
        SpecialReportItem *item = list.at(i);
        m_projectCategory->addTopLevelItem(
            createCategoryItem(item->categoryId, item->reportName, item->reportId));
    }
}

void ReportTemplatePage::refreshCategorys()
{
    m_cmbCategory->clear();
    m_cmbCategory->addItem("0显示全部");
    for (int i = 0; i < templateMgr()->templateConfigMgr()->categoryMgr()->getCategoryCount(); i++) {
        CategoryItem *item = templateMgr()->templateConfigMgr()->categoryMgr()->getCategoryByIndex(
            i);
        if (item) {
            m_cmbCategory->addItem(QString::number(item->id) + item->name);
        }
    }
    m_cmbCategory->setCurrentIndex(0);
}

void ReportTemplatePage::refreshReports()
{
    m_projectReport->clear();
    for (int i = 0; i < templateMgr()->templateConfigMgr()->reportsMgr()->getReportCount(); i++) {
        ReportItem *item = templateMgr()->templateConfigMgr()->reportsMgr()->getReportByIndex(i);
        if (item) {
            m_projectReport->addTopLevelItem(createReportItem(item->id, item->name));
        }
    }
}

void ReportTemplatePage::refreshCurrentCategoryReports()
{
    int index = m_cmbCategory->currentIndex();
    if (index < 0)
        return;
    int categoryId = 0;
    QString text = m_cmbCategory->itemText(index);
    QRegExp regex("\\d+"); // 匹配一个或多个数字
    int pos = 0;
    while ((pos = regex.indexIn(text, pos)) != -1) {
        QString number = regex.cap(0); // 获取匹配到的数字
        // 处理数字
        categoryId = number.toInt();
        break;
        pos += regex.matchedLength();
    }
    QTreeWidgetItem *item = m_projectIndex->currentItem();
    if (item == nullptr)
        return;
    int specialId = item->data(0, Qt::DisplayRole).toInt();
    refreshCategoryReports(specialId, categoryId);
}

void ReportTemplatePage::onProjectIndexCurrentItemChanged(QTreeWidgetItem *current,
                                                          QTreeWidgetItem *previous)
{
    if (current == nullptr)
        return;
    refreshCurrentCategoryReports();
}

void ReportTemplatePage::onReportCategoryCurrentIndexChanged(int index)
{
    if (index < 0)
        return;
    refreshCurrentCategoryReports();
}

ReportTemplatePage::~ReportTemplatePage()
{
    if (m_templateMgr)
        delete m_templateMgr;
}

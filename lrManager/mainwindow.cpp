#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>
#include "reporttemplatepage.h"
#include "ui_mainwindow.h"
#include "utils/commonutils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("报表模板管理");

    m_reportTemplateMgr = new ReportTemplatePageMgr(this);

    // lrTabPage *homepage = new ReportTemplatePage("首页", this);
    // m_reportTemplateMgr->appendFixedPage(homepage, "首页");

    // lrTabPage *page1 = new ReportTemplatePage("F:/QtDemo/LimeReport/lrManager/mainwindow.cpp", this);
    // if (m_reportTemplateMgr->appendNormalPage(page1, "F:/QtDemo/LimeReport/lrManager/mainwindow.cpp")
    //     == -1)
    //     delete page1;

    // lrTabPage *page2 = new ReportTemplatePage("F:/QtDemo/LimeReport/lrManager/mainwindow.h", this);
    // if (m_reportTemplateMgr->appendNormalPage(page2, "F:/QtDemo/LimeReport/lrManager/mainwindow.h")
    //     == -1)
    //     delete page2;
    // 在左上角添加按钮
    // QPushButton *button1 = new QPushButton("topL", tabMgr);
    // tabMgr->setCornerWidget(button1, Qt::TopLeftCorner);
    // // 在右上角添加按钮
    // QPushButton *button = new QPushButton("topR", tabMgr);
    // tabMgr->setCornerWidget(button, Qt::TopRightCorner);

    setCentralWidget(m_reportTemplateMgr);
}

MainWindow::~MainWindow()
{
    if (m_reportTemplateMgr)
        delete m_reportTemplateMgr;
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    //
}

void MainWindow::on_actionOpen_triggered()
{
    // text(*.txt *.xlsx);;excel(*.xlsx)
    QString file = QFileDialog::getOpenFileName(this,
                                                "选择一个报表模板文件",
                                                "",
                                                "报表模板文件(*.pmrp)");
    if (file.isEmpty())
        return;
    if (m_reportTemplateMgr->isExist(file)) {
        PMInfomation("文件已经打开！", this);
        m_reportTemplateMgr->activePage(file);
        return;
    }
    lrTabPage *page1 = new ReportTemplatePage(file, this);
    m_reportTemplateMgr->appendNormalPage(page1, file);
}

void MainWindow::on_actionSave_triggered()
{
    ReportTemplatePage *page = dynamic_cast<ReportTemplatePage *>(
        m_reportTemplateMgr->getCurrentPage());
    if (page == nullptr)
        return;
    if (page->save()) {
        PMInfomation("保存成功！", this);
        qDebug() << page->getFileName();
    }
}

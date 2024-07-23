#include "lrTabWidget.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDrag>
#include <QFileInfo>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include "lrTabBar.h"
#include "lrTabPage.h"
#include "lrTabPopup.h"

lrTabWidget::lrTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);

    initTabBar();
}

int lrTabWidget::appendNormalPage(QWidget *page, const QString &fileName)
{
    if (page == nullptr)
        return -1;
    if (isExist(fileName))
        return -1;
    if (!QFileInfo::exists(fileName))
        return -1;
    //设置为调用close时释放
    page->setAttribute(Qt::WA_DeleteOnClose);
    //最后是添加到stackedWidget去了
    QFileInfo file(fileName);
    const int index = addTab(page, file.completeBaseName() + '.' + file.suffix());
    setTabToolTip(index, fileName);

    //切换为当前新增页
    setCurrentIndex(index);
    return index;
}

void lrTabWidget::removeNormalPage(QWidget *page)
{
    if(!page)
        return;
    removeNormalIndex(indexOf(page));
}

void lrTabWidget::takeNormalPage(QWidget *page)
{
    if(!page)
        return;
    removeTab(indexOf(page));
}

void lrTabWidget::removeNormalIndex(int index)
{
    if(indexValid(index))
    {
        QWidget *page=this->widget(index);
        //判断是否为固定不删除的
        if(!page||fixedPage.contains(page))
            return;
        //removeTab只是从tabbar移除了，并没有释放
        removeTab(index);
        //可以自己调用delete，或者设置tab页为WA_DeleteOnClose,关闭时释放
        //delete page;
        page->close();
    }
}

QWidget *lrTabWidget::getPage(int index)
{
    if (indexValid(index)) {
        return this->widget(index);
    }
    return nullptr;
}

void lrTabWidget::removeCurrentPage()
{
    removeNormalIndex(currentIndex());
}

void lrTabWidget::onTabCloseRequested(int index)
{
    removeNormalIndex(index);
}

void lrTabWidget::clearNormalPage()
{
    const int tab_count = this->count();
    //从后往前删，这样index位置就是固定的
    for (int index = tab_count - 1; index >= 0; index--)
    {
        removeNormalIndex(index);
    }
}

void lrTabWidget::clearAllPage()
{
    //全部移除
    while(this->count()>0)
    {
        QWidget *page=this->widget(0);
        removeTab(0);
        if(page)
            page->close();
    }
    fixedPage.clear();
}

QWidget *lrTabWidget::getCurrentPage()
{
    if (indexValid(currentIndex()))
        return this->widget(currentIndex());
    else
        return nullptr;
}

void lrTabWidget::appendFixedPage(QWidget *page, const QString &fileName)
{
    if(!page)
        return;
    //设置为调用close时释放
    page->setAttribute(Qt::WA_DeleteOnClose);
    //最后是添加到stackedWidget去了
    const int index = addTab(page, fileName);
    setTabToolTip(index, fileName);
    //切换为当前新增页
    setCurrentIndex(index);
    setFixedPage(page);
}

void lrTabWidget::setFixedPage(QWidget *page)
{
    if(!page)
        return;
    setFixedIndex(indexOf(page));
}

void lrTabWidget::setFixedIndex(int index)
{
    if(indexValid(index))
    {
        QWidget *page=this->widget(index);
        if(page&&!fixedPage.contains(page))
        {
            fixedPage.push_back(page);
            //不显示关闭按钮，替换为nullptr
            tabBar()->setTabButton(index, QTabBar::RightSide, nullptr);
        }
    }
}

bool lrTabWidget::indexValid(int index) const
{
    if(index<0||index>=this->count())
        return false;
    return true;
}

bool lrTabWidget::isExist(const QString &fileName)
{
    const int tab_count = this->count();
    for (int index = tab_count - 1; index >= 0; index--) {
        lrTabPage *page = dynamic_cast<lrTabPage *>(this->widget(index));
        if (page->getFileName() == fileName)
            return true;
    }
    return false;
}

QWidget *lrTabWidget::getPage(const QString &fileName)
{
    const int tab_count = this->count();
    for (int index = tab_count - 1; index >= 0; index--) {
        lrTabPage *page = dynamic_cast<lrTabPage *>(this->widget(index));
        if (page->getFileName() == fileName)
            return page;
    }
    return nullptr;
}

void lrTabWidget::activePage(const QString &fileName)
{
    const int tab_count = this->count();
    for (int index = tab_count - 1; index >= 0; index--) {
        lrTabPage *page = dynamic_cast<lrTabPage *>(this->widget(index));
        if (page->getFileName() == fileName) {
            if (this->currentIndex() != index)
                this->setCurrentIndex(index);
        }
    }
}

bool lrTabWidget::pageValid(QWidget *page) const
{
    if(!page)
        return false;
    return indexValid(indexOf(page));
}

void lrTabWidget::showEvent(QShowEvent *event)
{
    QTabWidget::showEvent(event);
    //初始化时把已有的设置为close释放
    for (int index = 0; index < this->count(); index++) {
        QWidget *page=this->widget(index);
        if(page)
            page->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void lrTabWidget::initTabBar()
{
    lrTabBar *bar = new lrTabBar(this);
    //setTabBar是protected成员函数，要使用就得继承
    setTabBar(bar);
    //点击页签上的关闭按钮时，触发信号
    connect(bar, &QTabBar::tabCloseRequested, this, &lrTabWidget::onTabCloseRequested);
    //拖拽到外部-还未释放鼠标
    connect(bar, &lrTabBar::beginDragOut, this, [this, bar](int index) {
        if(!indexValid(index))
            return;
        QWidget *drag_tab=this->widget(index);
        //固定tab就不让拖出
        if(!drag_tab||fixedPage.contains(drag_tab))
            return;
        //把当前页作为快照拖拽
        //尺寸加了标题栏和边框
        QPixmap pixmap(drag_tab->size()+QSize(2,31));
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        if(painter.isActive())
        {
            //这里想做标题栏贴在内容之上
            //但是没法获取默认标题栏的图像啊，就随便画一个矩形框
            //如果设置了外部主题颜色，需要改下
            QRect title_rect{0,0,pixmap.width(),30};
            painter.fillRect(title_rect,Qt::white);
            painter.drawText(title_rect,Qt::AlignLeft|Qt::AlignVCenter,"  "+drag_tab->windowTitle());
            painter.drawRect(pixmap.rect().adjusted(0,0,-1,-1));
        }
        painter.end();
        drag_tab->render(&pixmap,QPoint(1,30));

        QMimeData *mime=new QMimeData;
        QDrag *drag=new QDrag(bar);
        drag->setMimeData(mime);
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(10,0));

        //鼠标弹起后drag就释放了，这时候去判断是否拖拽到了外部
        connect(drag,&QDrag::destroyed,this,[=]{
            QPoint bar_point=bar->mapFromGlobal(QCursor::pos());
            //不在范围，拖出
            if(!bar->contentsRect().contains(bar_point)){
                popPage(drag_tab);
            }
        });

        drag->exec(Qt::MoveAction);
    });
}

void lrTabWidget::popPage(QWidget *page)
{
    takeNormalPage(page);
    //这里套一个自定义标题栏的窗口给page
    lrTabPopup *pop = new lrTabPopup(this);
    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setContentWidget(page);
    pop->setWindowTitle(page->windowTitle());
    pop->resize(page->size());
    //拖出来的位置有点偏移
    pop->move(QCursor::pos() - QPoint(10, 10));

    //判断独立窗口是否拖回tab
    connect(pop, &lrTabPopup::dragRelease, this, [=](const QPoint &pos) {
        const QPoint bar_pos=tabBar()->mapFromGlobal(pos);
        //如果又拖回了tabbar范围内，就把widget取出来放回tab
        if(tabBar()->contentsRect().contains(bar_pos))
        {
            QWidget *content=pop->getContentWidget();
            this->appendNormalPage(content, "");
            pop->disconnect();
            //关闭的时候会在原来的位置闪一下？
            pop->close();
            //this->activateWindow();
        }
    });
    pop->show();
    page->show();
    pop->activateWindow();
    pop->setFocus();
}


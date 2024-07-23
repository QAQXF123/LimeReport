#ifndef LRTABWIDGET_H
#define LRTABWIDGET_H

#include <QTabWidget>
#include "lrTabPage.h"

using CreatorPageCallback = std::function<lrTabPage *(const QString &)>;

/**
 * @brief 自定义TabWidget
 * @details setTabBar为protected接口，要修改tabbar就得继承下
 */
class lrTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit lrTabWidget(QWidget *parent = nullptr);

    //添加一个页
    int appendNormalPage(QWidget *page, const QString &fileName);
    //移除一个页
    void removeNormalPage(QWidget *page);
    //从tab移出某页，但不close
    void takeNormalPage(QWidget *page);
    //移除一个页
    void removeNormalIndex(int index);
    //移除当前页
    void removeCurrentPage();
    //清除所有页（不包含fixed页）
    void clearNormalPage();
    //清除所有页（包含fixed页）
    void clearAllPage();
    //添加一个不能关闭且不能拖出的页
    //appendNormalPage+setFixedPage
    void appendFixedPage(QWidget *page, const QString &fileName);
    //设置某页不能关闭且不能拖出
    void setFixedPage(QWidget *page);
    //设置某页不能关闭且不能拖出
    void setFixedIndex(int index);

    //检查index是否有效
    bool indexValid(int index) const;
    //检查widget是否为有效页
    bool pageValid(QWidget *page) const;
    // 是否存在
    bool isExist(const QString &fileName);
    // 根据文件获取
    QWidget *getPage(const QString &fileName);
    // 根据文件激活
    void activePage(const QString &fileName);
    // 根据索引获取
    QWidget *getPage(int index);
    // 获取当前激活
    QWidget *getCurrentPage();

protected:
    void showEvent(QShowEvent *event);

public:
    virtual void onTabCloseRequested(int index);

private:
    //初始化tab栏
    void initTabBar();
    //弹出一个页面
    void popPage(QWidget *page);

private:
    //保存不能拖出、没有关闭按钮的tab页（如主页）
    QList<QWidget*> fixedPage;
};

#endif // LRTABWIDGET_H

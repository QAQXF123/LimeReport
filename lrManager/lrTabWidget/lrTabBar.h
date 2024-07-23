#ifndef LRTABBAR_H
#define LRTABBAR_H

#include <QTabBar>

/**
 * @brief 自定义TabBar
 * @details 以支持Tab页的拖入拖出
 * @note bar.rect的范围是有效范围而不是tabWidget的宽度
 */
class lrTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit lrTabBar(QWidget *parent = nullptr);

protected:
    void mousePressEvent (QMouseEvent *event) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;

signals:
    //拖出时发出信号
    void beginDragOut(int index);

private:
    //鼠标按下
    bool theDragPress=false;
    //拖拽到了外部
    bool theDragOut=false;
    //按下时位置
    //QPoint pressPos;
};

#endif // LRTABBAR_H

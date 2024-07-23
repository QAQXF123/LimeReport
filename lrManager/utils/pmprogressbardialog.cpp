#include "pmprogressbardialog.h"
#include <QKeyEvent>
#include <QProgressBar>
#include <QPushButton>

/*
callback demo:
PMProgressBarDialog progress;
//progress.setShowCancelButton(true); // 显示取消按钮,默认不显示,显示后,可通过 progress.wasCanceled() 捕获状态
progress.setMaximum(100); // 当 minimun = maximun = 0 时, 调用 progressStep() 滚动条是一直滚动的繁忙状态
...

progress.setLabelText("正在处理数据..."); // 更新提示文字
progress.progressStep();  // 更新进度:步长1
*/

PMProgressBarDialog::PMProgressBarDialog(QWidget *parent,
                                         const QString &title,
                                         const QString &text,
                                         int max)
    : QProgressDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setMinimumWidth(270);
    if (title.isEmpty())
        setWindowTitle("请稍候...");
    else
        setWindowTitle(title);
    setLabelText(text);
    setCancelButton(nullptr); // 取消按钮默认不显示
    setAutoReset(false);
    setAutoClose(false);
    setModal(true);
    //setWindowModality(Qt::WindowModal);
    setRange(0,
             max); // 当其最大值 = 最小值 = 0，进度条会没有显示但一直在动,progressStep()后滚动条会一直滚动
    setMinimumDuration(0); // 后跟 setValue(0) 启作用
    setValue(0);
}

void PMProgressBarDialog::setShowCancelButton(bool bShow)
{
    if (bShow) {
        QPushButton *btn = new QPushButton(this);
        btn->setText("取消(&C)");
        setCancelButton(btn);
    } else
        setCancelButton(nullptr);
}

void PMProgressBarDialog::progressStep(int step)
{
    setValue(this->value() + step);
}

void PMProgressBarDialog::keyPressEvent(QKeyEvent *event)
{
    if (event && event->key() == Qt::Key_Escape) {
        event->accept();
    } else
        return QProgressDialog::keyPressEvent(event);
}

PMProgressBarDialog::~PMProgressBarDialog()
{
    //
}

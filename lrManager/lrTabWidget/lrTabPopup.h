#ifndef LRTABPOPUP_H
#define LRTABPOPUP_H

#include <QDialog>

/**
 * @brief 一个容纳tab弹出页的窗口
 */
class lrTabPopup : public QDialog
{
    Q_OBJECT
public:
    explicit lrTabPopup(QWidget *parent = nullptr);
    void setText(QString text);
    QString getText();
    void setTipText(QString tipText);
    QString getTipText();

    void setContentWidget(QWidget *page);
    QWidget* getContentWidget();

protected:
    bool event(QEvent *event) override;

signals:
    void dragRelease(const QPoint &globalPos);

private:
    QWidget *m_content = nullptr;
    QString m_text;
    QString m_tipText;
};

#endif // LRTABPOPUP_H

#include "lrTabPopup.h"

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDebug>

lrTabPopup::lrTabPopup(QWidget *parent)
    : QDialog(parent)
{

}

void lrTabPopup::setText(QString text)
{
    m_text = text;
}

QString lrTabPopup::getText()
{
    return m_text;
}

void lrTabPopup::setTipText(QString tipText)
{
    m_tipText = tipText;
}

QString lrTabPopup::getTipText()
{
    return m_tipText;
}

void lrTabPopup::setContentWidget(QWidget *page)
{
    if(!page)
        return;
    m_content = page;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setMargin(0);
    layout->addWidget(page);
}

QWidget *lrTabPopup::getContentWidget()
{
    return m_content;
}

bool lrTabPopup::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::MouseButtonRelease:
    case QEvent::NonClientAreaMouseButtonRelease:
    {
        QMouseEvent *e=static_cast<QMouseEvent*>(event);
        if(e&&e->button()==Qt::LeftButton)
        {
            emit dragRelease(e->globalPos());
        }
    }
        break;
    }
    return QDialog::event(event);
}


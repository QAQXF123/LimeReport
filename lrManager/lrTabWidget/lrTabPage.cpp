#include "lrTabPage.h"

#include <QDebug>
#include <QLabel>

//用于累加计算唯一id
int global_id=0;
//用于保存当前存在的page实例的id
QList<int> active_id;

lrTabPage::lrTabPage(const QString &fileName, QWidget *parent)
    : QWidget(parent)
    , m_fileName(fileName)
{
    //当前page的唯一id
    m_id = global_id++;
    active_id.push_back(m_id);
    setWindowTitle(m_fileName);

    qDebug() << m_id << "init" << active_id;
    init();
}

void lrTabPage::init()
{
    //
}

bool lrTabPage::save()
{
    return true;
}

lrTabPage::~lrTabPage()
{
    active_id.removeOne(m_id);
    qDebug() << m_id << "free" << active_id;
}

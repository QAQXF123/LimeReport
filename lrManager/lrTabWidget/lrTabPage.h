#ifndef LRTABPAGE_H
#define LRTABPAGE_H

#include <QWidget>

/**
 * @brief 单个Page页
 * @details 用于测试构造析构等
 */
class lrTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit lrTabPage(const QString &fileName, QWidget *parent = nullptr);
    ~lrTabPage();

    //获取唯一id
    int getId() const { return m_id; }
    QString getFileName() { return m_fileName; }

    virtual void init();
    virtual bool save();

private:
    int m_id = 0;
    QString m_fileName;
};

#endif // LRTABPAGE_H

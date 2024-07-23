#ifndef PMINIFILE_H
#define PMINIFILE_H

/*! @filename   : pminifile.h
 *  @brief      : 读写ini文件
 *  @note       : 模仿delphi封装用于读写ini文件
 *  @author     : hwf
 *  @date       : 2024-03-14 09:42:11 
 *  @version    : v1.0
 *  @attention  :
*/

#include <QSettings>
#include <QTextCodec>

class PMIniFile
{
private:
    QSettings *m_IniFile = nullptr;

public:
    PMIniFile(const QString &szFileName);
    ~PMIniFile();

public:
    // 写值
    void setValue(const QString aGroupName, const QString aKey, QVariant aValue);
    // 读值
    int readValueAsInt(const QString aGroupName, const QString aKey);
    bool readValueAsBool(const QString aGroupName, const QString aKey);
    QString readValueAsString(const QString aGroupName, const QString aKey);
    double readValueAsDouble(const QString aGroupName, const QString aKey);
    // 获取的所有组列表
    void obtainGroupList(QStringList &grouplist);
    // 获取组下所有key列表
    void obtainKeyList(const QString aGroupName, QStringList &keylist);
    // 删除组
    void removeGroup(const QString aGroupName);
    // 删除组下key
    void removeKey(const QString aGroupName, const QString aKey);
};

#endif // PMINIFILE_H

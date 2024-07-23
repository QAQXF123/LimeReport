#include "pminifile.h"
#include <QTextCodec>

PMIniFile::PMIniFile(const QString &szFileName)
{
    m_IniFile = new QSettings(szFileName, QSettings::IniFormat);
    m_IniFile->setIniCodec(QTextCodec::codecForName("GBK"));
}

void PMIniFile::setValue(const QString aGroupName, const QString aKey, QVariant aValue)
{
    if (aGroupName.isEmpty() || aKey.isEmpty())
        return;
    if (m_IniFile->isWritable())
        m_IniFile->setValue(aGroupName + "/" + aKey, aValue);
}

int PMIniFile::readValueAsInt(const QString aGroupName, const QString aKey)
{
    if (aGroupName.isEmpty() || aKey.isEmpty())
        return 0;
    return m_IniFile->value(aGroupName + "/" + aKey).toInt();
}
bool PMIniFile::readValueAsBool(const QString aGroupName, const QString aKey)
{
    if (aGroupName.isEmpty() || aKey.isEmpty())
        return false;
    return m_IniFile->value(aGroupName + "/" + aKey).toBool();
}
QString PMIniFile::readValueAsString(const QString aGroupName, const QString aKey)
{
    if (aGroupName.isEmpty() || aKey.isEmpty())
        return "";
    return m_IniFile->value(aGroupName + "/" + aKey).toString();
}
double PMIniFile::readValueAsDouble(const QString aGroupName, const QString aKey)
{
    if (aGroupName.isEmpty() || aKey.isEmpty())
        return 0;
    return m_IniFile->value(aGroupName + "/" + aKey).toDouble();
};

void PMIniFile::obtainGroupList(QStringList &grouplist)
{
    grouplist.clear();
    QStringList groups = m_IniFile->childGroups();
    for (int i = 0; i < groups.size(); i++)
        grouplist.append(groups.at(i));
}

void PMIniFile::obtainKeyList(const QString aGroupName, QStringList &keylist)
{
    keylist.clear();
    if (aGroupName.isEmpty())
        return;
    m_IniFile->beginGroup(aGroupName);
    QStringList keys = m_IniFile->allKeys();
    for (int i = 0; i < keys.size(); i++)
        keylist.append(keys.at(i));
    m_IniFile->endGroup();
}

void PMIniFile::removeGroup(const QString aGroupName)
{
    if (aGroupName.isEmpty())
        return;
    m_IniFile->remove(aGroupName);
}

void PMIniFile::removeKey(const QString aGroupName, const QString aKey)
{
    if (aGroupName.isEmpty() || aKey.isEmpty())
        return;
    m_IniFile->remove(aGroupName + "/" + aKey);
}

PMIniFile::~PMIniFile()
{
    delete m_IniFile;
}

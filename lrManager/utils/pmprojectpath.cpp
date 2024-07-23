#include "pmprojectpath.h"
#include <QDir>
#include "commonutils.h"

PMProjectPath::PMProjectPath()
{
    m_Special = new std::map<int, QString>;
    m_Province = new std::map<int, QString>;
    // 加载specialconfig.xml获取中文目录名
    m_Special->insert(std::pair<int, QString>(1, "工民建"));

    m_Province->insert(std::pair<int, QString>(1, "浙江"));
    m_Province->insert(std::pair<int, QString>(3, "安徽"));
    m_Province->insert(std::pair<int, QString>(7, "湖北"));
    m_Province->insert(std::pair<int, QString>(8, "湖南"));
    m_Province->insert(std::pair<int, QString>(9, "新疆"));
    m_Province->insert(std::pair<int, QString>(10, "海南"));
    m_Province->insert(std::pair<int, QString>(11, "河南"));
    m_Province->insert(std::pair<int, QString>(16, "江西"));
    m_Province->insert(std::pair<int, QString>(6, "辽宁"));
    m_Province->insert(std::pair<int, QString>(17, "北京"));
    m_Province->insert(std::pair<int, QString>(34, "云南"));
    m_Province->insert(std::pair<int, QString>(24, "河北"));
    m_Province->insert(std::pair<int, QString>(210, "上海"));
    m_Province->insert(std::pair<int, QString>(15, "广西"));
    m_Province->insert(std::pair<int, QString>(4, "江苏"));
    m_Province->insert(std::pair<int, QString>(22, "山西"));
}

PMProjectPath::~PMProjectPath()
{
    delete m_Special;
    delete m_Province;
}

QString PMProjectPath::GetSpecialRootPath(const int ASpecialId)
{
    std::map<int, QString>::iterator it = m_Special->find(ASpecialId);
    if (it == m_Special->end()) {
        return "";
    } else {
        return GetAppPath() + "/" + it->second;
    }
}

QString PMProjectPath::GetProvinceRootPath(const int ASpecialId, const int AProvinceId)
{
    QString specPath = GetSpecialRootPath(ASpecialId);
    if (specPath.isEmpty()) {
        return "";
    } else {
        std::map<int, QString>::iterator it = m_Province->find(AProvinceId);
        if (it == m_Province->end()) {
            return "";
        } else {
            return specPath + "/" + it->second + "/";
        }
    }
}

QString PMProjectPath::GetProvincePolicyFilesPath(const int ASpecialId, const int AProvinceId)
{
    return QDir::cleanPath(GetProvinceRootPath(ASpecialId, AProvinceId)) + "/policyfiles/";
}

QString PMProjectPath::GetProvinceDatabasePath(const int ASpecialId, const int AProvinceId)
{
    return QDir::cleanPath(GetProvinceRootPath(ASpecialId, AProvinceId)) + "/database/";
}

QString PMProjectPath::GetProvinceReportPath(const int ASpecialId, const int AProvinceId)
{
    return QDir::cleanPath(GetProvinceRootPath(ASpecialId, AProvinceId)) + "/report/";
}

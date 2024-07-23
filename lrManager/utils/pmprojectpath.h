#ifndef PMPROJECTPATH_H
#define PMPROJECTPATH_H

#include <QString>
#include <map>

class PMProjectPath
{
private:
    std::map<int, QString> *m_Special;
    std::map<int, QString> *m_Province;

public:
    PMProjectPath();
    ~PMProjectPath();
    // 专业根目录
    QString GetSpecialRootPath(const int ASpecialId);
    // 专业下省份根目录
    QString GetProvinceRootPath(const int ASpecialId, const int AProvinceId);
    // 专业下省份政策文件目录
    QString GetProvincePolicyFilesPath(const int ASpecialId, const int AProvinceId);
    // 专业下省份库数据目录
    QString GetProvinceDatabasePath(const int ASpecialId, const int AProvinceId);
    // 专业下省份报表目录
    QString GetProvinceReportPath(const int ASpecialId, const int AProvinceId);
};

#endif // PMPROJECTPATH_H

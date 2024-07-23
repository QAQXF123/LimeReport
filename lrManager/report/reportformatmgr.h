#ifndef REPORTFORMATMGR_H
#define REPORTFORMATMGR_H

#include <QString>

/*!
    @ClassName   : ReportFormatMgr
    @Description : 报表格式文件管理
*/
class ReportFormatMgr
{
public:
    ReportFormatMgr();
    ~ReportFormatMgr();

    void loadFromFile(const QString &fileName);
    bool saveToFile(const QString &fileName);
};

#endif // REPORTFORMATMGR_H

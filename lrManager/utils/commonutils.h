#ifndef COMMONUTILS_H
#define COMMONUTILS_H

/*! @filename   : commonutils.h
 *  @brief      : 公共函数
 *  @note       : 详细说明
 *  @author     : hwf
 *  @date       : 2024-03-14 09:44:31 
 *  @version    : v1.0
 *  @attention  :
*/

#include <QFileInfo>
#include <QString>
#include <QUuid>
#include <QWidget>

// ---------------程序路径相关---------------
// 获取程序所在目录：后不带/路径修正符
QString GetAppPath();
//　系统级配置目录：后含/路径修正符
QString GetAppConfigPath();
// 系统级资源目录：后含/路径修正符
QString GetAppResPath();
// 系统辅助工具目录：后含/路径修正符
QString GetAppToolsPath();
// 程序动态库目录：后含/路径修正符
QString GetAppPluginsPath();
// 程序工程备份目录：后含/路径修正符
QString GetAppBackupPath();
// 程序临时目录：后含/路径修正符
QString GetAppTempPath();
// 我的工程目录：后含/路径修正符
QString GetAppMyProjectPath();

//---------------程序对话框---------------
//系统询问对话框
bool PMQuestion(QString szText, QWidget *parent = nullptr, QString szTitle = "询问");
//系统警告对话框
void PMWarning(QString szText, QWidget *parent = nullptr, QString szTitle = "警告");
//系统出错对话框
void PMError(QString szText, QWidget *parent = nullptr, QString szTitle = "错误");
//系统提示对话框
void PMInfomation(QString szText, QWidget *parent = nullptr, QString szTitle = "提示");

// ---------------浮点数处理---------------
//四舍五入函数：返回类型为double，digit = -2 表示保留小数位后2位
double PMRoundTo(const double d, int digit);
//四舍五入函数(科学计算法、最后位上为0时显示0)：返回类型为QString，digit = -2 表示保留小数位后2位
QString PMRoundToQStr(const double d, int digit);
//判断一个浮点数是否为0
bool PMIsZero(const double d);
//判断2个浮点数是否相等
bool PMIsEquel(const double a, double b);

// ---------------字符串相关---------------
// 生成一个全部小写的guid串，参数说明：完整形式、不含大括号、不含大括号及横杠符号
QString PMGenerateGUID(QUuid::StringFormat mode = QUuid::WithBraces);
// 获取基本单位
QString PMGetBasicUnit(QString szUnit);
// 获取单位步距
int PMGetUnitStepDistance(QString szUnit);
// 计算字符串的小写的MD5值
QString PMCalculateMD5(QString &val);
// 计算文件的md5
QString PMCalculateFileMD5(const QString &szFileName);

// ---------------其它---------------
// 文件是否存在
bool PMIsFileExist(const QString &szFileName);
// 判断文件是否可读
bool PMIsFileReadable(const QString &szFileName);
// 创建文件夹：路径上的文件夹名若不存在，创建，文件夹分隔格式: \\ 或 /
void PMCreateDir(const QString &szDirPath);
// 复制文件：若目标文件中的路径不存在时，先创建路径，再复制文件
bool PMCopyFile(const QString &szSrcFile, const QString &szDestFile);
// 删除文件或路径
bool PMDeleteFileOrPath(const QString &szFilePath);
// 获取指定路径下唯一文件名:文件名＋数字＋扩展名
bool PMObtainUniqueFileName(const QString& fullPath, const QString& name, const QString& suffix, QString& retFileName);

#endif // COMMONUTILS_H

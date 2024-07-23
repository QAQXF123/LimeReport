#include "commonutils.h"
#include <QApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QtMath>

QString GetAppPath()
{
    return QApplication::applicationDirPath();
}

QString GetAppConfigPath()
{
    return GetAppPath() + "/config/";
}

QString GetAppResPath()
{
    return GetAppPath() + "/res/";
}

QString GetAppToolsPath()
{
    return GetAppPath() + "/tools/";
}

QString GetAppPluginsPath()
{
    return GetAppPath() + "/plugins/";
}

QString GetAppBackupPath()
{
    return GetAppPath() + "/backup/";
}

QString GetAppTempPath()
{
    return GetAppPath() + "/temp/";
}

QString GetAppMyProjectPath()
{
    return GetAppPath() + "/我的工程/";
}

bool PMQuestion(QString szText, QWidget *parent, QString szTitle)
{
    return QMessageBox::question(parent,
                                 szTitle,
                                 szText,
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes)
           == QMessageBox::Yes;
}

void PMWarning(QString szText, QWidget *parent, QString szTitle)
{
    QMessageBox::warning(parent, szTitle, szText, QMessageBox::Abort);
}

void PMError(QString szText, QWidget *parent, QString szTitle)
{
    QMessageBox::critical(parent, szTitle, szText, QMessageBox::Yes);
}

void PMInfomation(QString szText, QWidget *parent, QString szTitle)
{
    QMessageBox::information(parent, szTitle, szText, QMessageBox::Ok);
}

double PMRoundTo(const double d, int digit)
{
    // 问题1：四舍六入五成双，银行家算法，如：QString::asprintf("%.7f", d)、QString::number(d2, 'f', 7);
    // 问题2：精度丢失，QString toDouble() 精度：小数点后最多保留5位
    //      通过 qDebug() << qSetRealNumberPrecision(10) << PMRoundTo(d, -3); 来处理精度
    // 问题3：防止计算溢出
    // 注：本函数保留位数最多8位
    double lp = qPow(10.0, digit);
    if (d < 0) {
        return floor(d / lp - 0.500000001) * lp;
    } else {
        return floor(d / lp + 0.500000001) * lp;
    }
}

QString PMRoundToQStr(const double d, int digit)
{
    // 转字符串时，注意处理科学计算法的问题
    int iBit;
    if (digit < 0) {
        iBit = qAbs(digit);
    } else {
        iBit = 0;
    }
    return QString::number(PMRoundTo(d, digit), 'f', iBit);
}

bool PMIsZero(const double d)
{
    return qFuzzyIsNull(d);
}

bool PMIsEquel(const double a, double b)
{
    return qFuzzyCompare(a, b);
}

QString PMGenerateGUID(QUuid::StringFormat mode)
{
    QUuid uid = QUuid::createUuid();
    return uid.toString(mode);
}

QString PMGetBasicUnit(QString szUnit)
{
    szUnit = szUnit.trimmed();
    if (szUnit.isEmpty()) {
        return "";
    }
    int iPos = 0;
    QChar *data = szUnit.data();
    while (!data->isNull()) {
        if (!data->isNumber()) {
            break;
        }
        ++data;
        iPos++;
    }
    return szUnit.mid(iPos);
}

int PMGetUnitStepDistance(QString szUnit)
{
    szUnit = szUnit.trimmed();
    if (szUnit.isEmpty()) {
        return 1;
    }
    int iPos = 0;
    QChar *data = szUnit.data();
    while (!data->isNull()) {
        if (!data->isNumber()) {
            break;
        }
        ++data;
        iPos++;
    }
    if (iPos > 0) {
        return szUnit.left(iPos).toInt();
    } else {
        return 1;
    }
}

QString PMCalculateMD5(QString &val)
{
    return QCryptographicHash::hash(val.toUtf8(), QCryptographicHash::Md5).toHex();
}

QString PMCalculateFileMD5(const QString &szFileName)
{
    if (szFileName.isEmpty())
        return "";
    QFile md5File(szFileName);
    if (!md5File.exists())
        return "";
    md5File.open(QIODevice::ReadOnly);
    QString md5 = QCryptographicHash::hash(md5File.readAll(), QCryptographicHash::Md5).toHex();
    md5File.close();
    return md5;
}

bool PMIsFileExist(const QString &szFileName)
{
    if (szFileName.isEmpty())
        return false;
    return QFile::exists(szFileName);
}

void PMCreateDir(const QString &szDirPath)
{
    QString path = QDir::cleanPath(szDirPath);
    QStringList dirNameArray = path.split('/');
    int nameSize = dirNameArray.size();
    for (int i = 1; i < nameSize + 1; i++) {
        QString iBefAllDirStr = "";
        for (int j = 0; j < i; j++) {
            iBefAllDirStr += QString(dirNameArray.at(j) + '/');
        }

        QDir diri(iBefAllDirStr);
        if (!diri.exists()) {
            diri.mkdir(iBefAllDirStr);
        }
    }
}

bool PMCopyFile(const QString &szSrcFile, const QString &szDestFile)
{
    // 是否传入了空的路径||源文件是否存在
    if (szSrcFile.isEmpty() || szDestFile.isEmpty() || !QFile().exists(szSrcFile))
        return false;
    // 源文件路径与目标路径相同，返回false
    if (szSrcFile == szSrcFile)
        return false;

    // 判断目标文件的目录存不存在，不存在则创建
    QFileInfo fileInfo(szDestFile);
    QString sDirPath = fileInfo.absolutePath(); // 取目标文件所在的绝对目录路径
    if (!QDir().exists(sDirPath))
        PMCreateDir(sDirPath);

    // 如果文件存在，先删除目标路径文件，删除失败返回
    QFile sTempFile(szDestFile);
    if (sTempFile.exists()) {
        if (!sTempFile.remove())
            return false;
    }
    // 复制文件
    QFile file(szSrcFile);
    return file.copy(szDestFile);
}

bool PMDeleteFileOrPath(const QString &szFilePath)
{
    //是否传入了空的路径||路径是否存在
    if (szFilePath.isEmpty() || !QDir().exists(szFilePath))
        return false;
    QFileInfo FileInfo(szFilePath);
    if (FileInfo.isFile()) // 如果是文件
        return QFile::remove(szFilePath);
    else if (FileInfo.isDir()) // 如果是文件夹
    {
        QDir dir(szFilePath);
        return dir.removeRecursively();
    }
    return true;
}

bool PMObtainUniqueFileName(const QString& fullPath, const QString& name, const QString& suffix, QString& retFileName) {
    retFileName = "";
    if (fullPath.isEmpty() || name.isEmpty() || suffix.isEmpty())
        return false;
    QString path = QDir::cleanPath(fullPath);
    if (!QDir().exists(path))
        return false;
    retFileName = path + "/" + name + suffix;
    int idx = 0;
    while (QFile::exists(retFileName)) {
        idx++;
        retFileName = path + "/" + name + QString::number(idx) + suffix;
    }
    return true;
}

bool PMIsFileReadable(const QString &szFileName)
{
    QFile file(szFileName);
    if (!file.exists()) {
        return false;
    }
    QFileInfo fileInfo(szFileName);
    return fileInfo.isReadable();
}

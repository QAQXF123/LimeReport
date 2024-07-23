#include "pmquazipcompress.h"
#include <QCryptographicHash>
#include <QDebug>
#include "JlCompress.h"
#include "quazipnewinfo.h"

QStringList PMQuaZipCompress::getZipNames(const QString &zipFile)
{
    return JlCompress::getFileList(zipFile);
}

bool PMQuaZipCompress::compressDir(const QString &sourceDir, const QString &targetZip)
{
    return JlCompress::compressDir(targetZip, sourceDir);
}

bool PMQuaZipCompress::extractDir(const QString &sourceZip, const QString &targetDir)
{
    QStringList list = JlCompress::extractDir(sourceZip, targetDir);
    qDebug() << "extract files:" << list;
    return list.size() > 0;
}

bool PMQuaZipCompress::pmCompressFiles(const QString &targetZip,
                                       QStringList &targetNames,
                                       QStringList &sourceFiles,
                                       const QString &password)
{
    if ((targetNames.size() == 0) || (targetNames.size() != sourceFiles.size()))
        return false;

    QString p = "";
    if (!password.isEmpty()) {
        QString strMD5 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
        p = strMD5.mid(11, 13);
    }

    QuaZip newZip(targetZip); //要生成的zip文件
    QDir().mkpath(QFileInfo(targetZip).absolutePath());
    if (!newZip.open(QuaZip::mdCreate)) {
        QFile::remove(targetZip);
        return false;
    }
    newZip.setComment("Copyright  2024 The PMSS All rights reserved.");

    int fileCount = 0;
    QuaZipFile file(&newZip);
    for (int i = 0; i < targetNames.size(); ++i) {
        QuaZipNewInfo info(targetNames[i], sourceFiles[i]);
        // 0为文件夹、8为文件
        if (file.open(QIODevice::WriteOnly,
                      info,
                      p.isEmpty() ? nullptr : p.toUtf8().constData(),
                      0,
                      8)) {
            QFile file2(sourceFiles[i]);
            if (file2.open(QIODevice::ReadOnly)) {
                file.write(file2.readAll());
                file2.close();
                fileCount++;
            }
            file.close();
        }
    }
    newZip.close();
    return fileCount == targetNames.size();
}

bool PMQuaZipCompress::pmExtractFiles(const QString &sourceZip,
                                      QStringList &sourceNames,
                                      QStringList &targetFiles,
                                      const QString &password)
{
    if ((sourceNames.size() == 0) || (sourceNames.size() != targetFiles.size()))
        return false;

    QString p = "";
    if (!password.isEmpty()) {
        QString strMD5 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
        p = strMD5.mid(11, 13);
    }

    QuaZip newZip(sourceZip);
    if (!newZip.open(QuaZip::mdUnzip))
        return false;
    int fileCount = 0;
    for (int i = 0; i < sourceNames.size(); i++) {
        for (bool f = newZip.goToFirstFile(); f; f = newZip.goToNextFile()) {
            QString fileName = newZip.getCurrentFileName();
            if (fileName != sourceNames.at(i))
                continue;
            QuaZipFile zipFile(newZip.getZipName(), fileName);
            if (p.isEmpty() ? zipFile.open(QIODevice::ReadOnly)
                            : zipFile.open(QIODevice::ReadOnly, p.toUtf8().constData())) {
                QString unZipFileName = targetFiles.at(i);
                QDir().mkpath(QFileInfo(unZipFileName).absolutePath());
                QFile unZipFile(unZipFileName);
                if (unZipFile.open(QIODevice::WriteOnly)) {
                    unZipFile.write(zipFile.readAll());
                    unZipFile.close();
                    if (zipFile.size() == unZipFile.size())
                        fileCount++;
                    else
                        unZipFile.remove();
                }
                zipFile.close();
                // zipFile.getZipError() == UNZ_OK
            }
            break;
        }
    }
    newZip.close();
    return fileCount == sourceNames.size();
}

bool PMQuaZipCompress::pmExtractFiles(const QString &sourceZip,
                                      const QString &targetDir,
                                      const QString &password)
{
    QString p = "";
    if (!password.isEmpty()) {
        QString strMD5 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
        p = strMD5.mid(11, 13);
    }

    QuaZip newZip(sourceZip);
    if (!newZip.open(QuaZip::mdUnzip))
        return false;
    int fileCount = 0;
    for (bool f = newZip.goToFirstFile(); f; f = newZip.goToNextFile()) {
        QString fileName = newZip.getCurrentFileName();
        QuaZipFile zipFile(newZip.getZipName(), fileName);
        if (p.isEmpty() ? zipFile.open(QIODevice::ReadOnly)
                        : zipFile.open(QIODevice::ReadOnly, p.toUtf8().constData())) {
            QString unZipFileName = QDir::cleanPath(targetDir) + "/" + fileName;
            QDir().mkpath(QFileInfo(unZipFileName).absolutePath());
            QFile unZipFile(unZipFileName);
            if (unZipFile.open(QIODevice::WriteOnly)) {
                unZipFile.write(zipFile.readAll());
                unZipFile.close();
                if (zipFile.size() == unZipFile.size())
                    fileCount++;
                else
                    unZipFile.remove();
            }
            zipFile.close();
        }
    }
    newZip.close();
    return fileCount > 0;
}

bool PMQuaZipCompress::pmExtractFile(const QString &sourceZip,
                                     const QString &sourceName,
                                     QByteArray &byteArr,
                                     const QString &password)
{
    QString p = "";
    if (!password.isEmpty()) {
        QString strMD5 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
        p = strMD5.mid(11, 13);
    }

    QuaZip newZip(sourceZip);
    if (!newZip.open(QuaZip::mdUnzip))
        return false;
    byteArr.clear();
    bool bRet = false;
    for (bool f = newZip.goToFirstFile(); f; f = newZip.goToNextFile()) {
        QString fileName = newZip.getCurrentFileName();
        if (fileName != sourceName)
            continue;
        QuaZipFile zipFile(newZip.getZipName(), fileName);
        if (p.isEmpty() ? zipFile.open(QIODevice::ReadOnly)
                        : zipFile.open(QIODevice::ReadOnly, p.toUtf8().constData())) {
            byteArr.append(zipFile.readAll());
            bRet = byteArr.size() == zipFile.size();
            zipFile.close();
        }
        break;
    }
    newZip.close();
    return bRet;
}

// 将zip中的指定文件提取至指定QByteArray
bool PMQuaZipCompress::pmExtractFile(QuaZip &quaZip,
                                     const QString &sourceName,
                                     QByteArray &byteArr,
                                     const QString &password)
{
    QString p = "";
    if (!password.isEmpty()) {
        QString strMD5 = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
        p = strMD5.mid(11, 13);
    }

    if (!quaZip.isOpen()) {
        if (!quaZip.open(QuaZip::mdUnzip))
            return false;
    }

    byteArr.clear();
    bool bRet = false;
    for (bool f = quaZip.goToFirstFile(); f; f = quaZip.goToNextFile()) {
        QString fileName = quaZip.getCurrentFileName();
        if (fileName != sourceName)
            continue;
        QuaZipFile zipFile(quaZip.getZipName(), fileName);
        if (p.isEmpty() ? zipFile.open(QIODevice::ReadOnly)
                        : zipFile.open(QIODevice::ReadOnly, p.toUtf8().constData())) {
            byteArr.append(zipFile.readAll());
            bRet = byteArr.size() == zipFile.size();
            zipFile.close();
        }
        break;
    }
    return bRet;
}

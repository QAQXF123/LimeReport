#ifndef PMQUAZIPCOMPRESS_H
#define PMQUAZIPCOMPRESS_H

#include "quazip.h"

class PMQuaZipCompress
{
public:
    // 获取zip文件中的文件名列表
    static QStringList getZipNames(const QString &zipFile);

    // 压缩整个目录
    static bool compressDir(const QString &sourceDir, const QString &targetZip);
    // 解压至某个目录下
    static bool extractDir(const QString &sourceZip, const QString &targetDir);

    // 将多个不同路径中的文件进行压缩
    static bool pmCompressFiles(const QString &targetZip,
                                QStringList &targetNames,
                                QStringList &sourceFiles,
                                const QString &password = "");
    // 将zip中的指定文件解压至指定文件
    static bool pmExtractFiles(const QString &sourceZip,
                               QStringList &sourceNames,
                               QStringList &targetFiles,
                               const QString &password = "");

    // 将zip中的所有文件解压至指定目录
    static bool pmExtractFiles(const QString &sourceZip,
                               const QString &targetDir,
                               const QString &password = "");

    // 将zip中的指定文件提取至指定QByteArray
    static bool pmExtractFile(const QString &sourceZip,
                              const QString &sourceName,
                              QByteArray &byteArr,
                              const QString &password = "");
    // 将zip中的指定文件提取至指定QByteArray
    static bool pmExtractFile(QuaZip &quaZip,
                              const QString &sourceName,
                              QByteArray &byteArr,
                              const QString &password = "");
};

#endif // PMQUAZIPCOMPRESS_H

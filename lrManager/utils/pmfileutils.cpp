#include "pmfileutils.h"
#include <QDebug>
#include <QFile>

bool PMFileReader::readXmlFromFile(const QString szFileName, QDomDocument& outDoc) {
    if (szFileName.isEmpty()) {
        qDebug() << "filename:" << szFileName << " is empty.";
        return false;
    }
    QFile file(szFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "open file error:" << file.errorString();
        file.close();
        return false;
    }
    QString errorInfo;
    int irow, icol;
    if (!outDoc.setContent(&file, &errorInfo, &irow, &icol)) {
        qDebug() << "xmldom setcontent error:" << errorInfo
                 << ",failed at line:" << QString::number(irow);
        file.close();
        return false;
    }
    if (outDoc.isNull()) {
        qDebug() << "file:" << szFileName << " is null.";
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool PMFileReader::readXmlFromString(const QString& szXmlContent, QDomDocument& outDoc) {
    if (szXmlContent.isEmpty()) {
        qDebug() << "readXmlFromString content is empty.";
        return false;
    }
    QString errorInfo;
    int irow, icol;
    if (!outDoc.setContent(szXmlContent, &errorInfo, &irow, &icol)) {
        qDebug() << "xmldom setcontent error:" << errorInfo
                 << ",failed at line:" << QString::number(irow);
        return false;
    }
    if (outDoc.isNull()) {
        qDebug() << "xml content:" << szXmlContent << " is null.";
        return false;
    }
    return true;
}

bool PMFileReader::readStrFromFile(const QString szFileName, QString& outStr) {
    if (szFileName.isEmpty())
        return false;

    QFile file(szFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "read file:" << szFileName << " fail.";
        return false;
    }
    outStr = file.readAll();
    file.close();
    return true;
}

bool PMFileWriter::saveXmlToFile(const QDomDocument &inDoc, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }
    QTextStream out(&file);
    out.setCodec("utf-8");

    inDoc.save(out, 4, QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}

bool PMFileWriter::saveStrToFile(const QString &szFileName, const QString &inStr)
{
    if (szFileName.isEmpty())
        return false;

    QFile file(szFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "write file:" << szFileName << " fail.";
        return false;
    }
    QTextStream out(&file);
    out.setCodec("utf-8");
    out << inStr;
    file.close();
    return true;
}

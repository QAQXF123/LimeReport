#ifndef PMFILEUTILS_H
#define PMFILEUTILS_H

#include <QSettings>
#include <QtXml/QDomDocument>

class PMFileReader
{
public:
    static bool readXmlFromFile(const QString szFileName, QDomDocument& outDoc);
    static bool readXmlFromString(const QString& szXmlContent, QDomDocument& outDoc);
    static bool readStrFromFile(const QString szFileName, QString& outStr);
};

class PMFileWriter
{
public:
    static bool saveXmlToFile(const QDomDocument &inDoc, const QString &fileName);
    static bool saveStrToFile(const QString &szFileName, const QString &inStr);
};

#endif // PMFILEUTILS_H

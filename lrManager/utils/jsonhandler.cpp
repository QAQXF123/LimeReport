#include "jsonhandler.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

JsonHandler::JsonHandler() {}

bool JsonHandler::readFileContent(const QString &fileName, QString &outStr)
{
    outStr = "";
    if (fileName.isEmpty())
        return false;

    if (!QFile::exists(fileName))
        return false;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "read file:" << fileName << " fail, error:" << file.error();
        return false;
    }
    outStr = file.readAll();
    file.close();
    return true;
}

bool JsonHandler::loadFromString(const QString &inStr)
{
    if (inStr.isEmpty())
        return false;
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(inStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parsing error at offset" << parseError.offset << ":"
                   << qPrintable(parseError.errorString());
        return false;
    }
    m_root = jsonDoc.object();
    return true;
}

bool JsonHandler::loadFromFile(const QString &fileName)
{
    QString fileStr;
    if (!readFileContent(fileName, fileStr))
        return false;
    return loadFromString(fileStr);
}

QString JsonHandler::saveToString() {
    QJsonDocument doc(m_root);
    return doc.toJson(QJsonDocument::Indented);
}

bool JsonHandler::saveToFile(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << qPrintable(file.errorString());
        return false;
    }
    QJsonDocument doc(m_root);
    QTextStream stream(&file);
    stream << doc.toJson(QJsonDocument::Indented);
    file.close(); // Ensure the file is properly closed
    return true;
}

int JsonHandler::getValueAsInt(const QString &key)
{
    if (!m_root.contains(key))
        return 0;
    QJsonValue value = m_root.value(key);
    return value.toInt();
}

bool JsonHandler::getValueAsBool(const QString &key)
{
    if (!m_root.contains(key))
        return false;
    QJsonValue value = m_root.value(key);
    return value.toBool();
}

double JsonHandler::getValueAsDouble(const QString &key)
{
    if (!m_root.contains(key))
        return 0;
    QJsonValue value = m_root.value(key);
    return value.toDouble();
}

QString JsonHandler::getValueAsString(const QString &key)
{
    if (!m_root.contains(key))
        return "";
    QJsonValue value = m_root.value(key);
    return value.toString();
}

QVariant JsonHandler::getValue(const QString& key) {
    if (m_root.contains(key))
        return m_root.value(key).toVariant();
    else
        return QVariant();
}

void JsonHandler::setValue(const QString& key, const QVariant& value) {
    QJsonValue jval = QJsonValue::fromVariant(value);
    // jval.fromVariant(value);
    m_root.insert(key, jval);
}

bool JsonHandler::contains(const QString& key) {
    return m_root.contains(key);
}

bool JsonHandler::isEmpty() {
    return m_root.isEmpty();
}

// 删除JSON键
bool JsonHandler::remove(const QString &key)
{
    if (m_root.contains(key)) {
        m_root.remove(key);
        return true;
    }
    return false;
}

// 将JSON对象转换为QMap<QString, QJsonValue>
QMap<QString, QJsonValue> JsonHandler::jsonObjectToMap() const
{
    QMap<QString, QJsonValue> map;
    for (auto it = m_root.begin(); it != m_root.end(); ++it) {
        map.insert(it.key(), it.value());
    }
    return map;
}

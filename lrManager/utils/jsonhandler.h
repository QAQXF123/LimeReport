#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QJsonObject>
#include <QString>

class JsonHandler
{
private:
    QJsonObject m_root;
    // 读文件内容
    bool readFileContent(const QString &fileName, QString &outStr);

public:
    JsonHandler();
    // 根据json串加载
    bool loadFromString(const QString &inStr);
    // 根据json文件加载
    bool loadFromFile(const QString &fileName);
    // 保存为json串
    QString saveToString();
    // 保存为json文件
    bool saveToFile(const QString& fileName);
    // 根据键获取值，int型
    int getValueAsInt(const QString &key);
    // 根据键获取值，bool型
    bool getValueAsBool(const QString &key);
    // 根据键获取值，double型
    double getValueAsDouble(const QString &key);
    // 根据键获取值，QString型
    QString getValueAsString(const QString &key);
    // 获取值
    QVariant getValue(const QString& key);
    // 设置键值: JsonValue(bool b);  QJsonValue(double n); QJsonValue(int n); QJsonValue(qint64 v);  QJsonValue(const QString &s);
    void setValue(const QString& key, const QVariant& value);
    // 是否包括键
    bool contains(const QString& key);
    // 删除JSON键
    bool remove(const QString &key);
    // 将JSON对象转换为QMap<QString, QJsonValue>
    QMap<QString, QJsonValue> jsonObjectToMap() const;
    // 是否为空
    bool isEmpty();
};

#endif // JSONHANDLER_H

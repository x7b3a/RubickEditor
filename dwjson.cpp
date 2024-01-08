#include "dwjson.h"

#include <QTextCodec>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QJsonObject>

dwJson::dwJson()
{

}
QJsonDocument dwJson::read_json(QString filename)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //РёР·РјРµРЅРµРЅРёСЏ
    QString val;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonParseError error;
        QJsonDocument Doc = QJsonDocument::fromJson(val.toUtf8(), &error);
        if (error.error != QJsonParseError::NoError)
        {
            errors++;
        }
        qDebug() << errors;
        return Doc;
}

QMap<QString, QString> dwJson::map_parser(QJsonObject item, QString word)
{
    QMap<QString, QString> array;
    QJsonObject keywords_value = item[word].toObject();
    foreach(const QString& key, keywords_value.keys()) {
        QJsonValue value = keywords_value.value(key);
        array.insert(key,value.toString());
    }
    return array;
}
int dwJson::error_json(QString filename)
{
    return 1;
}

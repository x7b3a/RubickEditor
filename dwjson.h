#ifndef DWJSON_H
#define DWJSON_H
#include <QDialog>

class dwJson
{
public:
    dwJson();
    int errors = 0;
    QJsonDocument read_json(QString filename);
    int error_json(QString filename);
    QMap<QString, QString> map_parser(QJsonObject item, QString word);

};

#endif // DWJSON_H

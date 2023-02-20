#ifndef DWJSON_H
#define DWJSON_H
#include <QDialog>

class dwJson
{
public:
    dwJson();
    QJsonDocument read_json(QString filename);
    int error_json(QString filename);

};

#endif // DWJSON_H

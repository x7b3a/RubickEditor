#include "dwjson.h"
#include <QTextCodec>
#include <QJsonDocument>
#include <QFile>
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
            error_json(filename);
          //  ui->error->setStyleSheet("color: rgba(255,0,0,255);");
            //ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+filename+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
        }
        return Doc;
}
int dwJson::error_json(QString filename)
{
    return 1;
}

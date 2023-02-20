#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include "macros.h"


Macros::Macros()
{

}

/*void Macros::WikiAndFixes()
{
    qDebug() << "WikiAndFixes called";
    progress->setValue(1);
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("WikiAndFixes").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    QJsonArray letter_with_for_medoke = WikiAndFixes["letter_with_for_medoke"].toArray();
    progress->setValue(50);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    progress->setValue(75);
    QRegExp texp = QRegExp(" " + letter_with_for_medoke[0].toString()+ " 1([0-9][0-9][^0-9])");
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(" " + letter_with_for_medoke[1].toString() + " 1" + texp.cap(1)));
    }
    progress->setValue(100);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}*/

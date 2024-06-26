﻿#include "dwnetmacros.h"
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <macros.h>

dwNetMacros::dwNetMacros()
{
    version="";
}
dwNetMacros::dwNetMacros(QString ver)
{
    version = ver;
}

dwNetMacros::dwNetMacros(QString ver, int lan)
{
    qDebug() << "dwNetMacros2 called";
    version = ver;
    language = lan;
}

dwNetMacros::~dwNetMacros()
{
    cleaning();
}
void dwNetMacros::cleaning()
{
    version.clear();
    output.clear();
    isFromPatch = false;
}
void  dwNetMacros::Patch_heroes()
{
    qDebug() << "Patch heroes called";
    QString number = version;
    //HTML_Parser parser;
    send_progress(1);
    QString url  = version;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   // qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QUrl patch_url("https://www.dota2.com/datafeed/patchnotes?version=" + number + "&language=russian");
    QNetworkRequest patch_request(patch_url);

    QNetworkReply* patch_reply= manager->get(patch_request);
    connect(patch_reply, SIGNAL(finished()),this,  SLOT(replyFinishedH()));
   // patch_reply->close();
   // patch_reply->deleteLater();
    //manager->deleteResource(patch_request);
    patch_url.clear();
    qDebug() << "connect?";
    send_progress(25);
}

void  dwNetMacros::Patch_items() //DO IT DO IT DO IT DO IT
{
    qDebug() << "Patch items called";
    QString number = version;
    //HTML_Parser parser;
    send_progress(1);
    QString url  = version;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   // qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QUrl patch_url("https://www.dota2.com/datafeed/patchnotes?version=" + number + "&language=russian");
    QNetworkRequest patch_request(patch_url);
    QNetworkReply* patch_reply=  manager->get(patch_request);
    connect(patch_reply, SIGNAL(finished()),this,  SLOT(replyFinishedI()));
   // patch_reply->close();
   // patch_reply->deleteLater();
    //manager->deleteResource(patch_request);
    patch_url.clear();
    qDebug() << "connect?";
    send_progress(25);
}

void  dwNetMacros::Patch_Version(int a)
{
    language = a;
    qDebug() << "Patch version called";
    QString number = version;
    //HTML_Parser parser;
    send_progress(1);
    QString url  = version;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   // qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QString urlstring = "https://www.dota2.com/datafeed/patchnotes?version=" + number;
    if (!language)
        urlstring+="&language=russian";
    else
        urlstring+="&language=english";
    QUrl patch_url(urlstring);
    QNetworkRequest patch_request(patch_url);
    QNetworkReply* patch_reply1=  manager->get(patch_request);
   connect(patch_reply1, SIGNAL(finished()),this,  SLOT(replyFinishedV()));
   // patch_reply->close();
   // patch_reply->deleteLater();
    //manager->deleteResource(patch_request);
    patch_url.clear();
    qDebug() << "connect?";
    send_progress(15);
}

void  dwNetMacros::replyFinishedV()
{
    qDebug() << "replyFinishedV called";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)

  {
      hero_list.clear();
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
    QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
    VersionJsonObj = doc.object();
    //QJsonArray heroes = JsonObj["heroes"].toArray();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString urlstring = "https://www.dota2.com/datafeed/herolist?language=";
    if (!language)
        urlstring+="russian";
    else
        urlstring+="english";
    QUrl heroes_url(urlstring);
     QNetworkRequest heroes_request(heroes_url);
     QNetworkReply* heroes_reply = manager->get(heroes_request);

     //reply->close();
     reply->deleteLater();

     connect(heroes_reply, SIGNAL(finished()),this,  SLOT(replyFinishedV2()));
     //heroes_reply-> deleteLater();
     //manager->deleteResource(heroes_request);
     heroes_url.clear();
  }
  else
  {
      send_progress(-1);
    output = reply->errorString(); emit send_end();
  }
  // разрешаем объекту-ответа "удалиться"
  send_progress(30);
}

void  dwNetMacros::replyFinishedV2()
{
     qDebug() << "replyFinishedV2 called";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)
  {
      hero_list.clear();
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
    QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
    HeroJsonObj = doc.object();
    //QJsonArray heroes = JsonObj["heroes"].toArray();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString urlstring = "https://www.dota2.com/datafeed/itemlist?language=";
    if (!language)
        urlstring+="russian";
    else
        urlstring+="english";
    QUrl heroes_url(urlstring);
     QNetworkRequest heroes_request(heroes_url);
     QNetworkReply* heroes_reply = manager->get(heroes_request);

     //reply->close();
     reply->deleteLater();

     connect(heroes_reply, SIGNAL(finished()),this,  SLOT(replyFinishedV3()));
     //heroes_reply-> deleteLater();
     //manager->deleteResource(heroes_request);
     heroes_url.clear();
  }
  else
  {
      send_progress(-1);
    output = reply->errorString(); emit send_end();
  }
  // разрешаем объекту-ответа "удалиться"
  send_progress(45);
}
void  dwNetMacros::replyFinishedV3()
{
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)

  {
      hero_list.clear();
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
   QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
   ItemJsonObj = doc.object();
    //QJsonArray heroes = JsonObj["heroes"].toArray();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString urlstring = "https://www.dota2.com/datafeed/abilitylist?language=";
    if (!language)
        urlstring+="russian";
    else
        urlstring+="english";
    QUrl heroes_url(urlstring);
     QNetworkRequest heroes_request(heroes_url);
     QNetworkReply* heroes_reply = manager->get(heroes_request);

     //reply->close();
     reply->deleteLater();

     connect(heroes_reply, SIGNAL(finished()),this,  SLOT(replyFinishedVersion()));
     //heroes_reply-> deleteLater();
     //manager->deleteResource(heroes_request);
     heroes_url.clear();
  }
  else
  {
      send_progress(-1);
    output = reply->errorString(); emit send_end();
  }
  // разрешаем объекту-ответа "удалиться"
  send_progress(60);
}

void  dwNetMacros::replyFinishedH()
{
     qDebug() << "replyFinishedH called";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)

  {
      hero_list.clear();
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
    QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
    QJsonObject JsonObj = doc.object();
    QJsonArray heroes = JsonObj["heroes"].toArray();

    foreach (const QJsonValue & v, heroes)
    {
         qDebug() << v.toObject().value("hero_id").toInt();
         hero_list.insert(v.toObject().value("hero_id").toInt());
    }
          qDebug() <<hero_list.size();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl heroes_url("https://www.dota2.com/datafeed/herolist?language=russian");
     QNetworkRequest heroes_request(heroes_url);
     QNetworkReply* heroes_reply = manager->get(heroes_request);

     //reply->close();
     reply->deleteLater();

     connect(heroes_reply, SIGNAL(finished()),this,  SLOT(replyFinishedHeroes()));
     //heroes_reply-> deleteLater();
     //manager->deleteResource(heroes_request);
     heroes_url.clear();
  }
  else
  {
      send_progress(-1);
    output = reply->errorString(); emit send_end();
  }
  // разрешаем объекту-ответа "удалиться"
  send_progress(55);
}
void  dwNetMacros::replyFinishedI() //DO IT DO IT DO IT
{
     qDebug() << "replyFinishedI called";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)

  {
      hero_list.clear();
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
    QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
    QJsonObject JsonObj = doc.object();
    QJsonArray heroes = JsonObj["heroes"].toArray();

    foreach (const QJsonValue & v, heroes)
    {
         qDebug() << v.toObject().value("hero_id").toInt();
         hero_list.insert(v.toObject().value("hero_id").toInt());
    }
          qDebug() <<hero_list.size();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl heroes_url("https://www.dota2.com/datafeed/herolist?language=russian");
     QNetworkRequest heroes_request(heroes_url);
     QNetworkReply* heroes_reply = manager->get(heroes_request);

     //reply->close();
     reply->deleteLater();

     connect(heroes_reply, SIGNAL(finished()),this,  SLOT(replyFinishedHeroes()));
     //heroes_reply-> deleteLater();
     //manager->deleteResource(heroes_request);
     heroes_url.clear();
  }
  else
  {
      send_progress(-1);
    output = reply->errorString(); emit send_end();
  }
  // разрешаем объекту-ответа "удалиться"
  send_progress(55);
}
void  dwNetMacros::replyFinishedItems() //DO IT DO IT DO IT DO IT
{
     qDebug() << "replyFinishedItems called";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() == QNetworkReply::NoError)
    {
    QByteArray content= reply->readAll();
    //QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString undercontent = QString(content);

 qDebug () << "f";
  //ui->text1->setPlainText(codec->toUnicode(content.data()));
  QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
  QJsonObject JsonObj = doc.object();
  QJsonObject Result  = JsonObj.value("result").toObject();
  QJsonObject Data =  Result.value("data").toObject();
  QJsonArray heroes = Data["heroes"].toArray();
  QString output = "";
  QMap<int, QString> dict;
  foreach(const QJsonValue & v, heroes) {
       //qDebug() << v.toObject().value("name_loc");
       dict.insert(v.toObject().value("id").toInt(), v.toObject().value("name_english_loc").toString());
  }
  send_progress(75);
  QList <QString> out;
  for (QSet<int>::iterator i = hero_list.begin(); i!= hero_list.end();i++)
  {
      out.insert(0,dict.value(*i));
    //  qDebug() << dict.value(1) << *i;
  }
  std::sort(out.begin(), out.end());
  foreach (QString value, out)
  {
      output += value;
      output +=", ";
  }
  output.resize(output.size()-2);
  emit send_end();
  send_progress(100);
    }
    else
    {
      // Выводим описание ошибки, если она возникает.
      send_progress(-1);
      output = reply->errorString(); emit send_end();
    }
    //reply->close();
    reply->deleteLater();
    send_progress(0);
}
void  dwNetMacros::replyFinishedHeroes()
{
     qDebug() << "replyFinishedHeroes called";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() == QNetworkReply::NoError)
    {
    QByteArray content= reply->readAll();
   // QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString undercontent = QString(content);

 qDebug () << "f";
  //ui->text1->setPlainText(codec->toUnicode(content.data()));
  QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
  QJsonObject JsonObj = doc.object();
  QJsonObject Result  = JsonObj.value("result").toObject();
  QJsonObject Data =  Result.value("data").toObject();
  QJsonArray heroes = Data["heroes"].toArray();
  //QString output = "";
  QMap<int, QString> dict;
  foreach(const QJsonValue & v, heroes) {
       //qDebug() << v.toObject().value("name_loc");
       dict.insert(v.toObject().value("id").toInt(), v.toObject().value("name_english_loc").toString());
     // qDebug() << v.toObject().value("id").toInt() << v.toObject().value("name_english_loc").toString();
  }
  send_progress(75);
  QList <QString> out;
  for (QSet<int>::iterator i = hero_list.begin(); i!= hero_list.end();i++)
  {
      out.insert(0,dict.value(*i));
    //  qDebug() << dict.value(1) << *i;
  }
  std::sort(out.begin(), out.end());
  foreach (QString value, out)
  {
      output += value;
      output +=", ";
  }
  output.resize(output.size()-2);
  emit send_end();
  send_progress(100);
    }
    else
    {
      // Выводим описание ошибки, если она возникает.
      send_progress(-1);
      output = reply->errorString(); emit send_end();
    }
    //reply->close();
    reply->deleteLater();
    send_progress(0);
}

void  dwNetMacros::replyFinishedVersion()
{
     qDebug() << "replyFinishedVersion called";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (reply->error() == QNetworkReply::NoError)
    {
    QByteArray content= reply->readAll();
    //QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString undercontent = QString(content);

 qDebug () << "f";
  //ui->text1->setPlainText(codec->toUnicode(content.data()));
  QJsonDocument doc = QJsonDocument::fromJson(undercontent.toUtf8());
  AbilityJsonObj = doc.object();
  send_progress(75);
  Do_Patch();

  /*QString output = "";

  output.resize(output.size()-2);
  //put_text(output);*/
    }
    else
    {
      // Выводим описание ошибки, если она возникает.
      send_progress(-1);
      output = reply->errorString(); emit send_end();
    }
    //reply->close();
    reply->deleteLater();
    send_progress(0);
}

QString  dwNetMacros::add_point(QString output1)
{
    if (output1.endsWith(" "))
    {
         output1.resize(output1.size()-1);
         output1+=".";
    }
    else
    if (!output1.endsWith(".")&&!output1.endsWith(":")&&!output1.endsWith("}")&&!output1.endsWith(".)"))
        output1+=".";
    return output1;
}

void  dwNetMacros::Do_Patch()
{
     qDebug() << "DoPatch called";
    QJsonObject Result  = HeroJsonObj.value("result").toObject();
    QJsonObject Data =  Result.value("data").toObject();
    QJsonArray id_list = Data["heroes"].toArray();
    QMap<int, QString> dict_heroes;
    foreach(const QJsonValue & v, id_list) {
         //qDebug() << v.toObject().value("name_loc");
         dict_heroes.insert(v.toObject().value("id").toInt(), v.toObject().value("name_loc").toString());
    }

    Result  = ItemJsonObj.value("result").toObject();
    Data =  Result.value("data").toObject();
    id_list = Data["itemabilities"].toArray();
    QMap<int, QString> dict_items;
    foreach(const QJsonValue & v, id_list) {
         dict_items.insert(v.toObject().value("id").toInt(), v.toObject().value("name_loc").toString());
    }

    Result  = AbilityJsonObj.value("result").toObject();
    Data =  Result.value("data").toObject();
    id_list = Data["itemabilities"].toArray();
    QMap<int, QString> dict_abilities;
    foreach(const QJsonValue & v, id_list) {
         dict_abilities.insert(v.toObject().value("id").toInt(), v.toObject().value("name_loc").toString());
    }

    QJsonArray generic = VersionJsonObj["generic"].toArray();
    QJsonArray neutral_creeps = VersionJsonObj["neutral_creeps"].toArray();
    QJsonArray neutral_items = VersionJsonObj["neutral_items"].toArray();
    QJsonArray items = VersionJsonObj["items"].toArray();
    QJsonArray heroes = VersionJsonObj["heroes"].toArray();
    QJsonValue time = VersionJsonObj["patch_timestamp"];
    QString tempstring;
    //QString output = "";
    QRegExp texp;
    output += "{{Version infobox\n| version = " + version +"\n| image = \n| highlights = \n| new = \n| significant = \n| buffed =\n| nerfed = \n| dota2 = " + QDateTime::fromTime_t(time.toInt()).toString("yyyy-MM-dd") +"\n}}\n&lt;onlyinclude&gt;\n\n";
    if (!language)
    {
        if (!generic.isEmpty())
            output += QStringLiteral(u"== Общие изменения ==\n");
        foreach (QJsonValue value, generic)
        {
            if (value.toObject().value("note").toString()!="<br>")
            {
                output += "* ";
                tempstring = value.toObject().value("note").toString();
                output += add_point(tempstring);
                tempstring.clear();
               /* output += "\n";
                output += value.toObject().value("info").toString();*/
                output += "\n";
            }
            output += "\n";
        }
        if (!neutral_creeps.isEmpty())
            output += QStringLiteral(u"== Нейтральные крипы ==\n");
        foreach (QJsonValue value, neutral_creeps)
        {
            output += "{{Unit label|";
            output += value.toObject().value("localized_name").toString();
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["neutral_creep_notes"].toArray();
            foreach (QJsonValue value2, temparray)
                {

                    if (value2.toObject().value("note").toString()!="<br>")
                    {
                        QString star = "* ";
                        QString start = "{{A|";
                        QString end = "}}";
                        QString mid = "|";
                        QString duo = ":" ;
                        texp = QRegExp("[\* \s]{2}([A-Za-z \s \']{1,25}):");
                        //qDebug() << texp;
                        tempstring = "* ";
                        tempstring += value2.toObject().value("note").toString();
                       // qDebug() << tempstring;
                        while(texp.indexIn(tempstring)!=-1)
                        {
                            //qDebug() << texp.cap(0) << texp.cap(1);
                            tempstring.replace(texp.cap(0), star + start   + value.toObject().value("localized_name").toString()+ mid+ texp.cap(1) + end + duo);
                            break;
                        }
                        output += add_point(tempstring);
                        tempstring.clear();
                        output += "\n";
                    }

                }
            output += "\n";
           /* output += "\n";
            output += value.toObject().value("info").toString();*/
            //NEED TO ADD INFO ELEMENT OF JSON
        }
        if (!items.isEmpty())
            output += QStringLiteral(u"== Предметы ==\n");
        foreach (QJsonValue value, items)
        {
            output += "{{Item label|";
            output += dict_items.value(value.toObject().value("ability_id").toInt());
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["ability_notes"].toArray();
            QRegExp texp;
            QString stringtexp;
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    QString star = "* ";
                    QString start = "{{A|";
                    QString end = "}}";
                    QString mid = "|";
                    QString duo = ":" ;
                    texp = QRegExp("[\* \s]{2}([A-Za-z \s \']{1,25}):");
                    //qDebug() << texp;
                    tempstring = "* ";
                    tempstring += value2.toObject().value("note").toString();
                   // qDebug() << tempstring;
                    while(texp.indexIn(tempstring)!=-1)
                    {
                        //qDebug() << texp.cap(0) << texp.cap(1);
                        tempstring.replace(texp.cap(0), star + start + dict_items.value(value.toObject().value("ability_id").toInt()) + mid + texp.cap(1) + end + duo);
                        break;
                    }
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }
            output += "\n";
        }
        if (!neutral_items.isEmpty())
            output += QStringLiteral(u"== Нейтральные предметы ==\n");
        foreach (QJsonValue value, neutral_items)
        {
            output += "{{Item label|";
            output += dict_items.value(value.toObject().value("ability_id").toInt());
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["ability_notes"].toArray();
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    QString star = "* ";
                    QString start = "{{A|";
                    QString end = "}}";
                    QString mid = "|";
                    QString duo = ":" ;
                    texp = QRegExp("[\* \s]{2}([A-Za-z \s \']{1,25}):");
                    //qDebug() << texp;
                    tempstring = "* ";
                    tempstring += value2.toObject().value("note").toString();
                   // qDebug() << tempstring;
                    while(texp.indexIn(tempstring)!=-1)
                    {
                        tempstring.replace(texp.cap(0), star + start   + dict_items.value(value.toObject().value("ability_id").toInt())+ mid+ texp.cap(1) + end + duo);
                        break;
                    }
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }
            output += "\n";
        }
        if (!heroes.isEmpty())
            output += QStringLiteral(u"== Герои ==\n");
        QJsonArray heroes2;
        foreach (QJsonValue value, heroes)
        {
            QJsonObject temp =value.toObject();
            temp.insert("hero_name", dict_heroes.value(value.toObject().value("hero_id").toInt()));
            heroes2.append(temp);
        }

        std::sort(heroes2.begin(), heroes2.end(), [](const QJsonValue &v1, const QJsonValue &v2) {
            return v1.toObject()["hero_name"].toString() < v2.toObject()["hero_name"].toString();
        });

        foreach (QJsonValue value, heroes2)
        {
            output += "{{Hero label|";
            output += value.toObject().value("hero_name").toString();
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["hero_notes"].toArray();
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    output += "* ";
                    tempstring = value2.toObject().value("note").toString();
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }
            temparray = value["abilities"].toArray();
            foreach (QJsonValue value2, temparray)
            {
                output += "* {{A|";
                output += value.toObject().value("hero_name").toString();
                output += "|";
                output += dict_abilities.value(value2.toObject().value("ability_id").toInt());
                QJsonArray temparray2 = value2["ability_notes"].toArray();
                if (temparray2.size()>1)
                {
                    output += "}}\n";
                    foreach (QJsonValue value3, temparray2)
                    {
                        if (value3.toObject().value("note").toString()!="<br>")
                        {
                            output += "** ";
                            tempstring = value3.toObject().value("note").toString();
                            output += add_point(tempstring);
                            tempstring.clear();
                            output += "\n";
                        }
                    }
                }
                else if (temparray2.size()==1)
                {

                    output += "}}: ";
                    foreach (QJsonValue value3, temparray2)
                    {
                        if (value3.toObject().value("note").toString()!="<br>")
                        {
                            tempstring = value3.toObject().value("note").toString();
                            tempstring[0] = tempstring[0].toLower();
                            output += add_point(tempstring);
                            tempstring.clear();
                            output += "\n";
                        }
                    }
                }
            }

            temparray = value["talent_notes"].toArray();
            if (!temparray.isEmpty())
                output += QStringLiteral(u"* {{Значок|Талант}} '''[[Таланты]]:'''\n");
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    output += "** ";
                    tempstring = value2.toObject().value("note").toString();
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }

            output += "\n";
        }
        if (output.back()=='\n')
            output.chop(1);
        output += QStringLiteral(u"&lt;/onlyinclude&gt;\n\n== См. также ==\n* [[Версии]]\n* [[Обновления]]\n\n{{Система}}\n\n[[en:Version ") + version + "]]";

    }
    else //ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH
        //ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH ENGLISH
    {
        if (!generic.isEmpty())
            output += "== General ==\n";
        foreach (QJsonValue value, generic)
        {
            if (value.toObject().value("note").toString()!="<br>")
            {
                output += "* ";
                tempstring = value.toObject().value("note").toString();
                output += add_point(tempstring);
                tempstring.clear();
               /* output += "\n";
                output += value.toObject().value("info").toString();*/
                output += "\n";
            }
            output += "\n";
        }
        if (!neutral_creeps.isEmpty())
            output += "== Neutral Creeps ==\n";
        foreach (QJsonValue value, neutral_creeps)
        {
            output += "{{Unit label|";
            output += value.toObject().value("localized_name").toString();
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["neutral_creep_notes"].toArray();
            foreach (QJsonValue value2, temparray)
                {

                    if (value2.toObject().value("note").toString()!="<br>")
                    {
                        QString star = "* ";
                        QString start = "{{A|";
                        QString end = "}}";
                        QString mid = "|";
                        QString duo = ":" ;
                        texp = QRegExp("[\* \s]{2}([A-Za-z \s \']{1,25}):");
                        //qDebug() << texp;
                        tempstring = "* ";
                        tempstring += value2.toObject().value("note").toString();
                       // qDebug() << tempstring;
                        while(texp.indexIn(tempstring)!=-1)
                        {
                            //qDebug() << texp.cap(0) << texp.cap(1);
                            tempstring.replace(texp.cap(0), star + start + value.toObject().value("localized_name").toString() + mid  + texp.cap(1)+ end + duo);
                            break;
                        }
                        output += add_point(tempstring);
                        tempstring.clear();
                        output += "\n";
                    }

                }
            output += "\n";
           /* output += "\n";
            output += value.toObject().value("info").toString();*/
            //NEED TO ADD INFO ELEMENT OF JSON
        }
        if (!items.isEmpty())
            output += "== Items ==\n";
        foreach (QJsonValue value, items)
        {
            output += "{{Item label|";
            output += dict_items.value(value.toObject().value("ability_id").toInt());
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["ability_notes"].toArray();
            QRegExp texp;
            QString stringtexp;
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    QString star = "* ";
                    QString start = "{{A|";
                    QString end = "}}";
                    QString mid = "|";
                    QString duo = ":" ;
                    texp = QRegExp("[\* \s]{2}([A-Za-z \s \']{1,25}):");
                    //qDebug() << texp;
                    tempstring = "* ";
                    tempstring += value2.toObject().value("note").toString();
                   // qDebug() << tempstring;
                    while(texp.indexIn(tempstring)!=-1)
                    {
                        //qDebug() << texp.cap(0) << texp.cap(1);
                        tempstring.replace(texp.cap(0), star + start + dict_items.value(value.toObject().value("ability_id").toInt()) + mid + texp.cap(1) + end + duo);
                        break;
                    }
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }
            output += "\n";
        }
        if (!neutral_items.isEmpty())
            output += "== Neutral Items ==\n";
        foreach (QJsonValue value, neutral_items)
        {
            output += "{{Item label|";
            output += dict_items.value(value.toObject().value("ability_id").toInt());
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["ability_notes"].toArray();
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    QString star = "* ";
                    QString start = "{{A|";
                    QString end = "}}";
                    QString mid = "|";
                    QString duo = ":" ;
                    texp = QRegExp("[\* \s]{2}([A-Za-z \s \']{1,25}):");
                    //qDebug() << texp;
                    tempstring = "* ";
                    tempstring += value2.toObject().value("note").toString();
                   // qDebug() << tempstring;
                    while(texp.indexIn(tempstring)!=-1)
                    {
                        //qDebug() << texp.cap(0) << texp.cap(1);
                        tempstring.replace(texp.cap(0), star + start + dict_items.value(value.toObject().value("ability_id").toInt()) + mid + texp.cap(1) + end + duo);
                        break;
                    }
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }
            output += "\n";
        }
        if (!heroes.isEmpty())
            output += "== Heroes ==\n";
        QJsonArray heroes2;
        foreach (QJsonValue value, heroes)
        {
            QJsonObject temp =value.toObject();
            temp.insert("hero_name", dict_heroes.value(value.toObject().value("hero_id").toInt()));
            heroes2.append(temp);
        }

        std::sort(heroes2.begin(), heroes2.end(), [](const QJsonValue &v1, const QJsonValue &v2) {
            return v1.toObject()["hero_name"].toString() < v2.toObject()["hero_name"].toString();
        });

        foreach (QJsonValue value, heroes2)
        {
            output += "{{Hero label|";
            output += value.toObject().value("hero_name").toString();
            output += "}}\n";
            QJsonObject temp = value.toObject();
            QJsonArray temparray = value["hero_notes"].toArray();
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    output += "* ";
                    tempstring = value2.toObject().value("note").toString();
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }
            temparray = value["abilities"].toArray();
            foreach (QJsonValue value2, temparray)
            {
                output += "* {{A|";
                output += value.toObject().value("hero_name").toString();
                output += "|";
                output += dict_abilities.value(value2.toObject().value("ability_id").toInt());
                QJsonArray temparray2 = value2["ability_notes"].toArray();
                if (temparray2.size())
                {
                    output += "}}\n";
                    foreach (QJsonValue value3, temparray2)
                    {
                        if (value3.toObject().value("note").toString()!="<br>")
                        {
                            output += "** ";
                            tempstring = value3.toObject().value("note").toString();
                            output += add_point(tempstring);
                            tempstring.clear();
                            output += "\n";
                        }
                    }
                }
            }

            temparray = value["talent_notes"].toArray();
            if (!temparray.isEmpty())
                output += "* {{Symbol|Talent}} [[Talents]]\n";
            foreach (QJsonValue value2, temparray)
            {
                if (value2.toObject().value("note").toString()!="<br>")
                {
                    output += "** ";
                    tempstring = value2.toObject().value("note").toString();
                    output += add_point(tempstring);
                    tempstring.clear();
                    output += "\n";
                }
            }

            output += "\n";
        }
        if (output.back()=='\n')
            output.chop(1);
        output += QStringLiteral(u"&lt;/onlyinclude&gt;\n\n== See also ==\n* [[Versions]]\n* [[Patches]]\n\n{{SystemNav}}\n\n[[ru:Версия ") + version + "]]";
    }
    emit send_end();
}

void dwNetMacros::Parse_Animations()
{
    QString url  = version;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   // qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QUrl patch_url("https://dota2.fandom.com/wiki/"+ version + "/Animations?action=edit");
    QNetworkRequest patch_request(patch_url);

    QNetworkReply* patch_reply= manager->get(patch_request);
    send_progress(25);
    connect(patch_reply, SIGNAL(finished()),this,  SLOT(Do_Animations()));
    patch_url.clear();
   // patch_reply->close();
   // patch_reply->deleteLater();
    //manager->deleteResource(patch_request);
}

void dwNetMacros::Do_Animations()
{
    qDebug() << "Do_Animations called";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)

  {
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
    QRegExp rxlen("<textarea (.*) name=\"wpTextbox1\">(.*)<.textarea>");
    rxlen.indexIn(undercontent);
    output = QStringLiteral(u"{{DISPLAYTITLE:Анимации ") + version + "}}\n" + rxlen.cap(2) + "[[en:" + version + "/Animations]]";
    send_progress(50);
    Macros dwcase(output);
    dwcase.Animations();
    output = dwcase.first;
    dwcase.clearing();
    colour.clear();
    send_end();
  reply->deleteLater();
}
    else
    {
       send_progress(-1);
       qDebug() << "bad end";
     output = reply->errorString(); emit send_end();
}
}
void dwNetMacros::Parse_Cosmetic()
{
    QString url  = version;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   // qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QUrl patch_url("https://dota2.fandom.com/wiki/"+ version + "?action=edit");
    QNetworkRequest patch_request(patch_url);

    QNetworkReply* patch_reply= manager->get(patch_request);
    send_progress(25);
    connect(patch_reply, SIGNAL(finished()),this,  SLOT(Do_Cosmetic()));
    patch_url.clear();
   // patch_reply->close();
   // patch_reply->deleteLater();
    //manager->deleteResource(patch_request);
}

void dwNetMacros::Do_Cosmetic()
{
    qDebug() << "Do_Cosmetic called";
  QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

  if (reply->error() == QNetworkReply::NoError)

  {
    // Получаем содержимое ответа
      QByteArray content= reply->readAll();
      QString undercontent = QString(content);

   qDebug () << "f";
    //ui->text2->setPlainText(codec->toUnicode(content.data()));
    QRegExp rxlen("<textarea (.*) name=\"wpTextbox1\">(.*)<.textarea>");
    rxlen.indexIn(undercontent);
    output = rxlen.cap(2);
    send_progress(50);
    Macros dwcase(output);
    dwcase.Cosmetics();
    output = dwcase.first;
    dwcase.clearing();
    colour.clear();
    send_end();
  reply->deleteLater();
}
    else
    {
       send_progress(-1);
       qDebug() << "bad end";
     output = reply->errorString(); emit send_end();
}
}
inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

void dwNetMacros::send_progress(int pr)
{
    emit new_progress(pr);
}

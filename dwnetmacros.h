﻿#ifndef DWNETMACROS_H
#define DWNETMACROS_H
#include <QString>
#include <QObject>
#include <QWidget>
#include <QJsonObject>
#include <QSet>


class dwNetMacros : public QWidget
{
    Q_OBJECT
public:
    dwNetMacros();
    dwNetMacros(QString);
    dwNetMacros(QString, int);
    ~dwNetMacros();
    QString output = "";

    int language = 0;
    QString version = "";
    QSet <int> hero_list;

    void cleaning();
    void Patch_heroes();
    void Patch_items();
    void Patch_Version(int a);
    void end();

    QString add_point(QString);

    QJsonObject VersionJsonObj;
    QJsonObject ItemJsonObj;
    QJsonObject HeroJsonObj;
    QJsonObject AbilityJsonObj;


    void send_progress(int pr);
signals:
    void send_end();
    void new_progress(int);
private slots:
    void replyFinishedH();
    void replyFinishedI();
    void replyFinishedV();
    void replyFinishedV2();
    void replyFinishedV3();
    void replyFinishedItems();
    void replyFinishedHeroes();
    void replyFinishedVersion();
    void Do_Patch();
};

#endif // DWNETMACROS_H
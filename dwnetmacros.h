#ifndef DWNETMACROS_H
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
    QString colour;

    int language = 0;
    bool isFromPatch = false;
    QString version = "";
    QSet <int> hero_list;

    void cleaning();
    void Patch_heroes();
    void Patch_items();
    void Patch_Version(int a);
    void Parse_Animations();
    void Parse_Cosmetic();
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
    void Do_Animations();
    void Do_Cosmetic();
};

#endif // DWNETMACROS_H

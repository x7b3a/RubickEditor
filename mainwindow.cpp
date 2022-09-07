#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_settings.h"
#include <QString>
#include <QTextEdit>
#include <QApplication>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <QDesktopServices>
#include <QMap>
#include <QHash>
#include "qsswitch.h"
#include <QLabel>
#include <QSyntaxHighlighter>
#include <string>
#include <QRegExp>
#include <QClipboard>
#include <QStringList>
#include <QThread>
#include <QDesktopWidget>
#include <QtWinExtras/QWinTaskbarProgress>
#include <QtWinExtras/QWinTaskbarButton>
#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "html_parser.h"
#define RVERSION "1.0.8"
//#define snap

QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    adaptive_screen();
    set_fonts();
    QJsonObject json = read_json("config2.json").object();
    QJsonValue value = json.value(QString("Theme"));
    maintheme.theme = value.toInt();
    QJsonObject WikiAndFixes= json.value("WikiAndFixes").toObject();
    this -> showMaximized();
    this -> setWindowTitle("Rubick Editor " + QString(RVERSION));
    this -> setWindowIcon(QIcon(":/images/images/Rubick_icon.webp"));
    append_buttons();
    set_buttons();
    this->setStyleSheet("background-image:url()");
    set_theme();
    from = new ui_settings();

    connect(this, SIGNAL(sendData(QString)), from, SLOT(recieveData(QString)));  //пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅ Form1 пїЅ Form2
    connect(from, SIGNAL(sendData(QString)), this, SLOT(recieveData(QString)));  //пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅ Form2 пїЅ Form1

    set_progressbar();

    ui->backz->setVisible(false);
}

MainWindow::~MainWindow()
{
     QJsonObject recordObject;
     recordObject.insert("Theme",maintheme.theme);
     QFile file;
     QJsonArray stringArray;
     for (int i = 0; i<buttons.size();i++)
         stringArray.push_back(QJsonValue::fromVariant(buttons[i] -> text().toUtf8()));
     recordObject.insert("Macros",stringArray);
     QJsonDocument doc(recordObject);
     QString jsonString = doc.toJson(QJsonDocument::Indented);
     file.setFileName("config2.json");
     file.open(QIODevice::WriteOnly | QIODevice::Text);
     QTextStream stream( &file );
     stream << jsonString;
           file.close();
    delete ui;
}


void MainWindow::on_buttochange_clicked()
{
   QString first = ui -> text1 -> toPlainText();
   QString second = ui -> text2 -> toPlainText();
   ui -> text1->setText(second);
   ui -> text2->setText(first);
}
void MainWindow::on_settings_clicked()
{
    from ->show();
    from->activateWindow();
}

void MainWindow::recieveData(QString q)
{
    set_buttons();
}

void MainWindow::on_dota2wiki_clicked()
{
    QString wikiurl = "https://dota2.fandom.com/ru/wiki/\u0423\u0447\u0430\u0441\u0442\u043d\u0438\u043a:X7b3a2j/Rubick_Editor";
    QDesktopServices::openUrl(QUrl(wikiurl));
}

void MainWindow::on_buttonpaste_clicked()
{
    QClipboard* pcb = QApplication::clipboard();
    ui->text1->setText(pcb->text());
}

void MainWindow::on_buttoncopy_clicked()
{
    QClipboard* pcb = QApplication::clipboard();
    pcb -> setText(ui->text1->toPlainText());
}

void MainWindow::button_switch(QString switchStr)
{
    ui->error->setStyleSheet("color: rgba(255,0,0,0);");
    ui->debug->clear();
#ifdef snap
    if (snapshot.isEmpty())
        snapshot.append(dwSnapshot(ui -> text1-> toPlainText(), ui -> text2-> toPlainText()));
#endif
    QSSWITCH(switchStr,
                QSCASE(cases[0], //пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅ пїЅпїЅпїЅпїЅпїЅ
                {
                    WikiAndFixes();break;
                })
                QSCASE(cases[1], //пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ пїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅ
                {
                    Commafix();break;
                })
                QSCASE(cases[2],//"пїЅпїЅпїЅпїЅпїЅпїЅ "пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ"
                {
                   Changelogs();break;
                })
                QSCASE(cases[3],//"пїЅпїЅпїЅпїЅпїЅпїЅ "пїЅпїЅпїЅпїЅпїЅпїЅпїЅ"
                {
                    Responses();break;
                })
                QSCASE(cases[4],// "пїЅпїЅпїЅпїЅпїЅпїЅ "пїЅпїЅпїЅпїЅпїЅ"
                {
                    Sounds();break;
                })
                QSCASE(cases[5],//"пїЅпїЅпїЅпїЅпїЅпїЅ "пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ"
                {
                   Cosmetics();break;
                })
                QSCASE(cases[6],//"Units - пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ"
                {
                   Units();break;
                })
                QSCASE(cases[7],
                {
                    Patch_heroes();break;
                })
                QSDEFAULT(
                {
                   ui -> error->setStyleSheet("color: rgba(255,0,0,255);");
                   ui -> text2->setText("dw error = \u043a\u043d\u043e\u043f\u043a\u0430 \u0441 \u0442\u0430\u043a\u0438\u043c \u043d\u0430\u0437\u0432\u0430\u043d\u0438\u0435\u043c\n \u043d\u0435 \u043d\u0430\u0439\u0434\u0435\u043d\u0430");break;
                })
                )
#ifdef snap
        if (snapshot.size()>9)
            snapshot.removeFirst();
        snapshot.append(dwSnapshot(ui -> text1-> toPlainText(), ui -> text2-> toPlainText()));
        snapshot_iterator = snapshot.size();
#endif
}


void MainWindow::WikiAndFixes()
{
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
    qDebug() << letter_with_for_medoke[0].toString() << letter_with_for_medoke[1].toString();
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
}

void MainWindow::Commafix()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString comma = color(",");
    QString point = ".";
    QString first = get_text();
    progress->setValue(10);
    for (int j = 0;j<10;j++)
        for (int k = 0;k<10;k++)
            first.replace(QString::number(j) + point + QString::number(k),QString::number(j) + comma + QString::number(k));
    progress->setValue(20);
    QJsonObject json = read_json("dict.json").object();
    QJsonArray Commafix = json["Commafix"].toArray();
        // (7.23d-7.24e)
    QRegExp texp = QRegExp("\\(([0-9])"+ comma +"([0-9abcdefgh]{2,3}-[0-9])"+comma+"([0-9abcdefgh]{2,3})\\)");
    while(texp.indexIn(first)!=-1 )
    {
         qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
         first.replace(texp.cap(0),"(" + texp.cap(1) + point + texp.cap(2)+ point + texp.cap(3)+ ")");

    }
    progress->setValue(30);
        // (7.23d)
    texp = QRegExp("\\(([0-9])" + comma + "([0-9abcdefgh]{2,3})\\)");
    while(texp.indexIn(first)!=-1 )
    {
         qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
         first.replace(texp.cap(0),"(" + texp.cap(1) + point + texp.cap(2) + ")");

    }
        // {{cf|At|2.28|2.31d}}
    progress->setValue(40);
    texp = QRegExp(("f\\|([Aa])t\\|([0-9])"+ comma +"([0-9abcdefgh]{2,3}\\|[0-9])"+comma+"([0-9abcdefgh]{2,3})\\}\\}"));
    while(texp.indexIn(first)!=-1 )
    {
         qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
         first.replace(texp.cap(0),"f|" + texp.cap(1) + "t|" + texp.cap(2) + point + texp.cap(3) + point + texp.cap(4) + "}}");

    }
     progress->setValue(50);
    for (int i = 0; i<Commafix.size();i++)
    {
        for (int j = 0;j<10;j++)
            for (int k = 0;k<10;k++)
                first.replace(Commafix[i].toString()+QString::number(j) + comma + QString::number(k),Commafix[i].toString()+QString::number(j) + "."+ QString::number(k));
    }
    progress->setValue(65);
    progress->setValue(85);
    progress->setValue(100);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}


void MainWindow::Changelogs()
{
    QString first = get_text();
    QJsonObject json = read_json("dict.json").object();
    QMap<QString, QString>::iterator i,j,k;
    QJsonValue value = json.value(QString("Changelogs"));
    QJsonObject item = value.toObject();
    QString space = QJsonValue(item["Space"]).toString();
    QString ability = QJsonValue(item["Ability"]).toString();
    QString Float = QJsonValue(item["Float"]).toString();
    QString changed = " changed:";
    QString eachlevel = " on each level";
    QString changed_rus = QJsonValue(item[changed]).toString();
    QString eachlevel_rus = QJsonValue(item[eachlevel]).toString();
    QString Level_numbers = QJsonValue(item["Level_numbers"]).toString();
    QString base = "base";
    QString gain = "gain";
    QString from = "from";
    QString point = ".";
    QString proc = "%";
    QString to = "to";
    QString sec = " \u0441\u0435\u043a.";
    QString minus = "\u2212";
    QString arrow = "\u279c";
    QMap<QString, QString> Keywords = map_parser(item,"Keywords");
    QMap<QString, QString> Preposition = map_parser(item,"Preposition");
    QMap<QString, QString> Attributes = map_parser(item,"Attributes");
    QMap<QString, QString> Attributes_base = map_parser(item,"Attributes_base");
    QMap<QString, QString> Attributes_flex = map_parser(item,"Attributes_flex");
    QJsonArray Flex_verb = item["Flex_verb"].toArray();
    QJsonArray Flex_adj = item["Flex_adj"].toArray();
    QJsonObject Stats = item["Stats"].toObject();
        QMap<QString, QString> Stats_d = map_parser(Stats, "duration");
        QMap<QString, QString> Stats_d_n = map_parser(Stats, "duration_n");
        QMap<QString, QString> Stats_f = map_parser(Stats, "female");
        QMap<QString, QString> Stats_m = map_parser(Stats, "male");
        QMap<QString, QString> Stats_n = map_parser(Stats, "neuter");
    QMap<QString, QString> Talents = map_parser(item,"Talents");
    QMap<QString, QString> Anti_Irismus_Talents_level = map_parser(item,"Anti_Irismus_Talents_level");
    QMap<QString, QString> Talents_level = map_parser(item,"Talents_level");
    QMap<QString, QString> Talents_changes = map_parser(item, "Talents_changes");
    QMap<QString, QString> Talents_changes_n = map_parser(item,"Talents_changes_n");
    QMap<QString, QString> Talents_changes_proc = map_parser(item,"Talents_changes_proc");
    QMap<QString, QString> Keywords_small = map_parser(item,"Keywords_small");
    QMap<QString, QString> Aghanim = map_parser(item, "Aghanim");
    QMap<QString, QString> Keywords_cooldown = map_parser(item,"Keywords_cooldown");
    QMap<QString, QString> Cooldown = map_parser(item, "Cooldown");
    QMap<QString, QString> Talents_abilities = map_parser(item,"Talents_abilities");
    QMap<QString, QString> Talents_abilities_proc = map_parser(item,"Talents_abilities_proc");
    QJsonObject Abilities_one = item["Abilities_one"].toObject();
    QMap<QString, QString> Abilities_one_d = map_parser(Abilities_one,"duration");
    QMap<QString, QString> Abilities_one_f = map_parser(Abilities_one,"female");
    QMap<QString, QString> Abilities_one_m = map_parser(Abilities_one,"male");
    QMap<QString, QString> Abilities_one_n = map_parser(Abilities_one,"neuter");
    QMap<QString, QString> New_Talent = map_parser(item,"New_Talent");
    QMap<QString, QString> New_Talent_abilities = map_parser(item,"New_Talent_abilities");
    QMap<QString,QString> New_ability = map_parser(item,"New_ability");
    QMap<QString,QString> Other_last = map_parser(item,"Other_last");
    progress->setValue(1);
    first = start_regular_replacer(first);
    for (i=Aghanim.begin();i!=Aghanim.end();i++)
    {
        first.replace(start_regular_replacer(i.key()),color(i.value()));
    }
    for (i = Anti_Irismus_Talents_level.begin(); i != Anti_Irismus_Talents_level.end(); i++)
    {
        first.replace(i.key(),color(i.value()));
    }
    for (i = Talents_level.begin(); i != Talents_level.end(); i++)
    {
        first.replace(i.key() + changed,color(i.value()+changed_rus));
        first.replace(i.key(),color(i.value()));
    }
    for (i = Talents.begin(); i != Talents.end(); i++)
    {
        first.replace(start_regular_replacer(i.key()),color(i.value()));
    }
    progress->setValue(10);
    QString temp1;
    QString temp2;
    QRegExp texp;
    int iter = 0;
    for (i=Keywords_small.begin(); i!= Keywords_small.end();i++,progress->setValue(progress->value()+15)) //increased
    {
        for (j = Talents_changes.begin();j!=Talents_changes.end();j++)
        {
            temp1 = Float+space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float + "\\." ;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                iter++;
                 qDebug() <<"while:"<< QString::number(iter)<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1).replace("+","") + space + Preposition.value(to)+ space + texp.cap(5).replace("+",""))+point);

            }
        }
        for (j=Talents_changes_proc.begin();j!=Talents_changes_proc.end();j++)
        {
            temp1 = Float + "%" +space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float+"%\\." ;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1).replace("+","") +proc+ space + Preposition.value(to)+ space + texp.cap(5).replace("+",""))+proc+point);
            }
        }
        for (j = Talents_abilities.begin();j!=Talents_abilities.end();j++)
        {
             temp1  =   Float + space + ability + space + j.key() + space + i.key() + space + to + space + Float + point;
             texp = QRegExp(temp1);
             while(texp.indexIn(first)!=-1 )
             {
                first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("+","") + space + Preposition.value(to)+ space  + texp.cap(7).replace("+","")+point));
             }
         }
        for (j=Talents_abilities_proc.begin();j!=Talents_abilities_proc.end();j++)
        {
            temp1 = Float + "%" +space + ability + space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float+"%\\." ;
            texp = QRegExp(temp1);
             while(texp.indexIn(first)!=-1 )
            {
                qDebug() << " ability proc:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+Flex_verb[0].toString()+ space+ Preposition.value(from) + space + texp.cap(1).replace("+","") + proc+ space  + Preposition.value(to)+ space + texp.cap(7).replace("+","")+proc+point));
            }
        }
    }


    for (i=Keywords_cooldown.begin();i!=Keywords_cooldown.end();i++,progress->setValue(progress->value()+10)) //increased as "ycileno"
    {
        for (j = Cooldown.begin();j!= Cooldown.end();j++)
        {
            temp1  =   Float +"s"+ space + ability + space + j.key() + space + i.key() + space + to + space + Float + "s.";
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
              //  qDebug() << " cooldown:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("-",minus)+sec + space + Preposition.value(to)+ space  + texp.cap(7).replace("-",minus)+sec));
            }

            temp1  =   Float +"s"+ space + start_regular_replacer(j.key()) + space + i.key() + space + to + space + Float + "s.";
           // qDebug() << "temp1:" <<temp1;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                first.replace(texp.cap(0),color(j.value() + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("-",minus) + space + Preposition.value(to)+ space  + texp.cap(5).replace("-",minus)+sec));
            }
        }
        for (j=Talents_changes_n.begin();j!=Talents_changes_n.end();j++)
        {
            temp1 = Float+space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float + "\\." ;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1).replace("-",minus) +proc+ space + Preposition.value(to)+ space + texp.cap(5).replace("-",minus))+point);
            }
        }
    }
    int count;
    for (i=Keywords.begin(); i!= Keywords.end();i++,progress->setValue(progress->value()+10)) //Increased
    {
        for (j=Attributes_base.begin(),count = 0;j!=Attributes_base.end();j++,count++)
        {
            temp1 = i.key()+space+base+space+start_regular_replacer(j.key() + space + from);
            temp2 = Attributes.value(base)+Flex_adj[count<2?0:1].toString()+space+j.value()+space+i.value()+Flex_verb[count<2?0:1].toString();
            first.replace(temp1,color(temp2) + space + from);
        }
        for (j=Attributes_flex.begin();j!=Attributes_flex.end();j++)
        {
            temp1 = i.key()+space+start_regular_replacer(j.key())+space+gain+space;
            temp2 = Attributes.value(gain)+space+j.value()+space+i.value()+space;
            first.replace(temp1,color(temp2));
        }
        for (j=Stats_d.begin();j!=Stats_d.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key()) + from + space + Level_numbers;
            texp = QRegExp(temp1);
            //qDebug() << "temp1" << temp1 <<"\n" <<texp;

            temp2 = j.value()+i.value()+Flex_verb[1].toString()+ space;
            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " duration:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                first.replace(texp.cap(0),color(temp2 + Preposition.value(from) + space + texp.cap(1)+ texp.cap(2).replace(" on each level",eachlevel_rus)  + Preposition.value(to)+ space + texp.cap(3) +QString(sec + texp.cap(4).replace(" on each level",eachlevel_rus)).replace("..",".")));
            }
        }
        for (j=Stats_d_n.begin();j!=Stats_d_n.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key()) + from + space + Level_numbers;
            texp = QRegExp(temp1);
            //qDebug() << "temp1" << temp1 <<"\n" <<texp;

            temp2 = j.value()+i.value()+Flex_verb[2].toString()+ space;
            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " duration:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                first.replace(texp.cap(0),color(temp2 + Preposition.value(from) + space + texp.cap(1)+ texp.cap(2).replace(" on each level",eachlevel_rus)  + Preposition.value(to)+ space + texp.cap(3) +QString(sec + texp.cap(4).replace(" on each level",eachlevel_rus)).replace("..",".")));
            }
        }
        for (j=Stats_n.begin();j!=Stats_n.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key()) + from;
            temp2 = j.value()+i.value()+Flex_verb[2].toString()+ space;
            first.replace(temp1,color(temp2) + from);
        }
        for (j=Stats_f.begin();j!=Stats_f.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key()  + from);
            temp2 = j.value()+i.value()+Flex_verb[1].toString()+ space;
            first.replace(temp1,color(temp2)  + from);
        }
        for (j=Stats_m.begin();j!=Stats_m.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key()  + from);
            temp2 = j.value()+i.value()+Flex_verb[0].toString()+ space;
            first.replace(temp1,color(temp2) + from);
        }
        for (j=Abilities_one_d.begin();j!=Abilities_one_d.end();j++)
        {
            temp1 = i.key() + space + ability + j.key() + from + space + Level_numbers;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color("{{A|"+ texp.cap(1)+ "|"+texp.cap(2)+"}}: " +j.value()+ i.value()+Flex_verb[1].toString()+ space+ Preposition.value(from) + space + texp.cap(3)+ texp.cap(4).replace(" on each level",eachlevel_rus)  + Preposition.value(to)+ space + texp.cap(5) +QString(sec + texp.cap(6).replace(" on each level",eachlevel_rus)).replace("..",".")));
            }
         }
        for (j=Abilities_one_f.begin();j!=Abilities_one_f.end();j++)
        {
            temp1 = i.key() + space + ability + j.key() + from;
            texp = QRegExp(temp1);
           // qDebug() << "abil" << temp1;

            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color("{{A|"+ texp.cap(1)+ "|"+texp.cap(2)+"}}: " +j.value()+ i.value()+Flex_verb[1].toString()+ space) + from);
            }
         }
        for (j=Abilities_one_m.begin();j!=Abilities_one_m.end();j++)
        {
            temp1 = i.key() + space + ability + j.key() + from;
            texp = QRegExp(temp1);
            //qDebug() << "abil" << temp1;

            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color("{{A|"+ texp.cap(1)+ "|"+texp.cap(2)+"}}: " +j.value()+ i.value()+ space) + from);
            }
         }
        for (j=Abilities_one_n.begin();j!=Abilities_one_n.end();j++)
        {
            temp1 = i.key() + space + ability + j.key() + from;
            texp = QRegExp(temp1);
            qDebug() << "abil" << temp1;

            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color("{{A|"+ texp.cap(1)+ "|"+texp.cap(2)+"}}: " +j.value()+ i.value()+Flex_verb[2].toString()+ space) + from);
            }
         }
    }
    progress->setValue(90);
    QString fromto = from+ space + "([0-9/\\.\\-,\u2012,%]{1,20})( on each level |.)to ([0-9/\\.\\-,\u2012,%]{1,20})( on each level.|\\.)";
    texp = QRegExp(fromto);
    while(texp.indexIn(first)!=-1 )
    {
         first.replace(texp.cap(0),color(Preposition.value(from) + space + texp.cap(1)+ texp.cap(2).replace(" on each level",eachlevel_rus)  + Preposition.value(to)+ space + texp.cap(3) + texp.cap(4).replace(" on each level",eachlevel_rus)));
    }
    for (i=New_Talent.begin();i!=New_Talent.end();i++)
    {
        temp1 = " ([0-9/\\.\\-\\+,%]{1,10})" + space + start_regular_replacer(i.key());
        texp = QRegExp(temp1);
        //qDebug() << "tal" << temp1 << texp;
        while(texp.indexIn(first)!=-1 )
        {
            qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
            first.replace(texp.cap(0),color(space + texp.cap(1)+space + i.value()));
        }
    }
    for (i=New_Talent_abilities.begin();i!=New_Talent_abilities.end();i++)
    {
        temp1 = " ([0-9/\\.\\-\\+,%s]{1,10}) "  + ability + start_regular_replacer(i.key());
        texp = QRegExp(temp1);
        qDebug() << "tal" << temp1 << texp;
        while(texp.indexIn(first)!=-1 )
        {
            qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
            first.replace(texp.cap(0),color(space + texp.cap(1).replace("s","").replace("-",minus) + i.value() +space+ " {{A|" + texp.cap(2)+"|"+ texp.cap(3) + "}}"));
        }
    }
    progress->setValue(93);
    for (i = New_ability.begin();i!= New_ability.end();i++)
    {
        first.replace("::" + i.key() + ":",":: " + color(i.value() + ":"));
        first.replace(":: " + i.key() + ":",":: " + color(i.value() + ":"));
    }
    progress->setValue(96);
    for (i=Other_last.begin();i!=Other_last.end();i++)
    {
        first.replace(i.key(), color(i.value()));
    }

        qDebug() << "end??";
    progress->setValue(100);
    end_regular_replacer (&first);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}

void  MainWindow::Responses()
{
    progress->setValue(1);
    QString space = " ";
    QString proc = "%";
    QString DInt = "([1-9][0-9])";
    QString Int = "([1-9])";
    QString chance = "\u0428\u0430\u043d\u0441 ";
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Responses").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    progress->setValue(33);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    QString temp;
    QRegExp texp;
    temp = DInt + "% chance";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(chance + texp.cap(1)+ proc));
    }
    temp = Int + "% chance";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(chance + texp.cap(1)+ proc));
    }
    progress->setValue(66);
    temp = Int + " seconds cooldown";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color("\u041f\u0435\u0440\u0435\u0437\u0430\u0440\u044f\u0434\u043a\u0430 " + texp.cap(1)+ (texp.cap(1).toInt()>4?" \u0441\u0435\u043a\u0443\u043d\u0434":texp.cap(1).toInt()>1?" \u0441\u0435\u043a\u0443\u043d\u0434\u044b":" \u0441\u0435\u043a\u0443\u043d\u0434\u0430")));
    }
    progress->setValue(100);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}


void  MainWindow::Sounds()
{
    progress->setValue(1);
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Sounds").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    progress->setValue(50);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    progress->setValue(100);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}

void  MainWindow::Cosmetics()
{
    progress->setValue(1);
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Cosmetics").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    progress->setValue(50);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    progress->setValue(100);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}

void  MainWindow::Units()
{
    progress->setValue(1);
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Units").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    progress->setValue(50);
    for (i = dict.begin(); i != dict.end(); i++)
    {
        first.replace ("{{Show|A|" + i.key()+"|", color("{{Show|A|" + i.value()+"|"));
        first.replace("|" + i.key()+"}}",color("|" + i.value()+"}}"));
        first.replace("|" + i.key()+"|text",color("|" + i.value())+"|text");
        //first.replace(i.key(), color(i.value()));
    }
    progress->setValue(100);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
    progress->setValue(0);
}

void MainWindow::Patch_heroes()
{
    progress->setValue(1);
    QString url  = get_text();
    QJsonObject json = read_json("dict.json").object();
    QJsonArray Commafix = json["Heroes"].toArray();
    QString output;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
     QUrl patch_url("https://www.dota2.com/datafeed/patchnotes?version=7.32&language=russian");
     QUrl heroes_url("https://www.dota2.com/datafeed/herolist?language=russian");
    QNetworkRequest patch_request(patch_url);
     QNetworkRequest heroes_request(heroes_url);


    QNetworkReply* patch_reply=  manager->get(patch_request);
    QNetworkReply* heroes_reply=  manager->get(heroes_request);
    QString temp;
    connect(patch_reply, SIGNAL(finished()),this,  SLOT(replyFinished()));
    put_text(temp);
    qDebug() << "connect?";
}

QString MainWindow::replyFinished()

{

  QNetworkReply *reply=

    qobject_cast<QNetworkReply *>(sender());



  if (reply->error() == QNetworkReply::NoError)

  {

    // Получаем содержимое ответа

    QByteArray content= reply->readAll();



    // Реализуем преобразование кодировки

    // (зависит от кодировки сайта)

    QTextCodec *codec = QTextCodec::codecForName("utf8");



    // Выводим результат
   // qDebug() << codec->toUnicode(content.data());
    //ui->text2->setPlainText(codec->toUnicode(content.data()) );
    *patch_r = codec->toUnicode(content.data());

  }

  else

  {
    // Выводим описание ошибки, если она возникает.
    ui->text2->setPlainText(reply->errorString());
  }
  // разрешаем объекту-ответа "удалится"
  reply->deleteLater();

}
void MainWindow::set_theme()
{

    maintheme.size =this->size();
    QString highlight = maintheme.get_highlight();
    QString text_back = maintheme.get_backcolor();
    QString button_back = maintheme.get_buttoncolor();
    QString texttext = maintheme.get_textcolor();
    QString text = maintheme.get_buttontextcolor();
    QString details = maintheme.get_details();
    QPalette palette = maintheme.get_backimage();
    QString background;
    QString border = maintheme.get_border();
    QString themetext = maintheme.get_themetext();
    QString btext = "border-top-color: rgb(127, 127, 127);border-top-width: 1px; border-top-style: solid;border-left-color: rgb(127, 127, 127);border-left-width: 1px;border-left-style: solid;\
            border-bottom-color: rgb(127, 127, 127);\
            border-bottom-width: 1px;\
                border-bottom-style: solid;\
                border-right-width: 12px;\
                border-right-style: outset;\
                border-bottom-right-radius: 3px;";
                    qDebug() << maintheme.theme;


                this->setPalette(palette);

                this->show();
                 for (int i = 0;i<buttons.size();i++)
        buttons[i] ->setStyleSheet(btext + "background-color: rgb" + button_back +";\
                color: rgb" + text +";\
                 border-right-color: rgb" + details + ";");
        ui -> changes -> setStyleSheet("color: rgb" + text + ";");
        QString dtext = "background-color: rgba(255, 255, 255,0);\
                border: none;\
                color: rgb";
                ui->Input_win->setStyleSheet(dtext+details+";");
                ui->Output_win->setStyleSheet(dtext+details+";");
                ui->Input_label->setStyleSheet(dtext+details+";");
                ui->Output_label->setStyleSheet(dtext+details+";");
                ui->themebutton -> setStyleSheet(dtext+details+";");
                ui->themelabel -> setStyleSheet(dtext+details+";");
        QString bback = "border-top-width: 1px;\
                    border-top-style: solid;\
                border-left-width: 1px;\
                    border-left-style: solid;\
                border-bottom-width: 1px;\
                    border-bottom-style: solid;\
                    border-right-width: 1px;\
                    border-right-style: solid; background-color: rgb";
         ui->text1 -> setStyleSheet(bback + text_back + ";\
 color: rgb"+texttext +"; border-color: rgb" + border + ";");
         ui->text2 -> setStyleSheet(bback + text_back + ";\
                                    color: rgb"+texttext +"; border-color: rgb" + border + ";");
         ui -> buttonpaste ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttonpaste_2 ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttoncopy ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttoncopy_2->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttochange ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         qDebug()<< "border-color: rgb" + details + "; border-width: 4px; border-radius: 4px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";";
         if (autoz)
            ui -> autozamena->setStyleSheet("border-color: rgb" + details + "; border-width: 1px;\n    border-style: solid  ; border-width: 4px; border-radius: 4px;\n    border-style: solid;background-color: rgb" + button_back + "; color: rgb" + text + ";");
         else
             ui->autozamena->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;background-color: rgb" + button_back + "; color: rgb" + text + ";");
         //ui -> refreshButton ->setStyleSheet("nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> settings ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> Clear_left ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> Clear_right ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui->themebutton->setText(themetext);
         ui->debug->setStyleSheet("color: rgb"+text+";");

}

QMap<QString, QString> MainWindow::map_parser(QJsonObject item, QString word)
{
    QMap<QString, QString> array;
    QJsonObject keywords_value = item[word].toObject();
    foreach(const QString& key, keywords_value.keys()) {
        QJsonValue value = keywords_value.value(key);
        array.insert(key,value.toString());
    }
    return array;
}
QString MainWindow::start_regular_replacer (QString temp)
{
    QString Array[5] = {"|", "{", "}", "[", "]"};
    int i;
    for (i=0; i<5;i++)
         temp.replace(Array[i],"&dw0"+QString::number(i)+"c");
    return temp;
}
void MainWindow::end_regular_replacer (QString *temp)
{
    QString Array[5] = {"|", "{", "}", "[", "]"};
    int i;
    for (i=0; i<5;i++)
        &temp ->replace("&dw0"+QString::number(i)+"c",Array[i]);
}
QString MainWindow::get_text()
{
    QString first;
    if (!input)
        first = ui -> text1-> toPlainText();
    else
        first = ui -> text2-> toPlainText();
    first.replace("<", "&lt;");
    first.replace(">", "&gt;");
    return first;
}

QString MainWindow::get_backtext()
{
   return QString("<span style= \"background:");
}

QString MainWindow::color(QString arg,QString color)
{
    color = maintheme.get_highlight();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //РёР·РјРµРЅРµРЅРёСЏ
    return QString("<span style= \"background:%1\">%2</span>").arg(color,arg);
}

int MainWindow::counter(QString text)
{
    QRegExp found(get_backtext());
    return text.count(found);
}

void MainWindow::put_text(QString text)
{
    //qDebug() << text;
    text.replace("\n","<br>");
    text = "<html>" + text + "</html>";
    //qDebug() << text;
    if (output==input||!autoz)
    {
        if (!output)
            ui->text1 ->setText(text);
        else
            ui -> text2 -> setText(text);
    }
    else if (output!=input&&autoz)
    {
        if (!input)
        {
            ui->text2 ->setPlainText(get_text());
            ui->text1 ->setText(text);
        }

        else
        {
            ui -> text1 ->setPlainText(get_text());
            ui -> text2 -> setText(text);
        }

    }

}

QJsonDocument MainWindow::read_json(QString filename)
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
            ui->error->setStyleSheet("color: rgba(255,0,0,255);");
            ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+filename+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
        }
        return Doc;
}

void MainWindow::append_buttons()
{
    buttons.append(ui->button1);
    buttons.append(ui->button2);
    buttons.append(ui->button3);
    buttons.append(ui->button4);
    buttons.append(ui->button5);
}

void MainWindow::set_buttons()
{
    QJsonObject json = read_json("config2.json").object();
    QJsonArray Macros = json["Macros"].toArray();
    for (int i = 0;i<buttons.size();i++)
        buttons[i] ->  setText(Macros[i].toString());
}

void MainWindow::label_settext(int count)
{
    int ends=0;
    QString endstring = "\u0438\u0437\u043c\u0435\u043d\u0435\u043d\u0438\u0439";
    if (count%10==1&&count!=11)
        ends = 1;
    if (count%10>1&&count%10<5&&(count<11||count>15))
        ends = 2;
    switch (ends)
    {
        case 1: endstring = "\u0438\u0437\u043c\u0435\u043d\u0435\u043d\u0438\u0435";break;
        case 2: endstring = "\u0438\u0437\u043c\u0435\u043d\u0435\u043d\u0438\u044f";break;
        default: break;
    }
    QString text = "\u0421 \u043f\u043e\u0441\u043b\u0435\u0434\u043d\u0438\u043c \u043c\u0430\u043a\u0440\u043e\u0441\u043e\u043c \u0441\u043e\u0432\u0435\u0440\u0448\u0435\u043d\u043e ";
    text +=  + "<font color=\"red\">" + QString::number(count) +"</font>" + " " + endstring;
    ui -> changes -> setText(text);
}

void MainWindow::on_button1_clicked()
{
     QString switchStr = ui -> button1 -> text().toUtf8();
     button_switch(switchStr);
}
void MainWindow::on_button2_clicked()
{
    QString switchStr = ui -> button2 -> text().toUtf8();
    button_switch(switchStr);
}
void MainWindow::on_button3_clicked()
{
    QString switchStr = ui -> button3 -> text().toUtf8();
    button_switch(switchStr);
}
void MainWindow::on_button4_clicked()
{
    QString switchStr = ui -> button4 -> text().toUtf8();
    button_switch(switchStr);
}
void MainWindow::on_button5_clicked()
{
    QString switchStr = ui -> button5 -> text().toUtf8();
    button_switch(switchStr);
}

void MainWindow::on_Input_win_clicked()
{
    double normw = 1920;
    double normh = 1080;
    double wide = QApplication::desktop()->screenGeometry().width()/normw;
    double hide = QApplication::desktop()->screenGeometry().height()/normh;

    input = input?0:1;
    int x = (ui -> Input_label->mapToGlobal(QPoint(0,0))).x();
    int width = ui->Input_label->geometry().size().width();
    ui->Input_label ->setGeometry(input?x+width:x-width,-50*hide, 85*wide,111*hide);
    ui->Input_win->setText(input?"\u0412\u0432\u043e\u0434: \u043e\u043a\u043d\u043e \u0441\u043f\u0440\u0430\u0432\u0430":"\u0412\u0432\u043e\u0434: \u043e\u043a\u043d\u043e \u0441\u043b\u0435\u0432\u0430");
    qDebug() << input;

}

void MainWindow::on_Output_win_clicked()
{
    double normw = 1920;
    double normh = 1080;
    double wide = QApplication::desktop()->screenGeometry().width()/normw;
    double hide = QApplication::desktop()->screenGeometry().height()/normh;
    output = output?0:1;
    int x = (ui -> Output_label->mapToGlobal(QPoint(0,0))).x();
    int width = ui->Output_label->geometry().size().width();
    ui->Output_label ->setGeometry(output?x+width:x-width,-50*hide, 85*wide,111*hide);
    ui->Output_win->setText(!output?"\u0412\u044b\u0432\u043e\u0434: \u043e\u043a\u043d\u043e \u0441\u043b\u0435\u0432\u0430":"\u0412\u044b\u0432\u043e\u0434: \u043e\u043a\u043d\u043e \u0441\u043f\u0440\u0430\u0432\u0430");
    qDebug() << output;
}

void MainWindow::on_themebutton_clicked()
{
    //put_text("lights up");
    maintheme.theme = maintheme.theme?0:1;
    set_theme();
}

void MainWindow::on_Clear_left_clicked()
{
    ui->text1->clear();
}

void MainWindow::on_Clear_right_clicked()
{
    ui->text2->clear();
}

void MainWindow::on_buttonpaste_2_clicked()
{
    QClipboard* pcb = QApplication::clipboard();
    ui->text2->setText(pcb->text());
}

void MainWindow::on_buttoncopy_2_clicked()
{
    QClipboard* pcb = QApplication::clipboard();
    pcb -> setText(ui->text2->toPlainText());
}

void MainWindow::on_excel_clicked()
{
    QString wikiurl = "https://docs.google.com/spreadsheets/d/1F0ehop88qvcn6qmvXwP-4C0gIL3IWG9_ezVUCZ-xOho/edit?usp=sharing";
    QDesktopServices::openUrl(QUrl(wikiurl));
}

void MainWindow::on_discord_clicked()
{
    QString wikiurl = "https://discord.com/channels/576585892937072640/577076136870281246";
    QDesktopServices::openUrl(QUrl(wikiurl));
}
 void MainWindow::adaptive_screen()
 {
     double normw = 1920;
     double normh = 1080;
     double wide = QApplication::desktop()->screenGeometry().width()/normw;
     double hide = QApplication::desktop()->screenGeometry().height()/normh;
     QWidget *test;

     for (int i = 0;i<29;i++)
     {
         test = centralWidget()->findChild<QWidget*>(centralWidget()->children().at(i)->objectName());
         QRect cords = test->geometry();
         test->setGeometry(int(cords.x()*wide),int(cords.y()*hide),int(cords.width()*wide),int(cords.height()*hide));
     }
 }
 void MainWindow::set_fonts()
 {
     double normw = 1920;
     double normh = 1080;
     double wide = QApplication::desktop()->screenGeometry().width()/normw;
     double hide = QApplication::desktop()->screenGeometry().height()/normh;
     qDebug() << wide << hide;
     int id = QFontDatabase::addApplicationFont(":/fonts/Reaver-Regular.ttf");
     QString reaver = QFontDatabase::applicationFontFamilies(id).at(0);
     wide = hide;
     QFont button_font(reaver,12*wide);
     QFont bottom_button_font(reaver,10*wide);
     QFont clear_button_font(reaver,9*wide);
     id = QFontDatabase::addApplicationFont(":/fonts/consola.ttf");
     QString consola = QFontDatabase::applicationFontFamilies(id).at(0);
     QFont text_font(consola,10);
     centralWidget()->setFont(button_font);
     for (int i = 0; i<buttons.size();i++)
         buttons[i]->setFont(button_font);
     ui->settings->setFont(button_font);
     ui->buttochange->setFont(button_font);
     ui->changes->setFont(QFont(reaver, 15*wide));
     ui->buttoncopy->setFont(bottom_button_font);
     ui->buttoncopy_2->setFont(bottom_button_font);
     ui->buttonpaste->setFont(bottom_button_font);
     ui->buttonpaste_2->setFont(bottom_button_font);
     ui->Clear_left->setFont(clear_button_font);
     ui->Clear_right->setFont(clear_button_font);
     ui->Input_win->setFont(QFont(reaver,12*wide,75));
     ui->Output_win->setFont(QFont(reaver,12*wide,75));
     ui->themebutton->setFont(QFont(reaver,12*wide,75));
     ui->text1->setFont(text_font);
     ui->text2->setFont(text_font);
     ui->autozamena->setFont(button_font);
 }
void MainWindow::set_progressbar()
{
    QWinTaskbarButton *tuskbar;
    tuskbar = new QWinTaskbarButton(this);
    tuskbar->setWindow(windowHandle());
    progress = tuskbar->progress();
    progress->show();
}
/*#ifdef snap
void MainWindow::on_backz_clicked()
{
    if (snapshot.size()>9)
        snapshot.removeFirst();
    snapshot.append(dwSnapshot(ui -> text1-> toPlainText(), ui -> text2-> toPlainText()));
    snapshot_iterator = snapshot.size();

    qDebug() << snapshot.size();
}
#endif*/

void MainWindow::on_autozamena_clicked()
{
    autoz = autoz?0:1;
    QString  str_autoz[2] = {"\u0410\u0432\u0442\u043e\u0437\u0430\u043c\u0435\u043d\u0430 (\u0432\u044b\u043a\u043b)", "\u0410\u0432\u0442\u043e\u0437\u0430\u043c\u0435\u043d\u0430 (\u0432\u043a\u043b)"};
    ui->autozamena->setText(str_autoz[autoz]);
    ui->autozamena->setStyleSheet(maintheme.do_autoz(autoz));
}

void MainWindow::end()
{

}

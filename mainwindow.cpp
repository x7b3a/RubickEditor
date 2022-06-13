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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QJsonObject json = read_json("config2.json").object();
    QJsonValue value = json.value(QString("Theme"));
    qDebug() << "theme" << value.toInt();
    maintheme.theme = value.toInt();
    QJsonObject WikiAndFixes= json.value("WikiAndFixes").toObject();
    this -> showMaximized();
    this->setWindowTitle("Rubick Editor");
    this->setWindowIcon(QIcon(":/images/images/Rubick_icon.webp"));
    set_buttons();
    this->setStyleSheet("background-image:url()");
    set_theme();
    from = new ui_settings();
    connect(this, SIGNAL(sendData(QString)), from, SLOT(recieveData(QString)));  //отправка данных из Form1 в Form2
    connect(from, SIGNAL(sendData(QString)), this, SLOT(recieveData(QString)));  //отправка данных обратно из Form2 в Form1
}

MainWindow::~MainWindow()
{
     QJsonObject recordObject;
     recordObject.insert("Theme",maintheme.theme);
     QFile file;
     QJsonArray stringArray;
     stringArray.push_back(QJsonValue::fromVariant(ui -> button1 -> text().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> button2 -> text().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> button3 -> text().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> button4 -> text().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> button5 -> text().toUtf8()));
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
    QString wikiurl = "https://dota2.fandom.com/ru/wiki/\u0423\u0447\u0430\u0441\u0442\u043d\u0438\u043a:X7b3a2j/\u041f\u0435\u0441\u043e\u0447\u043d\u0438\u0446\u0430";
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
    QSSWITCH(switchStr,
                QSCASE(cases[0], //Викификатор и фиксы
                {
                    WikiAndFixes();break;
                })
                QSCASE(cases[1], //Замена точек на запятую
                {
                    Commafix();break;
                })
                QSCASE(cases[2],//"Раздел "Изменения"
                {
                   Changelogs();break;
                })
                QSCASE(cases[3],//"Раздел "Реплики"
                {
                    Responses();break;
                })
                QSCASE(cases[4],// "Раздел "Звуки"
                {
                    Sounds();break;
                })
                QSCASE(cases[5],//"Раздел "Косметика"
                {
                   Cosmetics();break;
                })
                QSCASE(cases[6],//"Units - Существа"
                {
                   Units();break;
                })
                QSDEFAULT(
                {
                   ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                   ui -> text2->setText("dw error = \u043a\u043d\u043e\u043f\u043a\u0430 \u0441 \u0442\u0430\u043a\u0438\u043c \u043d\u0430\u0437\u0432\u0430\u043d\u0438\u0435\u043c\n \u043d\u0435 \u043d\u0430\u0439\u0434\u0435\u043d\u0430");break;
                })
                )
}


void MainWindow::WikiAndFixes()
{
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
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    qDebug() << letter_with_for_medoke[0].toString() << letter_with_for_medoke[1].toString();
    QRegExp texp = QRegExp(" " + letter_with_for_medoke[0].toString()+ " 1([0-9][0-9][^0-9])");
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(" " + letter_with_for_medoke[1].toString() + " 1" + texp.cap(1)));
    }
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void MainWindow::Commafix()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString comma = color(",");
    QString point = ".";
    QString first = get_text();
    for (int j = 0;j<10;j++)
        for (int k = 0;k<10;k++)
            first.replace(QString::number(j) + point + QString::number(k),QString::number(j) + comma + QString::number(k));
    QJsonObject json = read_json("dict.json").object();
    QJsonArray Commafix = json["Commafix"].toArray();
    QRegExp texp = QRegExp("([0-9])"+ comma+"([0-9abcdefgh]{1,3}-[0-9])"+comma+"([0-9abcdefgh]{1,3})");
     qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
    for (int i = 0; i<Commafix.size();i++)
    {
        for (int j = 0;j<10;j++)
            for (int k = 0;k<10;k++)
                first.replace(Commafix[i].toString()+QString::number(j) + comma + QString::number(k),Commafix[i].toString()+QString::number(j) + "."+ QString::number(k));
    }
    for (int i = 0;i<10;i++)
        for (int j = 0;j<100;j++)
            for (int k = 0;k<100;k++)
                first.replace(comma +QString::number(j)+"-"+QString::number(i)+ comma +QString::number(k),"."+QString::number(j)+"-"+QString::number(i)+"."+QString::number(k));
    int counted = counter(first);
    label_settext(counted);
    put_text(first);

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
    QString temp1;
    QString temp2;
    QRegExp texp;
    int iter = 0;
    for (i=Keywords_small.begin(); i!= Keywords_small.end();i++) //increased
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

    for (i=Keywords_cooldown.begin();i!=Keywords_cooldown.end();i++) //increased as "ycileno"
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
    for (i=Keywords.begin(); i!= Keywords.end();i++) //Increased
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
    QString fromto = from+ space + "([0-9/\\.\\-,%]{1,20})( on each level |.)to ([0-9/\\.\\-,%]{1,20})( on each level.|.)";
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

        qDebug() << "end??";

    end_regular_replacer (&first);
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void  MainWindow::Responses()
{
    QString space = " ";
    QString proc = "%";
    QString Int = "([0-9]+)";
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
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    QString temp;
    QRegExp texp;
    temp = Int + "% chance";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(chance + texp.cap(1)+ proc));
    }
    temp = Int + " seconds cooldown";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color("\u041f\u0435\u0440\u0435\u0437\u0430\u0440\u044f\u0434\u043a\u0430 " + texp.cap(1)+ (texp.cap(1).toInt()>4?" \u0441\u0435\u043a\u0443\u043d\u0434":texp.cap(1).toInt()>1?" \u0441\u0435\u043a\u0443\u043d\u0434\u044b":" \u0441\u0435\u043a\u0443\u043d\u0434\u0430")));
    }
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void  MainWindow::Sounds()
{
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Sounds").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void  MainWindow::Cosmetics()
{
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Cosmetics").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void  MainWindow::Units()
{
    QString first = get_text();
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = read_json("dict.json").object();
        QJsonObject WikiAndFixes= jsonObject.value("Units").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace("{{U|" + i.key(),color("{{U|" + i.value()));
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void MainWindow::set_theme()
{

    maintheme.size =this->size();
    QString highlight = maintheme.get_highlight();
    QString text_back = maintheme.get_backcolor();
    QString button_back = maintheme.get_buttoncolor();
    QString text = maintheme.get_textcolor();
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
        ui->button1 ->setStyleSheet(btext + "background-color: rgb" + button_back +";\
                color: rgb" + text +";\
                 border-right-color: rgb" + details + ";");
        ui->button2 ->setStyleSheet(btext + "background-color: rgb" + button_back +";\
                color: rgb" + text +";\
                 border-right-color: rgb" + details + ";");
        ui->button3 ->setStyleSheet(btext + "background-color: rgb" + button_back +";\
                color: rgb" + text +";\
                 border-right-color: rgb" + details + ";");
        ui->button4 ->setStyleSheet(btext + "background-color: rgb" + button_back +";\
                color: rgb" + text +";\
                 border-right-color: rgb" + details + ";");
        ui->button5 ->setStyleSheet(btext + "background-color: rgb" + button_back +";\
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
 color: rgb"+text +"; border-color: rgb" + border + ";");
         ui->text2 -> setStyleSheet(bback + text_back + ";\
                                    color: rgb"+text +"; border-color: rgb" + border + ";");
         ui -> buttonpaste ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttonpaste_2 ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttoncopy ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttoncopy_2->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         ui -> buttochange ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
         //ui -> refreshButton ->setStyleSheet("border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;\n\nbackground-color: rgb" + button_back + "; color: rgb" + text + ";");
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
    text.replace("\n","<br>");
    text = "<html>" + text + "</html>";
    if (!output)
        ui->text1 ->setText(text);
    else
        ui -> text2 -> setText(text);
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

void MainWindow::set_buttons()
{
    QJsonObject json = read_json("config2.json").object();
    QJsonArray Macros = json["Macros"].toArray();

   ui -> button1 -> setText(Macros[0].toString());
   ui -> button2 -> setText(Macros[1].toString());
   ui -> button3 -> setText(Macros[2].toString());
   ui -> button4 -> setText(Macros[3].toString());
   ui -> button5 -> setText(Macros[4].toString());
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
    input = input?0:1;
    int x = (ui -> Input_label->mapToGlobal(QPoint(0,0))).x();
    int width = ui->Input_label->geometry().size().width();
    ui->Input_label ->setGeometry(input?x+width:x-width,-50, 85,111);
    ui->Input_win->setText(input?"\u0412\u0432\u043e\u0434: \u043e\u043a\u043d\u043e \u0441\u043f\u0440\u0430\u0432\u0430":"\u0412\u0432\u043e\u0434: \u043e\u043a\u043d\u043e \u0441\u043b\u0435\u0432\u0430");
    qDebug() << input;

}

void MainWindow::on_Output_win_clicked()
{
    output = output?0:1;
    int x = (ui -> Output_label->mapToGlobal(QPoint(0,0))).x();
    int width = ui->Output_label->geometry().size().width();
    ui->Output_label ->setGeometry(output?x+width:x-width,-50, 85,111);
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

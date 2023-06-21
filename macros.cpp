#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "macros.h"
#include "dwjson.h"
#include <QDesktopWidget>
#include <QWidget>
#include <QObject>
Macros::Macros()
{

}

Macros::~Macros()
{
    first.clear();
    errors.clear();
}

Macros::Macros(QString str, QString str2)
{
    first = str;
    colour = str2;
}
void Macros::clearing()
{
    first.clear();
    errors.clear();
}

void Macros::Commafix()
{
    qDebug() << "Commafix called";
    QString comma = color(",");
    QString point = ".";
    send_progress(10);
    for (int j = 0;j<10;j++)
        for (int k = 0;k<10;k++)
            first.replace(QString::number(j) + point + QString::number(k),QString::number(j) + comma + QString::number(k));
    send_progress(20);
    QJsonObject json = dwJ.read_json("dict.json").object();
    if (dwJ.errors)
    {
        qDebug() << "errors";
        errors = "dict.json";
        return;
    }
    QJsonArray Commafix = json["Commafix"].toArray();
        // (7.23d-7.24e)
    QRegExp texp = QRegExp("\\(([0-9])"+ comma +"([0-9abcdefgh]{2,3}-[0-9])"+comma+"([0-9abcdefgh]{2,3})\\)");
    while(texp.indexIn(first)!=-1 )
    {
         qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
         first.replace(texp.cap(0),"(" + texp.cap(1) + point + texp.cap(2)+ point + texp.cap(3)+ ")");
    }
    send_progress(30);
        // (7.23d)
    texp = QRegExp("\\(([0-9])" + comma + "([0-9abcdefgh]{2,3})\\)");
    while(texp.indexIn(first)!=-1 )
    {
         qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
         first.replace(texp.cap(0),"(" + texp.cap(1) + point + texp.cap(2) + ")");

    }
        // {{cf|At|2.28|2.31d}}
    send_progress(40);
    texp = QRegExp(("f\\|([Aa])t\\|([0-9])"+ comma +"([0-9abcdefgh]{2,3}\\|[0-9])"+comma+"([0-9abcdefgh]{2,3})\\}\\}"));
    while(texp.indexIn(first)!=-1 )
    {
         qDebug() <<"while:"<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
         first.replace(texp.cap(0),"f|" + texp.cap(1) + "t|" + texp.cap(2) + point + texp.cap(3) + point + texp.cap(4) + "}}");

    }
     send_progress(50);
    for (int i = 0; i<Commafix.size();i++)
    {
        for (int j = 0;j<10;j++)
            for (int k = 0;k<10;k++)
                first.replace(Commafix[i].toString()+QString::number(j) + comma + QString::number(k),Commafix[i].toString()+QString::number(j) + "."+ QString::number(k));
    }
    send_progress(65);
    send_progress(85);
    send_progress(100);
    counted = counter(first);

}

void Macros::WikiAndFixes()
{
    qDebug() << "WikiAndFixes called";
   // send_progress(1);
    send_progress(1);
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
    dwJ.read_json("dict.json");
        QJsonObject jsonObject = dwJ.read_json("dict.json").object();
        if (dwJ.errors)
        {
            qDebug() << "errors";
            errors = "dict.json";
            return;
        }
        QJsonObject WikiAndFixes= jsonObject.value("WikiAndFixes").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
        QMap<QString,QString> Symbols = dwJ.map_parser(WikiAndFixes, "Symbols");
        for (i=Symbols.begin();i!=Symbols.end();i++)
        {
            qDebug() << i.key();
            QString temp  =QString("{{Symbol|" + i.key());
            qDebug() << temp;
                first.replace(temp,color(dict.value("{{Symbol|") + i.value()), Qt::CaseInsensitive);
           // first.replace(i.key(),i.value());
        }
    QJsonArray letter_with_for_medoke = WikiAndFixes["letter_with_for_medoke"].toArray();
    //send_progress(50);
    send_progress(40);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));

    //send_progress(75);
    send_progress(60);
    QRegExp texp = QRegExp(" " + letter_with_for_medoke[0].toString()+ " 1([0-9][0-9][^0-9])");
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(" " + letter_with_for_medoke[1].toString() + " 1" + texp.cap(1)));
    }



    texp = QRegExp("\\{\\{VersionTableElement\\|([0-9a-g\\.]{4,5})\\|\\{\\{");
    //texp.setPatternSyntax(QRegExp::FixedString);
    qDebug() <<texp;
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),"{{VersionTableElement|" + texp.cap(1) +  color("|\n{{"));
    }

   // send_progress(100);
    send_progress(90);
    counted = counter(first);
}

void Macros::Changelogs()
{
    qDebug() <<  "Changelogs called";
    QJsonObject json = dwJ.read_json("dict.json").object();
    if (dwJ.errors)
    {
        qDebug() << "errors";
        errors = "dict.json";
        return;
    }
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
    QString sec = QStringLiteral(u" сек.");
    QString secwp = QStringLiteral(u" сек");
    QString minus = QStringLiteral(u"\u2212"); //минус
    QString arrow = QStringLiteral(u"\u279c"); //стрелочка
    QMap<QString, QString> Keywords = dwJ.map_parser(item,"Keywords");
    QMap<QString, QString> Preposition = dwJ.map_parser(item,"Preposition");
    QMap<QString, QString> Attributes = dwJ.map_parser(item,"Attributes");
    QMap<QString, QString> Attributes_base = dwJ.map_parser(item,"Attributes_base");
    QMap<QString, QString> Attributes_flex = dwJ.map_parser(item,"Attributes_flex");
    QJsonArray Flex_verb = item["Flex_verb"].toArray();
    QJsonArray Flex_adj = item["Flex_adj"].toArray();
    QJsonObject Stats = item["Stats"].toObject();
        QMap<QString, QString> Stats_d = dwJ.map_parser(Stats, "duration");
        QMap<QString, QString> Stats_d_n = dwJ.map_parser(Stats, "duration_n");
        QMap<QString, QString> Stats_f = dwJ.map_parser(Stats, "female");
        QMap<QString, QString> Stats_m = dwJ.map_parser(Stats, "male");
        QMap<QString, QString> Stats_n = dwJ.map_parser(Stats, "neuter");
    QMap<QString, QString> Talents = dwJ.map_parser(item,"Talents");
    QMap<QString, QString> Anti_Irismus_Talents_level = dwJ.map_parser(item,"Anti_Irismus_Talents_level");
    QMap<QString, QString> Talents_level = dwJ.map_parser(item,"Talents_level");
    QMap<QString, QString> Talents_changes = dwJ.map_parser(item, "Talents_changes");
    QMap<QString, QString> Talents_changes_n = dwJ.map_parser(item,"Talents_changes_n");
    QMap<QString, QString> Talents_changes_proc = dwJ.map_parser(item,"Talents_changes_proc");
    QMap<QString, QString> Keywords_small = dwJ.map_parser(item,"Keywords_small");
    QMap<QString, QString> Aghanim = dwJ.map_parser(item, "Aghanim");
    QMap<QString, QString> Keywords_cooldown = dwJ.map_parser(item,"Keywords_cooldown");
    QMap<QString, QString> Cooldown = dwJ.map_parser(item, "Cooldown");
    QMap<QString, QString> Talents_abilities = dwJ.map_parser(item,"Talents_abilities");
    QMap<QString, QString> Talents_abilities_proc = dwJ.map_parser(item,"Talents_abilities_proc");
    QJsonObject Abilities_one = item["Abilities_one"].toObject();
    QMap<QString, QString> Abilities_one_d = dwJ.map_parser(Abilities_one,"duration");
    QMap<QString, QString> Abilities_one_d_n = dwJ.map_parser(Abilities_one,"duration_n");
    QMap<QString, QString> Abilities_one_f = dwJ.map_parser(Abilities_one,"female");
    QMap<QString, QString> Abilities_one_m = dwJ.map_parser(Abilities_one,"male");
    QMap<QString, QString> Abilities_one_n = dwJ.map_parser(Abilities_one,"neuter");
    QMap<QString, QString> New_Talent = dwJ.map_parser(item,"New_Talent");
    QMap<QString, QString> New_Talent_abilities = dwJ.map_parser(item,"New_Talent_abilities");
    QMap<QString,QString> New_ability = dwJ.map_parser(item,"New_ability");
    QMap<QString,QString> Other_last = dwJ.map_parser(item,"Other_last");
    QMap<QString,QString> unihero = dwJ.map_parser(item,"unihero");

   // send_progress(1);
    send_progress(1);
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
    //send_progress(10);
    int pr=10;
    send_progress(10);
    QString temp1;
    QString temp2;
    QRegExp texp;
    QRegExp texp2;
    int iter = 0;
    //tal changes


    for (i=unihero.begin();i!=unihero.end();i++)
    {
        texp = QRegExp(start_regular_replacer("{{H|") + "([a-zA-Z\\ \\-]{1,35})" +(start_regular_replacer("}}")+ start_regular_replacer(i.key())));
        while(texp.indexIn(first)!=-1 )
        {
            first.replace(texp.cap(0),color(i.value()));
        }

    }
    for (i=Keywords_small.begin(); i!= Keywords_small.end();i++,pr+=15) //increased
    {
        send_progress(pr);
        for (j = Talents_changes.begin();j!=Talents_changes.end();j++)
        {
            temp1 = Float+space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                iter++;
                 //last qdebug qDebug() <<"while:"<< QString::number(iter)<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1).replace("+","") + space + Preposition.value(to)+ space + texp.cap(5).replace("+","")));

            }
        }
        for (j=Talents_changes_proc.begin();j!=Talents_changes_proc.end();j++)
        {
            temp1 = Float + "%" +space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float+"%" ;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1).replace("+","") +proc+ space + Preposition.value(to)+ space + texp.cap(5).replace("+",""))+proc);
            }
        }
        for (j = Talents_abilities.begin();j!=Talents_abilities.end();j++)
        {
             temp1  =   Float + space + ability + space + j.key() + space + i.key() + space + to + space + Float;
             texp = QRegExp(temp1);
             while(texp.indexIn(first)!=-1 )
             {
                first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("+","") + space + Preposition.value(to)+ space  + texp.cap(7).replace("+","")));
             }
         }
        for (j = Talents_abilities.begin();j!=Talents_abilities.end();j++)
        {
             temp1  =   Float +"s"+ space + ability + space + j.key() + space + i.key() + space + to + space + Float +"s";
             texp = QRegExp(temp1);
             while(texp.indexIn(first)!=-1 )
             {
                first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("+","") + space + Preposition.value(to)+ space  + texp.cap(7).replace("+","") + secwp));
             }
         }
        for (j=Talents_abilities_proc.begin();j!=Talents_abilities_proc.end();j++)
        {
            temp1 = Float + "%" +space + ability + space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float+"%" ;
            texp = QRegExp(temp1);
             while(texp.indexIn(first)!=-1 )
            {
                //last qdebug qDebug() << "  proc:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+Flex_verb[0].toString()+ space+ Preposition.value(from) + space + texp.cap(1).replace("+","") + proc+ space  + Preposition.value(to)+ space + texp.cap(7).replace("+","")+proc));
            }
        }
    }
    //tal changes end

    for (i=Keywords_small.begin(); i!= Keywords_small.end();i++,pr+=15) //increased
    {
        send_progress(pr);
        for (j = Talents_changes.begin();j!=Talents_changes.end();j++)
        {
            temp1 = Float+space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+Float + "\\." ;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                iter++;
                 //last qdebug qDebug() <<"while:"<< QString::number(iter)<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
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
                //last qdebug qDebug() << "  proc:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+Flex_verb[0].toString()+ space+ Preposition.value(from) + space + texp.cap(1).replace("+","") + proc+ space  + Preposition.value(to)+ space + texp.cap(7).replace("+","")+proc+point));
            }
        }
    }


    for (i=Keywords_cooldown.begin();i!=Keywords_cooldown.end();i++,pr+=10) //increased as "ycileno"
    {
        send_progress(pr);
        for (j = Cooldown.begin();j!= Cooldown.end();j++)
        {
            temp1  =   Float +"s"+ space + ability + space + j.key() + space + i.key() + space + to + space + Float + "s.";
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                qDebug() << "123";
              //  qDebug() << " cooldown:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("-",minus) + space + Preposition.value(to)+ space  + texp.cap(7).replace("-",minus)+sec));
            }

            temp1  =   Float +"s"+ space + start_regular_replacer(j.key()) + space + i.key() + space + to + space + Float + "s.";
           // qDebug() << "temp1:" <<temp1;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                //last qdebug qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
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
    for (i=Keywords.begin(); i!= Keywords.end();i++,pr+=10) //Increased
    {
        send_progress(pr);
        for (j=Attributes_base.begin(),count = 0;j!=Attributes_base.end();j++,count++)
        {
            temp1 = i.key()+space+base+space+start_regular_replacer(j.key() + space + from);
            temp2 = Attributes.value(base)+Flex_adj[count==1?0:1].toString()+space+j.value()+space+i.value()+Flex_verb[count==1?0:1].toString();
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
        //qDebug() << Stats_d_n.begin().key();
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
        for (j=Abilities_one_d_n.begin();j!=Abilities_one_d_n.end();j++)
        {
            temp1 = i.key() + space + ability + j.key() + from + space + Level_numbers;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color("{{A|"+ texp.cap(1)+ "|"+texp.cap(2)+"}}: " +j.value()+ i.value()+Flex_verb[2].toString()+ space+ Preposition.value(from) + space + texp.cap(3)+ texp.cap(4).replace(" on each level",eachlevel_rus)  + Preposition.value(to)+ space + texp.cap(5) +QString(sec + texp.cap(6).replace(" on each level",eachlevel_rus)).replace("..",".")));
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
            //last qdebug qDebug() << "abil" << temp1;

            while(texp.indexIn(first)!=-1 )
            {
                //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
                  first.replace(texp.cap(0),color("{{A|"+ texp.cap(1)+ "|"+texp.cap(2)+"}}: " +j.value()+ i.value()+Flex_verb[2].toString()+ space) + from);
            }
         }
    }
    send_progress(90);
    QString fromto = from+ space + "([0-9/\\.\\-,\u2012,%]{1,20})( on each level |.)to ([0-9/\\.\\-,\u2012,%]{1,20})( on each level.|\\.)";
    texp = QRegExp(fromto);
    while(texp.indexIn(first)!=-1 )
    {
         first.replace(texp.cap(0),color(Preposition.value(from) + space + texp.cap(1)+ texp.cap(2).replace(" on each level",eachlevel_rus)  + Preposition.value(to)+ space + texp.cap(3) + texp.cap(4).replace(" on each level",eachlevel_rus)));
    }
    for (i=New_Talent.begin();i!=New_Talent.end();i++)
    {
        temp1 = "(;|=|= )([0-9/\\.\\-\\+\\ ,%]{1,10})" + space + start_regular_replacer(i.key()) +";";
        texp = QRegExp(temp1);
        temp2 = "(;|=)([0-9/\\.\\-\\+\\ ,%]{1,10})" + space + start_regular_replacer("[["+i.key()+"]]")+";";
        texp2=QRegExp(temp2);
        //qDebug() << "tal" << temp1 << texp;
        while(texp.indexIn(first)!=-1 )
        {
            qDebug() << texp.cap(0);
            //last qdebug qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
            first.replace(texp.cap(0),texp.cap(1)+color(texp.cap(2)+space + i.value())+";");
        }
        while(texp2.indexIn(first)!=-1 )
        {
                        qDebug() << texp2.cap(0);
            //last qdebug qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
            first.replace(texp2.cap(0), texp2.cap(1)+color(texp2.cap(2)+space+i.value())+";");
        }
    }
    for (i=New_Talent_abilities.begin();i!=New_Talent_abilities.end();i++)
    {
        temp1 = "([0-9/\\.\\-\\+\\ ,%s;=]{1,10}) "  + ability + start_regular_replacer(i.key());
        texp = QRegExp(temp1);
        //last qdebug qDebug() << "tal" << temp1 << texp;
        while(texp.indexIn(first)!=-1 )
        {
            //qDebug() << " respawn:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
            first.replace(texp.cap(0),color(texp.cap(1).replace("s","").replace("-",minus) + i.value() +space+ " {{A|" + texp.cap(2)+"|"+ texp.cap(3) + "}}"));
        }
    }
    send_progress(92);
    for (i = New_ability.begin();i!= New_ability.end();i++)
    {
        first.replace("::" + i.key() + ":",":: " + color(i.value() + ":"));
        first.replace(":: " + i.key() + ":",":: " + color(i.value() + ":"));
    }
    send_progress(96);
    for (i=Other_last.begin();i!=Other_last.end();i++)
    {
        first.replace(i.key(), color(i.value()));
    }

        qDebug() << "end??";
    end_regular_replacer (&first);
    counted = counter(first);
}


void Macros::Responses()
{
    qDebug() << "Responses called";
    send_progress(1);
    QString space = " ";
    QString proc = "%";
    QString DInt = "([1-9][0-9])";
    QString Int = "([1-9])";
    QString chance = QStringLiteral(u"Шанс ");

    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = dwJ.read_json("dict.json").object();
        if (dwJ.errors)
        {
            qDebug() << "errors";
            errors = "dict.json";
            return;
        }
        QJsonObject WikiAndFixes= jsonObject.value("Responses").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    send_progress(33);
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
    temp = DInt + "% Chance";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(chance + texp.cap(1)+ proc));
    }
    temp = Int + "% Chance";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(chance + texp.cap(1)+ proc));
    }
    send_progress(66);
    temp = Int + " seconds cooldown";
    texp = QRegExp(temp);
    while(texp.indexIn(first)!=-1 )
    {
        first.replace(texp.cap(0),color(QStringLiteral(u"Перезарядка ") + texp.cap(1)+ (texp.cap(1).toInt()>4?QStringLiteral(u" секунд"):texp.cap(1).toInt()>1?QStringLiteral(u" секунды"):QStringLiteral(u" секунда"))));
    }
    send_progress(100);
    counted = counter(first);
}


void Macros::Sounds()
{
    qDebug() << "Sound called";
    send_progress(1);

    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = dwJ.read_json("dict.json").object();
        if (dwJ.errors)
        {
            qDebug() << "errors";
            errors = "dict.json";
            return;
        }
        QJsonObject WikiAndFixes= jsonObject.value("Sounds").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    send_progress(50);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    send_progress(100);
    counted = counter(first);
}

void Macros::Cosmetics()
{
    qDebug() << "Cosmetics called";
    send_progress(1);

    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = dwJ.read_json("dict.json").object();
        if (dwJ.errors)
        {
            qDebug() << "errors";
            errors = "dict.json";
            return;
        }
        QJsonObject WikiAndFixes= jsonObject.value("Cosmetics").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    send_progress(50);
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
    int isInterwiki = first.indexOf("[[en:");
    qDebug() << isInterwiki;
    if (isInterwiki<0)
    {
        QRegExp rxlen("\\| name = ([A-Za-z'\\s!?\\(\\)\\-,:]{1,40})\\n\\|");
        int pos = rxlen.indexIn(first);
        QString name;
        if (pos > -1)
            name = rxlen.cap(1);
        first += color("\n[[en:" + name + "]]");
    }
    int isBundle = first.indexOf("| prefab = Bundle");
    if (isBundle>=0)
    {
        QRegExp rxlen("(\\| setitem([0-9]{1,2}) = ([A-Za-z'\\s!?\\(\\)\\-,:]{1,35}) Loading Screen\\n)");
        int pos = rxlen.indexIn(first);
        if (pos>-1)
            first.replace(rxlen.cap(0),color("| setitem" + rxlen.cap(2) + QStringLiteral(u" = Загрузочный экран: ") + rxlen.cap(3) + "\n"));
            qDebug() << rxlen.cap(0) << rxlen.cap(1) << rxlen.cap(2) << rxlen.cap(3);
    }
    int isLoadingScreen = first.indexOf("| prefab = Loading Screen");
    if (first.indexOf("| slot = Loading Screen")>isLoadingScreen)
        isLoadingScreen = first.indexOf("| slot = Loading Screen");
    if (isLoadingScreen>=0)
    {
        QRegExp rxlen("\\| name = ([A-Za-z'\\s!?\\(\\)\\-,:]{1,35}) Loading Screen\\n\\|");
        int pos = rxlen.indexIn(first);
        QString name;
        if (pos > -1)
            first.replace(rxlen.cap(0), color(QStringLiteral(u"| name = Загрузочный экран: ") + rxlen.cap(1) + "\n|"));
    }
    send_progress(100);
    counted = counter(first);
}

void Macros::Units()
{
    qDebug() << "Units called";
    send_progress(1);

    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject jsonObject = dwJ.read_json("dict.json").object();
        if (dwJ.errors)
        {
            qDebug() << "errors";
            errors = "dict.json";
            return;
        }
        QJsonObject WikiAndFixes= jsonObject.value("Units").toObject();
        foreach(const QString& key, WikiAndFixes.keys()) {
            QJsonValue value = WikiAndFixes.value(key);
            dict.insert(key,value.toString());
        }
    send_progress(50);
    for (i = dict.begin(); i != dict.end(); i++)
    {
        first.replace ("{{Show|A|" + i.key()+"|", color("{{Show|A|" + i.value()+"|"));
        first.replace ("{{Show|U|" + i.key()+"|", color("{{Show|U|" + i.value()+"|"));
        first.replace("|" + i.key()+"}}",color("|" + i.value()+"}}"));
        first.replace("|" + i.key()+"|text",color("|" + i.value())+"|text");
    }
    send_progress(100);
    counted = counter(first);
}

void Macros::Animations()
{
    qDebug() << "Anime called";
    QMap<QString, QString> dict;
    QMap<QString, QString>::iterator i;
        QJsonObject json = dwJ.read_json("dict.json").object();
        if (dwJ.errors)
        {
            qDebug() << "errors";
            errors = "dict.json";
            return;
        }
        QJsonValue value = json.value(QString("Animations"));
        QJsonObject item = value.toObject();
        QMap<QString, QString> Complex = dwJ.map_parser(item,"Complex");
        QMap<QString, QString> Main = dwJ.map_parser(item,"Main");
        QMap<QString, QString> Tags = dwJ.map_parser(item,"Tags");
        QMap<QString, QString> Extra;
        Extra.insert("2 ||", " 2 ||");
        Extra.insert("3 ||", " 3 ||");
        Extra.insert("4 ||", " 4 ||");
        QRegExp texp;
        QString texr;
       for (i=Extra.begin();i!=Extra.end();i++)
        {    
            first.replace(i.key(),i.value());
        }
        for (i=Complex.begin();i!=Complex.end();i++)
        {
            first.replace(i.key(), i.value());
        }
        send_progress(60);
       for (i = Main.begin(); i != Main.end(); i++)
        {
            first.replace(i.key(),color(i.value()));
        }
        send_progress(70);
        for (i = Tags.begin(); i != Tags.end(); i++)
        {
            texr = "("+ i.key()+")([^\\.]{30})";
            texp = QRegExp(texr);
            while(texp.indexIn(first)!=-1 )
            {
                first.replace(texp.cap(0),color(i.value()) + texp.cap(2));
            }
        }

    send_progress(80);
    counted = counter(first);
    send_progress(100);
}
QString Macros::get_backtext()
{
   return QString("<span style= \"background:");
}

int Macros::counter(QString text)
{
    QRegExp found(get_backtext());
    return text.count(found);
}

QString Macros::start_regular_replacer (QString temp)
{
    QString Array[5] = {"|", "{", "}", "[", "]"};
    int i;
    for (i=0; i<5;i++)
         temp.replace(Array[i],"&dw0"+QString::number(i)+"c");
    return temp;
}

void Macros::end_regular_replacer (QString *temp)
{
    QString Array[5] = {"|", "{", "}", "[", "]"};
    int i;
    for (i=0; i<5;i++)
        &temp ->replace("&dw0"+QString::number(i)+"c",Array[i]);
}
void Macros::send_progress(int pr)
{
    emit new_progress(pr);
}

QString Macros::color(QString arg,QString color)
{
    return QString("<span style= \"background:%1\">%2</span>").arg(colour,arg);
}

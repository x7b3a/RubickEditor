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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/images/back.png").scaled(this->size())));
    this->setPalette(palette);
    this->setStyleSheet("background-image:url()");

    this -> showMaximized();
    this->setWindowTitle("Rubick Editor");
    this->setWindowIcon(QIcon(":/images/images/Rubick_icon.webp"));

    set_buttons();
}

MainWindow::~MainWindow()
{
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
    int ishow = 0;
    ui_settings *from = new ui_settings;
    from ->show();
    from->activateWindow();
    ishow ++;
}

void MainWindow::on_refreshButton_clicked()
{
    set_buttons();
}

void MainWindow::on_dota2wiki_clicked()
{
    QString wikiurl = "https://dota2.fandom.com/ru/wiki/Dota_2_\u0412\u0438\u043a\u0438";
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
    pcb->setText(ui->text2->toPlainText());
}

void MainWindow::button_switch(QString switchStr)
{
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
                    ui -> text2->setText(cases[3]);break;
                })
                QSCASE(cases[4],// "Раздел "Звуки"
                {
                    ui -> text2->setText(cases[4]);break;
                })
                QSCASE(cases[5],//"Раздел "Косметика"
                {
                    ui -> text2->setText(cases[5]);break;
                })
                QSCASE(cases[6],//"Units - Существа"
                {
                    ui -> text2->setText(cases[6]);break;
                })
                QSDEFAULT(
                {
                   ui -> text2->setText("dw error = button not founded");break;
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
    for (i = dict.begin(); i != dict.end(); i++)
        first.replace(i.key(),color(i.value()));
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
    QString changed = " changed:";
    QString changed_rus = QJsonValue(item[changed]).toString();
    QString base = "base";
    QString gain = "gain";
    QString from = "from";
    QString point = ".";
    QString proc = "%";
    QString to = "to";
    QString sec = " \u0441\u0435\u043a.";
    QString minus = "\u2212";
  //  QMap<QString, QString> changed;
    QMap<QString, QString> Keywords = map_parser(item,"Keywords");
    QMap<QString, QString> Preposition = map_parser(item,"Preposition");
    QMap<QString, QString> Attributes = map_parser(item,"Attributes");
    QMap<QString, QString> Attributes_base = map_parser(item,"Attributes_base");
    QMap<QString, QString> Attributes_flex = map_parser(item,"Attributes_flex");
   // changed.insert(item.keys().at(0),item.value(item.keys().at(0)).toString());
    QJsonArray Flex_verb = item["Flex_verb"].toArray();
    QJsonArray Flex_adj = item["Flex_adj"].toArray();
    QJsonObject Stats = item["Stats"].toObject();
    QMap<QString, QString> Stats_f = map_parser(Stats, "female");
    QMap<QString, QString> Stats_m = map_parser(Stats, "male");
    QMap<QString, QString> Stats_n = map_parser(Stats, "neuter");
    QMap<QString, QString> Talents = map_parser(item,"Talents");
    QMap<QString, QString> Talents_level = map_parser(item,"Talents_level");
    QMap<QString, QString> Talents_changes = map_parser(item, "Talents_changes");
    QMap<QString, QString> Talents_changes_proc = map_parser(item,"Talents_changes_proc");
    QMap<QString, QString> Keywords_small = map_parser(item,"Keywords_small");
    QMap<QString, QString> Aghanim = map_parser(item, "Aghanim");
    QMap<QString, QString> Keywords_cooldown = map_parser(item,"Keywords_cooldown");
    QMap<QString, QString> Cooldown = map_parser(item, "Cooldown");
    first = start_regular_replacer(first);
    for (i=Aghanim.begin();i!=Aghanim.end();i++)
    {
        first.replace(start_regular_replacer(i.key()),color(i.value()));
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

    for (i=Keywords_small.begin(); i!= Keywords_small.end();i++)
    {
        for (j = Talents_changes.begin();j!=Talents_changes.end();j++)
        {
            temp1 = ".([0-9]+)"+space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+".([0-9]+)\\." ;
          //  qDebug() << "temp11:" << temp1;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {
                iter++;
                 qDebug() <<"while:"<< QString::number(iter)<< texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5);
                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1) + space + Preposition.value(to)+ space + texp.cap(2))+point);

            }

        }
        for (j=Talents_changes_proc.begin();j!=Talents_changes_proc.end();j++)
        {
            temp1 = "([0-9]+)%"+space + start_regular_replacer(j.key()) +space+i.key()+space+to+space+"([0-9]+)%\\." ;
            texp = QRegExp(temp1);
            while(texp.indexIn(first)!=-1 )
            {

                 first.replace(texp.cap(0),color( j.value() +space + i.value() +space +Preposition.value(from) + space +texp.cap(1) +proc+ space + Preposition.value(to)+ space + texp.cap(2))+proc+point);

            }
        }


    }
    for (i=Keywords_cooldown.begin();i!=Keywords_cooldown.end();i++)
    {
        j = Cooldown.begin();

        temp1  =   "(?=.)([+-]?(?=[\\d.])(\\d*)(.(\\d+))?)s"+ space + j.key() + space + i.key() + space + to + space + "(?=.)([+-]?(?=[\\d.])(\\d*)(.(\\d+))?)s.";
        //temp1 = ".([0-9]+)s" + space + j.key() + space + i.key() + space + to + space + ".([0-9]+)s.";
        qDebug() << "temp:" << temp1;
        texp = QRegExp(temp1);
        while(texp.indexIn(first)!=-1 )
        {
        qDebug() << " cooldown:" << texp.cap( 0 )  << texp.cap(1) << texp.cap(2) << texp.cap(3) << texp.cap(4) << texp.cap(5)<< texp.cap(6)<< texp.cap(7)<< texp.cap(8);
        first.replace(texp.cap(0),color(j.value() + " {{A|"+ texp.cap(5)+ "|"+texp.cap(6)+"}} " + i.value()+ space + Preposition.value(from) + space +texp.cap(1).replace("-",minus)+sec + space + Preposition.value(to)+ space  + texp.cap(7).replace("-",minus)+sec));
        }
    }
    int count;
    for (i=Keywords.begin(); i!= Keywords.end();i++)
    {
        for (j=Attributes_base.begin(),count = 0;j!=Attributes_base.end();j++,count++)
        {
            temp1 = i.key()+space+base+space+start_regular_replacer(j.key());
            //qDebug() << QString::number(count<2?1:0);
            temp2 = Attributes.value(base)+Flex_adj[count<2?0:1].toString()+space+j.value()+space+i.value()+Flex_verb[count<2?0:1].toString();
            first.replace(temp1,color(temp2));
        }
        for (j=Attributes_flex.begin();j!=Attributes_flex.end();j++)
        {
            temp1 = i.key()+space+start_regular_replacer(j.key())+space+gain+space;
            temp2 = Attributes.value(gain)+space+j.value()+space+i.value()+space;
            first.replace(temp1,color(temp2));
        }
        for (j=Stats_f.begin();j!=Stats_f.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key());
           // qDebug() << temp1;
            temp2 = j.value()+i.value()+Flex_verb[1].toString()+ space;
            first.replace(temp1,color(temp2));
        }
        for (j=Stats_m.begin();j!=Stats_m.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key());
            temp2 = j.value()+i.value()+Flex_verb[0].toString()+ space;
            first.replace(temp1,color(temp2));
        }
        for (j=Stats_n.begin();j!=Stats_n.end();j++)
        {
            temp1 = i.key()+start_regular_replacer(j.key());
            temp2 = j.value()+i.value()+Flex_verb[2].toString()+ space;
            first.replace(temp1,color(temp2));
        }
    }
    QString fromto = from+space + "(?=.)([+-]?(?=[\\d.])(\\d*)(.(\\d+))?)" + space + to + space + "(?=.)([+-]?(?=[\\d.])(\\d*)(.(\\d+))?)";
    texp = QRegExp(fromto);
    while(texp.indexIn(first)!=-1 )
    {
        //qDebug() << "1:" << texp.cap(0);
         first.replace(texp.cap(0),color(Preposition.value(from)+space + texp.cap(1)+space+Preposition.value(to)+space+texp.cap(5)));
    }

        qDebug() << "end??";

    end_regular_replacer (&first);
    int counted = counter(first);
    label_settext(counted);
     put_text(first);
}
QMap<QString, QString> MainWindow::map_parser(QJsonObject item, QString word)
{
    QMap<QString, QString> array;
    QJsonObject keywords_value = item[word].toObject();
    foreach(const QString& key, keywords_value.keys()) {
        QJsonValue value = keywords_value.value(key);
        array.insert(key,value.toString());
        //qDebug() << key << value.toString() << "key??";
    }
    return array;
}
void  MainWindow::Responses()
{
    QString first = get_text();
}

void  MainWindow::Sounds()
{
    QString first = get_text();
}

void  MainWindow::Cosmetics()
{
    QString first = get_text();
}

void  MainWindow::Units()
{
    QString first = get_text();
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
    QString first = ui -> text1 -> toPlainText();
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
    if ((ui->reverseMode ->QCheckBox::checkState()))
    {
        ui -> text2 ->setText(ui->text1 ->toPlainText());
        ui -> text1 -> setText(text);
    }
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
            qDebug() << "dw error: read json " << filename;
             // return 0;
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



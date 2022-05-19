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
                QSCASE(cases[0], //����������� � �����
                {
                    WikiAndFixes();break;
                })
                QSCASE(cases[1], //������ ����� �� �������
                {
                    Commafix();break;
                })
                QSCASE(cases[2],//"������ "���������"
                {
                   Changelogs();break;
                })
                QSCASE(cases[3],//"������ "�������"
                {
                    ui -> text2->setText(cases[3]);break;
                })
                QSCASE(cases[4],// "������ "�����"
                {
                    ui -> text2->setText(cases[4]);break;
                })
                QSCASE(cases[5],//"������ "���������"
                {
                    ui -> text2->setText(cases[5]);break;
                })
                QSCASE(cases[6],//"Units - ��������"
                {
                    ui -> text2->setText(cases[6]);break;
                })
                QSDEFAULT(
                {
                   ui -> text2->setText("dw error = not founded");break;
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
        first.replace(i.key(),color("#ff8f45",i.value()));
    int counted = counter(first);
    label_settext(counted);
    put_text(first);
}

void MainWindow::Commafix()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    QString comma = color("#ff8f45",",");
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
    QString changed = " changed";
    QString base = "base";
    QString gain = "gain";
    QString from = "from";
    QString to = "to";
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
    QMap<QString, QString> Keywords_small = map_parser(item,"Keywords_small");
    for (i = Talents_level.begin(); i != Talents_level.end(); i++)
    {
        first.replace(i.key() + changed,color("#ff8f45",i.value()));
        first.replace(i.key(),color("#ff8f45",i.value()));
    }
    for (i = Talents.begin(); i != Talents.end(); i++)
    {
        first.replace(i.key(),color("#ff8f45",i.value()));
    }
    int count;
    QString temp1;
    QString temp2;
    for (i=Keywords.begin(); i!= Keywords.end();i++)
    {
        for (j=Attributes_base.begin(),count = 0;j!=Attributes_base.end();j++,count++)
        {
            temp1 = i.key()+space+base+space+j.key();
            qDebug() << QString::number(count<2?1:0);
            temp2 = Attributes.value(base)+Flex_adj[count<2?0:1].toString()+space+j.value()+space+i.value()+Flex_verb[count<2?0:1].toString();
            first.replace(temp1,color("#ff8f45",temp2));
        }
        for (j=Attributes_flex.begin();j!=Attributes_flex.end();j++)
        {
            temp1 = i.key()+space+j.key()+space+gain;
            temp2 = Attributes.value(gain)+space+j.value()+space+i.value();
            first.replace(temp1,color("#ff8f45",temp2));
        }
    }
    int lastPos = 0;
    QRegExp re;
    int iter = 0;
    for (i=Keywords_small.begin(); i!= Keywords_small.end();i++)
    {
        for (j = Talents_changes.begin();j!=Talents_changes.end();j++)
        {
             re = QRegExp ("\\d+" + space + to + space + "\\d+");
            //re = QRegExp( "(\\d+)\\b"+space+j.key()+space+i.key()+space+to+space+"\\b(\\d+)" );
           // QString re = "1"+space+j.key()+space+i.key()+space+to+space+"2";
           // qDebug() << re;
            lastPos = 0;
            QString second = "1 to 20\n30 to 15\n1to20";
           // second.replace(re,"123");
            //first.replace(re,j.value() +space + i.value() + Preposition.value(from) + space +re.cap(2) + Preposition.value(to)+space + re.cap(1));
            //qDebug() << QString::number( re.indexIn(ar, lastPos ) );
            while( ( lastPos = re.indexIn(second, lastPos ) ) != -1 )
            {
                iter++;
                lastPos += re.matchedLength();
                 qDebug() <<"while:"<< QString::number(iter)<< re.cap( 0 ) << QString::number(lastPos);
                 //second.replace(re.cap(0), QString::number(lastPos));
                 lastPos = re.indexIn(second, lastPos );
                qDebug() <<second;
                 /*qDebug() << j.value() +space + i.value() +space + Preposition.value(from) + space +"re.cap(2)" + Preposition.value(to)+space + "re.cap(1)";
                first.replace(re ,j.value() +space + i.value() +space +Preposition.value(from) + space +"re.cap(2)" + Preposition.value(to)+space + "re.cap(1)");*/
             //   lastPos = 0;
            }

        }
    }
   /* QString str = "23 to 30\n45 to 325\nqwer23to24";
    QRegExp re("([0-9]+) to ([0-9]+)");
    int lastPos = 0;
    str.replace(re, re.cap(2)+ "to"+ re.cap(1));
     qDebug() << str;
    while( ( lastPos = re.indexIn( str, lastPos ) ) != -1 ) {
        lastPos += re.matchedLength();
        qDebug() << re.cap( 0 ) << ":" << re.cap( 1 ) << ":" << re.cap(2);

        qDebug() << str;
    }*/


//    qDebug() << first;
   /* static const char* const TEXT =
        "#include <QRegExp>\n"
        "#include <QStringList>\n"
        "#include <QDebug>\n"
        "\n"
        "int main() {\n"
            "QRegExp re( \"#include <([^>]+)>\" );\n"
            "int lastPos = 0;\n"
            "while( ( lastPos = re.indexIn( TEXT, lastPos ) ) != -1 ) {\n"
                "lastPos += re.matchedLength();\n"
                "qDebug() << re.cap( 0 );\n"
            "}\n"
            "return 0;\n"
        "}";



        QRegExp req( "#include <([^>]+)>" );

        int lastPosq = 0;
        while( ( lastPosq = req.indexIn( TEXT, lastPosq ) ) != -1 ) {
            lastPosq += req.matchedLength();
            qDebug() << req.cap( 0 ) << ":" << req.cap( 1 );
        }*/
        qDebug() << "end??";


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

QString MainWindow::color(QString color,QString arg)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
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



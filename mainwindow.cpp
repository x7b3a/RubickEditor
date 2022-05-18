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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        //�������� ����
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/images/back.png").scaled(this->size())));
    this->setPalette(palette);
    this->setStyleSheet("background-image:url()");
        //�������� �� ������
    this -> showMaximized();
    this->setWindowTitle("Rubick Editor");
    this->setWindowIcon(QIcon(":/images/images/Rubick_icon.webp"));

        //������ �����
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
                    ui -> text2->setText(cases[2]);break;
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
        ui ->text2 ->setText(ui->text1 ->toPlainText());
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
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForLocale(codec);

    QJsonParseError error;
        QJsonDocument Doc = QJsonDocument::fromJson(val.toUtf8(), &error);
        if (error.error != QJsonParseError::NoError)
        {
            qDebug() << "dw error: read json";
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

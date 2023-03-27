#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_settings.h"
#include "qsswitch.h"
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
#include <QSyntaxHighlighter>
#include <QLabel>
#include <string>
#include <QRegExp>
#include <QClipboard>
#include <QStringList>
#include <QThread>
#include <QDesktopWidget>
#include <QtWinExtras/QWinTaskbarProgress>
#include <QtWinExtras/QWinTaskbarButton>
#include "dwjson.h"
#include "macros.h"
#include "dwnetmacros.h"
#include <QTime>
#include <QPainter>
#include <QColor>
#include <QRgb>
#define RVERSION "1.1.0"

QT_FORWARD_DECLARE_CLASS(QWinTaskbarButton)
QT_FORWARD_DECLARE_CLASS(QWinTaskbarProgress)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    append_buttons();
    adaptive_screen();
    set_fonts();
    QJsonObject json = read_json("config2.json").object();
    QJsonValue value = json.value(QString("Theme"));
    maintheme.theme = value.toInt();

    this -> showMaximized();
    this -> setWindowTitle("Rubick Editor " + QString(RVERSION));
    this -> setWindowIcon(QIcon(":/images/images/Rubick_icon.webp"));
    set_buttons();
    append_cases();
    this->setStyleSheet("background-image:url()");
    maintheme.size =this->size();
    set_theme();
    from = new ui_settings();

    connect(this, SIGNAL(sendData(QString)), from, SLOT(recieveData(QString)));  //�������� ������ �� Form1 � Form2
    connect(from, SIGNAL(sendData(QString)), this, SLOT(recieveData(QString)));  //�������� ������ ������� �� Form2 � Form1

    connect(&dwnetcase,&dwNetMacros::send_end,this,&MainWindow::receive_netmacros);
    connect(&dwnetcase,&dwNetMacros::new_progress,this,&MainWindow::receive_progress);

    set_progressbar();


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

void MainWindow::receive_progress(int pr)
{
    if (pr>-1)
        progress->setValue(pr);
    else
        progress->stop();
}
void MainWindow::receive_netmacros()
{
    qDebug() << "netmacros rececived";
    put_text(dwnetcase.output);
    dwnetcase.cleaning();
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
    progress->resume();
    Macros dwcase(get_text(),maintheme.get_highlight());
    connect(&dwcase,&Macros::new_progress,this,&MainWindow::receive_progress);
    QSSWITCH(switchStr,
                QSCASE(cases[0], //����������� � �����
                {
                    dwcase.Commafix();
                    put_text(dwcase.first);
                    label_settext(dwcase.counted);
                    if (!dwcase.errors.isEmpty())
                 {
                     ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                     ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                 }
                 dwcase.clearing();break;
                })
                QSCASE(cases[1], //������ ����� �� �������
                {

                    dwcase.WikiAndFixes();
                    put_text(dwcase.first);
                    label_settext(dwcase.counted);
                    if (!dwcase.errors.isEmpty())
                    {
                        ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                        ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                    }
                    dwcase.clearing();
                    break;
                    //WikiAndFixes(); break;
                })
                QSCASE(cases[2],//"������ "���������"
                {
                    dwcase.Changelogs();
                    put_text(dwcase.first);
                    label_settext(dwcase.counted);
                    if (!dwcase.errors.isEmpty())
                    {
                        ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                        ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                    }
                    dwcase.clearing();
                   //Changelogs();break;
                })
                QSCASE(cases[3],//"������ "�������"
                {
                    dwcase.Responses();
                    put_text(dwcase.first);
                    label_settext(dwcase.counted);
                    if (!dwcase.errors.isEmpty())
                    {
                        ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                        ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                    }
                    dwcase.clearing();
                })
                QSCASE(cases[4],// "������ "�����"
                {
                    dwcase.Sounds();
                    put_text(dwcase.first);
                    label_settext(dwcase.counted);
                    if (!dwcase.errors.isEmpty())
                    {
                        ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                        ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                    }
                    dwcase.clearing();
                })
                QSCASE(cases[5],//"������ "���������"
                {
                   dwcase.Cosmetics();
                   put_text(dwcase.first);
                   label_settext(dwcase.counted);
                   if (!dwcase.errors.isEmpty())
                   {
                       ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                       ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                   }
                   dwcase.clearing();
                })
                QSCASE(cases[6],//"Units - ��������"
                {
                   dwcase.Units();
                   put_text(dwcase.first);
                   label_settext(dwcase.counted);
                   if (!dwcase.errors.isEmpty())
                   {
                       ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                       ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                   }
                   dwcase.clearing();
                })
                QSCASE(cases[7],
                {

                        dwnetcase.version=get_text();
                        dwnetcase.Patch_heroes();
                        break;


                })
                QSCASE(cases[8],
                {
                    dwnetcase.version=get_text();
                    dwnetcase.language=0;
                    dwnetcase.Patch_Version(0);
                    break;

                })
                QSCASE(cases[9],
                {
                    dwnetcase.version=get_text();
                    dwnetcase.language=1;
                    dwnetcase.Patch_Version(1);
                    break;
                })
                QSCASE(cases[10],
                {
                    qDebug() << "size" << get_text().size();
                    if (get_text().size()<100)
                    {
                        dwnetcase.version=get_text();
                        dwnetcase.colour=maintheme.get_highlight();
                        dwnetcase.Parse_Animations();
                    }
                    else
                        {
                            dwcase.Animations();
                            put_text(dwcase.first);
                            if (!dwcase.errors.isEmpty())
                            {
                                ui->error->setStyleSheet("color: rgba(255,0,0,255);");
                                ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+dwcase.errors+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
                            }
                            dwcase.clearing();
                        }
                    break;

                })
                QSDEFAULT(
                {
                   ui -> error->setStyleSheet("color: rgba(255,0,0,255);");
                   ui -> text2->setText("dw error = \u043a\u043d\u043e\u043f\u043a\u0430 \u0441 \u0442\u0430\u043a\u0438\u043c \u043d\u0430\u0437\u0432\u0430\u043d\u0438\u0435\u043c\n \u043d\u0435 \u043d\u0430\u0439\u0434\u0435\u043d\u0430");break;
                })
                )
}


void error_checker(QString er)
{
    if (er.isEmpty())
    {

    }
}
void MainWindow::set_theme()
{
    QString highlight = maintheme.get_highlight();
    QString text_back = maintheme.get_backcolor();
    QString button_back = maintheme.get_buttoncolor();
    QString button_back_hover = maintheme.get_buttoncolor_hover();
    QString button_back_pressed = maintheme.get_buttoncolor_pressed();
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
        buttons[i] ->setStyleSheet("QPushButton {" + btext + "background-color: rgb" + button_back +";\
                color: rgb" + text +";\
                 border-right-color: rgb" + details + ";} \
                    QPushButton:hover {" + btext + "background-color: rgb" + button_back_hover + ";\
                    color: rgb" + text + ";\
                  border-right-color: rgb" + details + ";} \
                    QPushButton:pressed {" + btext + "background-color: rgb" + button_back_pressed + ";\
                                     color: rgb" + text + ";\
                                   border-right-color: rgb" + details + ";}");
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
         QString bobtext = "QPushButton {border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid; background-color: rgb" + button_back + "; color: rgb" + text + ";} QPushButton:hover {\
                border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid; background-color: rgb" +button_back_hover + "; color: rgb" + text + ";} QPushButton:pressed {\
            border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid; background-color: rgb" + button_back_pressed + "; color: rgb" + text + ";}";
         ui -> buttonpaste ->setStyleSheet(bobtext);
         ui -> buttonpaste_2 ->setStyleSheet(bobtext);
         ui -> buttoncopy ->setStyleSheet(bobtext);
         ui -> buttoncopy_2->setStyleSheet(bobtext);
         ui -> buttochange ->setStyleSheet(bobtext);
         ui -> autozamena->setStyleSheet(maintheme.do_autoz(autoz));


         ui -> settings ->setStyleSheet(bobtext);
         ui -> Clear_left ->setStyleSheet(bobtext);
         ui -> Clear_right ->setStyleSheet(bobtext);
         ui->themebutton->setText(themetext);
         ui->debug->setStyleSheet("color: rgb"+text+";");
         if (maintheme.theme)
         {
             ui->dota2wiki->setIcon(maintheme.wiki_dark);
             ui->excel->setIcon(maintheme.excel_dark);
             ui->discord->setIcon(maintheme.discord_dark);
         }
         else {
             ui->dota2wiki->setIcon(QIcon(":/images/images/wiki.png"));
             ui->excel->setIcon(QIcon(":/images/images/excel.png"));
             ui->discord->setIcon(QIcon(":/images/images/discord.png"));
         }

}

void MainWindow::append_cases()
{
    QJsonObject json = read_json("params.json").object();
    QJsonArray Macros = json["Macros_list"].toArray();
   // for (int i = 0;i<Macros.size();i++)
    foreach(const QJsonValue &v, Macros)
    {
        cases.push_back (v.toString());
    }
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

QString MainWindow::color(QString arg,QString color)
{
    color = maintheme.get_highlight();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    return QString("<span style= \"background:%1\">%2</span>").arg(color,arg);
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
    progress->setValue(0);
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

            ui->error->setStyleSheet("color: rgba(255,0,0,255);");
            ui->debug->setText("dw error:\u043e\u0448\u0438\u0431\u043a\u0430 \u043f\u0440\u0438 <br>\u0447\u0442\u0435\u043d\u0438\u0438 \u0441\u043b\u043e\u0432\u0430\u0440\u044f "+filename+ "<br>\u043f\u0440\u043e\u0432\u0435\u0440\u044c\u0442\u0435 \u0444\u0430\u0439\u043b \u043d\u0430 \u0432\u0430\u043b\u0438\u0434\u043d\u043e\u0441\u0442\u044c!");
          //  Doc = -1;
        }
        qDebug() << Doc.isNull();
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
     //3840 � 2160
     double normw = 1920;
     double normh = 1080;
     double wide = QApplication::desktop()->screenGeometry().width()/normw;
     double hide = QApplication::desktop()->screenGeometry().height()/normh;
     //double wide = 3840/normw;
     //double hide = 2160/normh;

     QWidget *test;
     for (int i = 0;i<centralWidget()->children().size();i++)
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
     //double wide = 3840/normw;
     //double hide = 2160/normh;
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

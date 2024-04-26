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
#include <QSyntaxHighlighter>
#include <QLabel>
#include <QRegExp>
#include <QClipboard>
#include <QThread>
#include <QDesktopWidget>
#include <QTime>
#include <QPainter>
#include <QColor>
#include <QRgb>
#include <QScrollBar>

#include <QtWinExtras/QWinTaskbarProgress>
#include <QtWinExtras/QWinTaskbarButton>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_settings.h"
#include "qsswitch.h"
#include "dwTextComparator.h"
#include "dwjson.h"
#include "macros.h"
#include "dwnetmacros.h"
#include <QTimer>

//#define MCVC
#define RVERSION "1.2.2"

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
    ui->text1->setAcceptRichText(false);
    ui->text2->setAcceptRichText(false);
    this->setStyleSheet("background-image:url()");
    maintheme.size =this->size();
    set_theme();

    from = new ui_settings();

    connect(this, &MainWindow::sendData, from, &ui_settings::recieveData);  //пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅ Form1 пїЅ Form2
    connect(from, &ui_settings::sendData, this, &MainWindow::recieveData);  //пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅ Form2 пїЅ Form1

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
    qDebug() << "progress: " << pr;
    if (pr>-1)
        progress->setValue(pr);
    else
        progress->stop();
}
void MainWindow::receive_netmacros()
{
    qDebug() << "netmacros rececived";
    put_text(&dwnetcase.output, true);
    dwnetcase.cleaning();
}
void MainWindow::recieveData()
{
    set_buttons();
}

void MainWindow::on_dota2wiki_clicked()
{
    QString wikiurl = QStringLiteral(u"https://liquipedia.net/dota2gameru/Участник:X7b3a2j/Rubick_Editor");
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
    x =  QDateTime::currentMSecsSinceEpoch();
    ui->error->setStyleSheet("color: rgba(255,0,0,0);");
    ui->debug->clear();
    progress->resume();
    Macros dwcase(get_text());
    connect(&dwcase,&Macros::new_progress,this,&MainWindow::receive_progress);
    QSSWITCH(switchStr,
                QSCASE(cases[0], //замена точек на запятые
                {
                    dwcase.Commafix();
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    break;
                })
                QSCASE(cases[1], //викификатор
                {
                    dwcase.WikiAndFixes();
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    break;
                })
                QSCASE(cases[2],//изменения
                {
                    dwcase.Changelogs();
                    section = 2;
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    progress->setValue(0);
                    qDebug() <<"progress value:  " <<  progress->value();
                    break;
                })
                QSCASE(cases[3], //Изменения + Коммафикс + викификатор + Units
                {
                    dwcase.Changelogs();
                    dwcase.Commafix();
                    dwcase.WikiAndFixes();
                    dwcase.Units();
                    section = 2;
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    progress->setValue(0);
                    qDebug() <<"progress value:  " <<  progress->value();
                    break;

                })
                QSCASE(cases[4],//"Реплики"
                {
                    dwcase.Responses();
                    section = 3;
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    break;
                })
                QSCASE(cases[5],// "Звуки"
                {
                    dwcase.Sounds();
                    section = 4;
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    break;
                })
                QSCASE(cases[6],//Косметика
                {
                        section = 5;
                        if (get_text().size()<100)
                        {
                            dwnetcase.version=get_text();
                            dwnetcase.colour=maintheme.get_highlight();
                            dwnetcase.Parse_Cosmetic();
                        }
                        else
                            {
                                dwcase.Cosmetics();
                                put_text(&dwcase.first);
                                checkForDictionaryErrors(&dwcase.errors);
                                dwcase.clearing();
                            }
                        break;
                })
                QSCASE(cases[7],//"Units - существа"
                {
                   dwcase.Units();
                   put_text(&dwcase.first);
                   checkForDictionaryErrors(&dwcase.errors);
                   dwcase.clearing();
                   break;
                })
                QSCASE(cases[8], //List of Changed Heroes
                {
                    if (get_text().size()>100) {
                        notLinkMessage();
                        break;
                    }
                    dwnetcase.version=get_text();
                    dwnetcase.Patch_heroes();
                    break;
                })
                QSCASE(cases[9], //Новая версия
                {
                    if (get_text().size()>100) {
                        notLinkMessage();
                        break;
                    }
                    dwnetcase.version=get_text();
                    dwnetcase.language=0;
                    dwnetcase.Patch_Version(0);
                    break;

                })
                QSCASE(cases[10], //New Version
                {
                    if (get_text().size()>100) {
                        notLinkMessage();
                        break;
                    }
                    dwnetcase.version=get_text();
                    dwnetcase.language=1;
                    dwnetcase.Patch_Version(1);
                    break;
                })
                QSCASE(cases[11], //Раздел "Анимации"
                {
                    section = 10;
                    if (get_text().size()<100)
                    {
                        dwnetcase.version=get_text();
                        dwnetcase.colour=maintheme.get_highlight();
                        dwnetcase.Parse_Animations();
                    }
                    else
                        {
                            dwcase.Animations();
                            put_text(&dwcase.first);
                            checkForDictionaryErrors(&dwcase.errors);
                            dwcase.clearing();
                        }
                    break;

                })
                QSCASE(cases[12],
                {
                    section = 11;

                    QString a = QString("Axe\nArmorPhysical 0\nAttackDamageMin 40\nAttackDamageMax 54\nAttributeBaseStrength 25\nAttributeStrengthGain 2.8\nAttributeBaseIntelligence 18\nAttributeIntelligenceGain 1.6\nAttributeBaseAgility 20\nAttributeAgilityGain 1.7\nStatusHealthRegen 2.5\n");
                    a += "\n" + QString("Bane\nArmorPhysical 0\nAttackDamageMin 40\nAttackDamageMax 50\nAttributeBaseStrength 22\nAttributeStrengthGain 2.2\nAttributeBaseIntelligence 22\nAttributeIntelligenceGain 2.2\nAttributeBaseAgility 22\nAttributeAgilityGain 2.2\nStatusHealthRegen 2.2\n");
                    a += "\n" + QString("Bloodseeker\nArmorPhysical 3\nAttackDamageMin 44\nAttackDamageMax 67\nAttributeBaseStrength 23\nAttributeStrengthGain 3.8\nAttributeBaseIntelligence 48\nAttributeIntelligenceGain 5.6\nAttributeBaseAgility 10\nAttributeAgilityGain 3.7\nStatusHealthRegen 3.5\n");
                    put_text(&a);

                    dwcase.clearing();

                })
                QSCASE(cases[13],
                {
                    section = 12;
                    dwcase.AbilitySwapper();
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    break;
                })
                QSCASE(cases[14],
                {
                    section = 13;
                    dwcase.ValueNumberChanger(getSecondaryText());
                    put_text(&dwcase.first);
                    checkForDictionaryErrors(&dwcase.errors);
                    dwcase.clearing();
                    break;

                })
                QSDEFAULT(
                {
                   ui -> error->setStyleSheet("color: rgba(255,0,0,255);");
                   ui -> debug->setText(QStringLiteral(u"dw error: кнопка с таким названием\n не найдена"));
                   break;
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
    QString details_hover = maintheme.get_details_hover();
    QPalette palette = maintheme.get_backimage();
    QString background;
    QString border = maintheme.get_border();
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
                ui->Input_win->setStyleSheet("QPushButton {"+dtext+details+";} QPushButton:hover {" + dtext + details_hover + ";}");
                ui->Output_win->setStyleSheet("QPushButton {"+dtext+details+";} QPushButton:hover {" + dtext + details_hover + ";}");
                ui->Input_label->setStyleSheet(dtext+details+";");
                ui->Output_label->setStyleSheet(dtext+details+";");
                ui->themebutton -> setStyleSheet("QPushButton {"+dtext+details+";} QPushButton:hover {" + dtext + details_hover + ";}");
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
         ui->themebutton->setText(maintheme.get_themetext());
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

QString MainWindow::getSecondaryText()
{
    QString first;
    if (input)
        first = ui -> text1-> toPlainText();
    else
        first = ui -> text2-> toPlainText();
    first.replace("<", "&lt;");
    first.replace(">", "&gt;");
    return first;
}

void MainWindow::put_text(QString* text, bool isNetMacros, bool hasParameters)
{
    text->replace("\n","<br>");
    *text =  "<html>" + *text + "</html>";

    QString textBeforeEdits = get_text();
    textBeforeEdits.replace("\n","<br>");
    textBeforeEdits =  "<html>" + textBeforeEdits + "</html>";

    QTextEdit* inputTextEdit = input?ui->text2:ui->text1;
    QTextEdit* outputTextEdit = output?ui->text2:ui->text1;
    if (!hasParameters)
    {
        if (output==input||!autoz)
        {
           dwTextComparator comparator(inputTextEdit, outputTextEdit, &textBeforeEdits, text);
           label_settext(comparator.compareText(isNetMacros?-1:maintheme.theme));
        }
        else if (output!=input&&autoz)
        {
            dwTextComparator comparator(outputTextEdit, inputTextEdit, &textBeforeEdits, text);
            label_settext(comparator.compareText(isNetMacros?-1:maintheme.theme));
        }
    }

    progress->setValue(0);
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
            ui->debug->setText(QStringLiteral(u"dw error:ошибка при <br>чтении словаря ")+filename+ QStringLiteral(u"<br>проверьте файл на валидность!"));
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
    QString endstring;
    if (count%10==1&&count!=11)
        ends = 1;
    if (count%10>1&&count%10<5&&(count<11||count>15))
        ends = 2;
    switch (ends)
    {
        case 1: endstring = QStringLiteral(u"изменение");break;
        case 2: endstring = QStringLiteral(u"изменения");break;
        default: endstring = QStringLiteral(u"изменений"); break;
    }
    QString text = QStringLiteral(u"С последним макросом совершено ");
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
    ui->Input_win->setText(input?QStringLiteral(u"Ввод: окно справа"):QStringLiteral(u"Ввод: окно слева"));
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
    ui->Output_win->setText(!output?QStringLiteral(u"Вывод: окно слева"):QStringLiteral(u"Вывод: окно справа"));
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
     //3840 х 2160
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

void MainWindow::notLinkMessage()
{
    ui->error->setStyleSheet("color: rgba(255,0,0,255);");
    ui->debug->setText(QStringLiteral(u"dw error:Слишком длинное<br>название "));
}

void MainWindow::checkForDictionaryErrors(QString *errors)
{
    QString validation =  QStringLiteral(u"<br>проверьте файл на валидность!");
    if (!errors->isEmpty())
         {
             ui->error->setStyleSheet("color: rgba(255,0,0,255);");
             ui->debug->setText(QStringLiteral(u"dw error:ошибка при <br>чтении словаря ")+errors+validation);
         }
}

void MainWindow::on_autozamena_clicked()
{
    autoz = autoz?0:1;
    QString str_autoz[2]=  {QStringLiteral(u"Автозамена (выкл)"), QStringLiteral(u"Автозамена (вкл)")};
    ui->autozamena->setText(str_autoz[autoz]);
    ui->autozamena->setStyleSheet(maintheme.do_autoz(autoz));
}

void MainWindow::end()
{

}

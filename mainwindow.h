#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "dwtheme.h"
#include "dwjson.h"
#include "dwnetmacros.h"
//#include "dwsetter.h"
#include "ui_settings.h"
#include <QtWinExtras/QWinTaskbarProgress>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    QVector<QPushButton*> buttons;
    QWinTaskbarProgress *progress;
    ui_settings *from;
    dwNetMacros dwnetcase;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool refresh=1;
    Ui::MainWindow *ui;
    QVector<QString> cases;
/*virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);*/
signals:
    void sendData(QString);
public slots:
    void recieveData(QString);
private:
    bool input = 0;
    bool output = 1;
    int theme = 0;
    QJsonDocument read_json(QString filename);
    void button_switch(QString);
    void label_settext(int);
    void append_buttons();
    void append_cases();
    void set_buttons();
    void put_text(QString text);
    QString get_text();
    QString color(QString arg,QString color="#ff8f45");
    void set_theme();
    void adaptive_screen();
    void set_fonts();
    void set_progressbar();
    dwTheme maintheme;
    int autoz = 0;

private slots:
    void receive_netmacros();
    void receive_progress(int);
    void on_buttochange_clicked();
    void on_settings_clicked();
    void on_dota2wiki_clicked();
    void end();
    void on_button1_clicked();
    void on_button2_clicked();
    void on_button3_clicked();
    void on_button4_clicked();
    void on_button5_clicked();
    void on_buttonpaste_clicked();
    void on_buttoncopy_clicked();
    void on_Input_win_clicked();
    void on_Output_win_clicked();
    void on_themebutton_clicked();
    void on_Clear_left_clicked();
    void on_Clear_right_clicked();
    void on_buttonpaste_2_clicked();
    void on_buttoncopy_2_clicked();
    void on_excel_clicked();
    void on_discord_clicked();
    //void on_pushButton_clicked();
   // void on_backz_clicked();
    void on_autozamena_clicked();
};
#endif // MAINWINDOW_H

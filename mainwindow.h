#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include <QTextEdit>
#

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool refresh=1;
    Ui::MainWindow *ui;
    QString cases[7] = {"\u0412\u0438\u043a\u0438\u0444\u0438\u043a\u0430\u0442\u043e\u0440\u0020\u0438\u0020\u0444\u0438\u043a\u0441\u044b", //wikifikator
                        "\u0417\u0430\u043c\u0435\u043d\u0430\u0020\u0442\u043e\u0447\u0435\u043a\u0020\u043d\u0430\u0020\u0437\u0430\u043f\u044f\u0442\u0443\u044e", //commafix
                       "\u0420\u0430\u0437\u0434\u0435\u043b\u0020\u0022\u0418\u0437\u043c\u0435\u043d\u0435\u043d\u0438\u044f\u0022", //changelog
                       "\u0420\u0430\u0437\u0434\u0435\u043b\u0020\u0022\u0420\u0435\u043f\u043b\u0438\u043a\u0438\u0022", //responses
                       "\u0420\u0430\u0437\u0434\u0435\u043b\u0020\u0022\u0417\u0432\u0443\u043a\u0438\u0022", //sounds
                       "\u0420\u0430\u0437\u0434\u0435\u043b\u0020\u0022\u041a\u043e\u0441\u043c\u0435\u0442\u0438\u043a\u0430\u0022", //comsetic
                       "\u0055\u006e\u0069\u0074\u0073\u0020\u002d\u0020\u0421\u0443\u0449\u0435\u0441\u0442\u0432\u0430"}; //units
private:
    int counter(QString);
    QJsonDocument read_json(QString filename);
    void button_switch(QString);
    void label_settext(int);
    void set_buttons();
    void put_text(QString text);
    QString get_text();
    QString color(QString color, QString arg);
    QString get_backtext();
private slots:
    void on_buttochange_clicked();
    void on_settings_clicked();
    void on_refreshButton_clicked();
    void on_dota2wiki_clicked();

    void WikiAndFixes();
    void Commafix();
    void Changelogs();
    void Responses();
    void Sounds();
    void Cosmetics();
    void Units();

    void on_button1_clicked();
    void on_button2_clicked();
    void on_button3_clicked();
    void on_button4_clicked();
    void on_button5_clicked();
    void on_buttonpaste_clicked();
    void on_buttoncopy_clicked();
};
#endif // MAINWINDOW_H

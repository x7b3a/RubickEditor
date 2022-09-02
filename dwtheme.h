#ifndef DWTHEME_H
#define DWTHEME_H

#include <QMainWindow>
//#include "mainwindow.h"
class dwTheme : public QMainWindow
{
    Q_OBJECT
public:
    int theme = 0;
    QSize size;
    //MainWindow win;
    explicit dwTheme(QWidget *parent = nullptr);
    QString get_themetext();
    QString get_highlight();
    QString get_backcolor();
    QString get_buttoncolor();
    QString get_textcolor();
    QString get_details();
    QPalette get_backimage();
    QString get_border();
    QString get_buttontextcolor();
    QString do_autoz(int);
signals:

};

#endif // DWTHEME_H

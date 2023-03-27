#include "dwtheme.h"
#include <QMainWindow>
#include <QPainter>

dwTheme::dwTheme(QWidget *parent) : QMainWindow(parent)
{
   colorize_image(wiki_dark, ":/images/images/wiki_trans.png", 104,123,200);
   colorize_image(excel_dark, ":/images/images/excel_trans.png", 114,132,203);
   colorize_image(discord_dark, ":/images/images/discord.png", 114,132,203);
}

void dwTheme::colorize_image(QPixmap &image, QString path, int r, int g, int b)
{
    image = QPixmap(path);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(image.rect(),qRgb(r,g,b));
    painter.end();
}

QString dwTheme::get_themetext()
{
    switch (theme)
    {
        case 0: return "\u0421\u0432\u0435\u0442\u043b\u0430\u044f \u0442\u0435\u043c\u0430";break;
        case 1: return "\u0422\u0451\u043c\u043d\u0430\u044f \u0442\u0435\u043c\u0430"; break;
        default: return "\u0421\u0432\u0435\u0442\u043b\u0430\u044f \u0442\u0435\u043c\u0430";break;
    }
}
QString dwTheme::get_border()
{
    switch (theme)
    {
    case 0: return "(127, 127, 127)"; break;
    case 1: return "(222, 222, 222)"; break;
    default: return "(127, 127, 127)"; break;
    }
}
QPalette dwTheme::get_backimage()
{
    QPalette palette;
    switch (theme)
    {
        case 0:  palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/images/back3.png").scaled(size,Qt::IgnoreAspectRatio)));; break;
        case 1:  palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/images/back_dark4.png").scaled(size,Qt::IgnoreAspectRatio))); break;
        default:  palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/images/back3.png").scaled(size,Qt::IgnoreAspectRatio))); break;
    }
    return palette;
}
QString dwTheme::get_highlight()
{
    switch (theme)
    {
    case 0: return "#ff8f45"; break;
    case 1: return "#b26430"; break;
    //case 1: return "#d05300"; break;
    default: return "#ff8f45"; break;
    }
}
QString dwTheme::get_backcolor()
{
    switch (theme)
    {
    case 0: return "(255,255,255)"; break;
    case 1: return "(28, 28, 28)"; break;
    default: return "(255,255,255)"; break;
    }
}
QString dwTheme::get_buttoncolor()
{
    switch (theme)
    {
    case 0: return "(230, 230, 230)"; break;

        //case 1: return "(75,105,255)"; break;
        case 1: return "(63, 86, 173)"; break;
    //case 1: return "(2, 0, 98)"; break;
    default: return "(243, 243, 243)"; break;
    }
}
QString dwTheme::get_buttoncolor_hover()
{
    switch (theme)
    {
    case 0: return "(250, 250, 250)"; break;

        //case 1: return "(75,105,255)"; break;
        case 1: return "(83, 106, 193)"; break;
    //case 1: return "(2, 0, 98)"; break;
    default: return "(250, 250, 250)"; break;
    }
}
QString dwTheme::get_buttoncolor_pressed()
{
    switch (theme)
    {
    case 0: return "(210, 210, 210)"; break;

        //case 1: return "(75,105,255)"; break;
        case 1: return "(63, 46, 153)"; break;
    //case 1: return "(2, 0, 98)"; break;
    default: return "(210, 210, 210)"; break;
    }
}
QString dwTheme::get_textcolor()
{
    switch (theme)
    {
    case 0: return "(0,0,0)"; break;
    case 1: return "(203, 203, 203)"; break;
    default: return "(0,0,0)"; break;
    }
}
QString dwTheme::get_buttontextcolor()
{
    switch (theme)
    {
    case 0: return "(0,0,0)"; break;
    case 1: return "(243, 255, 255)"; break;
    default: return "(0,0,0)"; break;
    }
}
QString dwTheme::get_details()
{
    switch (theme)
    {
    case 0: return "(255, 191, 102)"; break;
    case 1: return "(209, 124, 249)"; break;
    default: return "(255, 191, 102)"; break;
    }
}
QString dwTheme::do_autoz(int onoff)
{
    QString details = get_details();
    QString button_back = get_buttoncolor();
    QString button_back_hover = get_buttoncolor_hover();
    QString text = get_buttontextcolor();
    QString text_autoz =  "QPushButton {border-color: rgb" + details + "; border-width: 1px;\n    border-style: solid  ; border-width: 4px; border-radius: 4px;\n    border-style: solid;background-color: rgb" + button_back + "; color: rgb" + text + ";} QPushButton:hover {border-color: rgb" + details + "; border-width: 1px;\n    border-style: solid  ; border-width: 4px; border-radius: 4px;\n    border-style: solid;background-color: rgb" + button_back_hover + "; color: rgb" + text + ";}";
    QString text_noautoz = "QPushButton {border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;background-color: rgb" + button_back + "; color: rgb" + text + ";} QPushButton:hover {border-color: rgb(127, 127, 127); border-width: 1px;\n    border-style: solid;background-color: rgb" + button_back_hover + "; color: rgb" + text + ";}";
    switch (onoff)
    {
        case 0: return text_noautoz;
        case 1: return text_autoz;
        default: return text_noautoz;
    }
}

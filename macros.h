#include <QString>
#include <QObject>
#include "dwjson.h"
#ifndef MACROS_H
#define MACROS_H

class Macros : public QObject
{
    Q_OBJECT
public:
    explicit Macros();
    ~Macros();
    Macros(QString);
    void clearing();
    dwJson dwJ;
    QString first;
    int counted=0;
    QString errors="";
    void put_text(QString text);
    QString start_regular_replacer (QString);
    void end_regular_replacer (QString *);
    QString get_text();

    void label_settext(int);
    QString get_backtext();

    void send_progress(int);
    void Commafix();
    void WikiAndFixes();
    void Changelogs();
    void Responses();
    void Sounds();
    void Cosmetics();
    void Units();
    void Animations();
signals:
    void new_progress(int);

};

#endif // MACROS_H

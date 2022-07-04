#ifndef DWSNAPSHOT_H
#define DWSNAPSHOT_H
#include <QMainWindow>

class dwSnapshot
{
public:
    dwSnapshot();
    dwSnapshot(QString, QString);
private:
    QString left;
    QString right;
};

#endif // DWSNAPSHOT_H

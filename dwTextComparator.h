#ifndef TEXTCOMPARATOR_H
#define TEXTCOMPARATOR_H

#include <QObject>
#include <QTextEdit>

class dwTextComparator : public QObject {
    Q_OBJECT
public:
    dwTextComparator(QTextEdit* textEdit1, QTextEdit* textEdit2, QString* text1, QString* text2);
    ~dwTextComparator();
    int compareText(int theme = 1); //0 - light, 1 - dark

private:
    QTextEdit* textEdit1;
    QTextEdit* textEdit2;
    QString* text1;
    QString* text2;
};

#endif // TEXTCOMPARATOR_H

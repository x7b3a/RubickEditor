#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSyntaxHighlighter>

namespace Hi {
class MainWidget;
}

class SyntaxHighlighter;

// ********************************************************************************

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = 0 );
    ~MainWidget();

private slots:
    void onFind();

private:
    Hi::MainWidget* ui;

    SyntaxHighlighter* m_highlighter;
};

// ********************************************************************************

class SyntaxHighlighter : public QSyntaxHighlighter {

public:
    SyntaxHighlighter( QTextDocument* parent );

    void highlightBlock( const QString& text );

    void setHighlightedString( const QString& str );

private:
    QString m_highlightedString;

};

#endif // MAINWIDGET_H

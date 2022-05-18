#include "mainwidget.h"
#include "mainwindow.ui"

// ********************************************************************************

MainWidget::MainWidget( QWidget* parent ) :
    QWidget( parent ),
   ui( new Hi::MainWidget ) {
    ui->setupUi( this );

    m_highlighter = new SyntaxHighlighter( ui->txt->document() );

    connect( ui->bnFind, SIGNAL( clicked( bool ) ), SLOT( onFind() ) );
}

MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::onFind() {
    if( m_highlighter ) {
        m_highlighter->setHighlightedString( ui->edSearchInput->text() );
    }
}

// ********************************************************************************

SyntaxHighlighter::SyntaxHighlighter( QTextDocument* parent ) :
    QSyntaxHighlighter( parent ) {
}

void SyntaxHighlighter::highlightBlock( const QString& text ) {
    if( m_highlightedString.isEmpty() ) {
        return;
    }

    QTextCharFormat fmt;
    fmt.setBackground( Qt::yellow );

    const int LEN = m_highlightedString.length();
    for(
        int index = text.indexOf( m_highlightedString );
        0 <= index;
        index = text.indexOf( m_highlightedString, index + LEN )
    ) {
        setFormat( index, LEN, fmt );
    }
}

void SyntaxHighlighter::setHighlightedString( const QString& str ) {
    m_highlightedString = str;
    rehighlight();
}

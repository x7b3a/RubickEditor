#include "html_parser.h"
#include <QString>
#include <QNetworkReply>
HTML_Parser::HTML_Parser(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)) {
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onPage_loaded(QNetworkReply*)));
}


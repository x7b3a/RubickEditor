#include "html_parser.h"
#include <QString>
#include <QNetworkReply>
HTML_Parser::HTML_Parser(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)) {
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onPage_loaded(QNetworkReply*)));
}
void HTML_Parser::parse(QString url) {
    qDebug() << "parse";
    QNetworkRequest request(url);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    m_manager->post(request, "");
}
void HTML_Parser::onPage_loaded(QNetworkReply *reply) {
    qDebug() << "onPage_loaded()";
    QString buff = reply->readAll();
    qDebug() << buff;
    QRegExp iconsRegExp("<div class=\"tabl_td\".*<a href=\"//([^\"]+)\".*<img src=\"//([^\"]+)\".*([^<]+)<br>([^<]+)<");
    iconsRegExp.setMinimal(true);
    //QList<IconData> icons;
    int lastPos = 0;
   /* while ((lastPos = iconsRegExp.indexIn(buff, lastPos)) != -1) {
       /* IconData iconData;
        lastPos += iconsRegExp.matchedLength();
        iconData.urlImagePage = iconsRegExp.cap(1);
        iconData.urlIconSrc = iconsRegExp.cap(2);
        iconData.datetime = iconsRegExp.cap(3);
        iconData.size = iconsRegExp.cap(4);
        qDebug() << iconData.urlIconSrc << iconData.urlImagePage
                 << iconData.datetime << iconData.size;
        icons.push_back(iconData);
    }*/
    emit finished(1);
    reply->deleteLater();
}

#ifndef HTML_PARSER_H
#define HTML_PARSER_H
#include <QObject>
#include <QNetworkAccessManager>
#endif // HTML_PARSER_H
class HTML_Parser : public QObject {
  Q_OBJECT
  public:
      explicit HTML_Parser(QObject *parent = 0);
  public slots:
      void parse(QString url);
  private slots:
      void onPage_loaded(QNetworkReply*);
  signals:
      void finished(int);
  private:
      QNetworkAccessManager *m_manager;
};

#include "ui_settings.h"
#include "ui_ui_settings.h"
#include <QtGui>
#include <QComboBox>
#include <QDebug>

ui_settings::ui_settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_settings)
{
    ui->setupUi(this);
    this->setWindowTitle("\u041d\u0430\u0441\u0442\u0440\u043e\u0439\u043a\u0438\u0020\u043a\u043d\u043e\u043f\u043e\u043a");
    this->setWindowIcon(QIcon(":/images/images/Rubick_icon.webp"));
    QString val;
    QFile file;
    file.setFileName("config2.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray Macros = json["Macros"].toArray();
    for (int i=0;i<5;i++)
    {
        Macros[i].toString() = json.value(QString("Macros")).toString();
    }
    ui -> comboBox -> setCurrentText(Macros[0].toString());
    ui -> comboBox_2 -> setCurrentText(Macros[1].toString());
    ui -> comboBox_3 -> setCurrentText(Macros[2].toString());
    ui -> comboBox_4 -> setCurrentText(Macros[3].toString());
    ui -> comboBox_5 -> setCurrentText(Macros[4].toString());
}

ui_settings::~ui_settings()
{
    delete ui;
}

void ui_settings::on_close_clicked()
{
    qDebug() << "click";
    QJsonObject recordObject;
     QJsonArray stringArray;
     stringArray.push_back(QJsonValue::fromVariant(ui -> comboBox -> currentText().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> comboBox_2 -> currentText().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> comboBox_3 -> currentText().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> comboBox_4 -> currentText().toUtf8()));
     stringArray.push_back(QJsonValue::fromVariant(ui -> comboBox_5 -> currentText().toUtf8()));
     recordObject.insert("Macros",stringArray);
    QFile file;
    QJsonDocument doc(recordObject);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    file.setFileName("config2.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &file );
    stream << jsonString;
          file.close();
  //s  par->ui -> button1 -> setPlaintText("123");
          emit sendData("123");
    qDebug() << "sended?";
    close();
}


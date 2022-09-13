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
    append_combobox();
    add_items();
    set_comboboxes();
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
     for (int i=0;i<5;i++)
         stringArray.push_back(QJsonValue::fromVariant(comboboxes[i]->currentText().toUtf8()));
     recordObject.insert("Macros",stringArray);
    QFile file;
    QJsonDocument doc(recordObject);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    file.setFileName("config2.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &file );
    stream << jsonString;
          file.close();
          emit sendData("123");
    qDebug() << "sended?";
    close();
}

void ui_settings::append_combobox()
{
    comboboxes.append(ui->comboBox);
    comboboxes.append(ui->comboBox_2);
    comboboxes.append(ui->comboBox_3);
    comboboxes.append(ui->comboBox_4);
    comboboxes.append(ui->comboBox_5);
}
void ui_settings::set_comboboxes()
{
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
        comboboxes[i]->setCurrentText(Macros[i].toString());
    }

}

void ui_settings::add_items()
{
    for (int i=0;i<5;i++)
    {
        QString val;
        QFile file;
        file.setFileName("params.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject json = doc.object();
        QJsonArray Macros = json["Macros_list"].toArray();
       // for (int i = 0;i<Macros.size();i++)
        foreach(const QJsonValue & v, Macros)
        {
            comboboxes[i]->addItem(v.toString());
        }
    }

}

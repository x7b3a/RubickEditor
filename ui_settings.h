#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QWidget>
#include <QDialog>
#include <QApplication>
#include <QComboBox>
namespace Ui {
class ui_settings;
}

class ui_settings : public QWidget
{
    Q_OBJECT

public:
    explicit ui_settings(QWidget *parent = nullptr);

    QVector<QComboBox*> comboboxes;

    ~ui_settings();

signals:
    void sendData(QString);

public slots:
    void recieveData(QString);

private slots:
    void on_comboBox_activated(const QString &arg1);
    void on_close_clicked();

private:
    Ui::ui_settings *ui;
    void append_combobox();
    void set_comboboxes();
    void add_items();

};

#endif // UI_SETTINGS_H

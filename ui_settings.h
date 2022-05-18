#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QWidget>
#include <QApplication>

namespace Ui {
class ui_settings;
}

class ui_settings : public QWidget
{
    Q_OBJECT

public:
    explicit ui_settings(QWidget *parent = nullptr);
    ~ui_settings();


private slots:
    void on_toolButton_triggered(QAction *arg1);

    void on_comboBox_activated(const QString &arg1);


    void on_pushButton_clicked();

    void on_close_clicked();

private:
    Ui::ui_settings *ui;

};

#endif // UI_SETTINGS_H

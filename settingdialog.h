#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    double getTargetHour() const;
    // 直接把Dialog的ui设为 public
    Ui::SettingDialog *ui;

private:
private slots:
    void on_btn_yes_clicked();
    void on_btn_no_clicked();
};

#endif // SETTINGDIALOG_H

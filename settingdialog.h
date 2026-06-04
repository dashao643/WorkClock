#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <dataStructure.h>

#include <QDialog>
#include <QListWidgetItem>
// #include <QSqlDatabase>
// #include <QSqlQuery>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(const Config_t &config, QWidget *parent = nullptr);
    ~SettingDialog();

    double getTargetHour() const;
    QVector<TargetItem_t> getTargetVec() const;

signals:
    void sgn_addField(QString fieldName);
    void sgn_updateTarget(QString fieldName);

private slots:
    void on_btn_addTarget_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    // QSqlDatabase db_;
    // QSqlQuery *query_;

    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H

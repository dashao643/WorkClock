#include "timedialog.h"
#include "ui_timedialog.h"

TimeDialog::TimeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimeDialog) 
{
    ui->setupUi(this);
    this->setWindowTitle("settings");

    ui->spinBox->setValue(1);
}

int TimeDialog::getChangeMinutes()
{
    return ui->spinBox->value();
}

void TimeDialog::on_btn_ok_clicked()
{
    this->accept();
}

void TimeDialog::on_btn_cancel_clicked()
{
    this->reject();
}


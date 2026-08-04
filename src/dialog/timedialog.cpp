#include "timedialog.h"
#include "ui_timedialog.h"

TimeDialog::TimeDialog(QWidget *parent, bool isDirectExit, QKeySequence showHideHotkey)
    : QDialog(parent)
    , ui(new Ui::TimeDialog) 
{
    ui->setupUi(this);
    this->setWindowTitle("settings");

    ui->spinBox->setValue(0);

    ui->rdBtn_exit->setChecked(isDirectExit);
    ui->rdBtn_tray->setChecked(!isDirectExit);

    ui->keySequenceEdit->setClearButtonEnabled(true);
    ui->keySequenceEdit->setMaximumSequenceLength(1);
    ui->keySequenceEdit->setKeySequence(showHideHotkey);
}

int TimeDialog::getChangeMinutes()
{
    return ui->spinBox->value();
}

bool TimeDialog::getIsDirectExit()
{
    return ui->rdBtn_exit->isChecked();
}

QKeySequence TimeDialog::getShowHideHotkey()
{
    return ui->keySequenceEdit->keySequence();
}

void TimeDialog::on_btn_ok_clicked()
{
    this->accept();
}

void TimeDialog::on_btn_cancel_clicked()
{
    this->reject();
}


#include "settingdialog.h"
#include "ui_settingdialog.h"

#include <QStyleFactory>

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent), 
    ui(new Ui::SettingDialog) 
{
    ui->setupUi(this);
    this->setStyle(QStyleFactory::create("Fusion"));
}

// 如果槽函数命名为on_控件名_信号名，不用手动connect,Qt会自动绑定
void SettingDialog::on_btn_yes_clicked() 
{ 
    this->accept(); 
}

void SettingDialog::on_btn_no_clicked() 
{
    this->reject(); 
}

double SettingDialog::getTargetHour() const 
{
    return ui->doubleSpinBox->value();
}

SettingDialog::~SettingDialog() 
{ 
    delete ui; 
}

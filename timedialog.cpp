#include "timedialog.h"
#include "ui_timedialog.h"

TimeDialog::TimeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TimeDialog) 
{
    ui->setupUi(this);
}

TimeDialog::~TimeDialog() 
{ 
    delete ui; 
}

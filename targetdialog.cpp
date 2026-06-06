#include "settingdialog.h"
#include "ui_settingdialog.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSqlError>

SettingDialog::SettingDialog(const Config_t &config, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    ui->doubleSpinBox->setValue(config.targetHour);

    // 根据容器内的目标，初始化ListWidget
    foreach (TargetItem_t targetItem, config.targetVec) {
        QListWidgetItem *item = new QListWidgetItem(targetItem.targetName);
        if(targetItem.isComplete){
            item->setCheckState(Qt::Checked);
            item->setBackground(QColor(0,170,0));
        }
        else
            item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
    };
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

double SettingDialog::getTargetHour() const
{
    return ui->doubleSpinBox->value();
}

QVector<TargetItem_t> SettingDialog::getTargetVec() const
{
    QVector<TargetItem_t> vec;
    for(int i = 0; i < ui->listWidget->count(); i++){
        TargetItem_t item;
        item.targetName = ui->listWidget->item(i)->text();
        if(ui->listWidget->item(i)->checkState() == Qt::Checked)
            item.isComplete = true;
        else
            item.isComplete = false;
        vec.push_back(item);
    }
    return vec;
}

void SettingDialog::on_btn_addTarget_clicked()
{
    bool isOk = false;
    QString str = QInputDialog::getText(this, "对话框", "请输入文本",
                                        QLineEdit::Normal, "每日目标：", &isOk);
    if(!isOk) return;
    if(str.isEmpty() && isOk){
        QMessageBox::information(this, "提示", "目标为空，添加失败");
        return;
    }
    if (str.length() > 20) {
        QMessageBox::warning(this, "提示", "名称请控制在20个字符内");
        return;
    }
    QRegularExpression re("^[a-zA-Z_][a-zA-Z0-9_]*$");
    if (!re.match(str).hasMatch()) {
        QMessageBox::warning(this, "提示", "名称只能以字母或下划线开头，且只能包含字母、数字、下划线");
        return;
    }
    if (keywords.contains(str.toLower())) {
        QMessageBox::warning(this, "提示", "名称不能使用SQL保留字");
        return;
    }
    // 遍历list查看是否文本重复
    if(!ui->listWidget->findItems(str, Qt::MatchExactly).empty()){
        QMessageBox::information(this, "提示", "重复添加，添加失败");
        return;
    }
    QListWidgetItem *item = new QListWidgetItem(str);
    item->setCheckState(Qt::Unchecked);
    ui->listWidget->addItem(item);

    sgn_addField(str);
}

void SettingDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked){
        QMessageBox::information(this, "提示", "此目标今日已打卡");
        return;
    }
    auto res = QMessageBox::question(this, "提示", "今日要打卡此目标吗?",
                                     QMessageBox::Save|QMessageBox::Cancel,
                                     QMessageBox::Save);
    if(res == QMessageBox::Save){
        item->setCheckState(Qt::Checked);
        item->setBackground(QColor(0,170,0));

        sgn_updateTarget(item->text());
    }
}

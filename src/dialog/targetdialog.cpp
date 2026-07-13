#include "targetdialog.h"
#include "ui_targetdialog.h"
#include "tmessagebox.h"

#include <QDate>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlError>

TargetDialog::TargetDialog(const QStringList &targetList, QSqlQuery *query, QWidget *parent)
    : QDialog(parent)
    , targetFlagV_(3, false)
    , ui(new Ui::TargetDialog)
{
    ui->setupUi(this);

    query_ = query;
    todayStr_ = QDate::currentDate().toString("yyyy-MM-dd");
    itemColor_ = QColor(144, 238, 144);
    itemIcon_ = QIcon(":/picture/target.png");
    itemNIcon_ = QIcon(":/picture/target_n.png");

    listWidgetInit(targetList);
}

TargetDialog::~TargetDialog()
{
    delete ui;
}

QStringList TargetDialog::getTargetList()
{
    QStringList list;
    int cnt = ui->listWidget->count();
    for(int i = 0; i < cnt; i++){
        list.push_back(ui->listWidget->item(i)->text());
    }
    return list;
}

void TargetDialog::on_btn_addTarget_clicked()
{
    if(ui->listWidget->count() == 3) {
        QMessageBox::information(this, "提示", "暂不支持大于3个目标");
        return;
    }

    bool isOk = false;
    QString str = QInputDialog::getText(this, "添加目标", "请输入文本",
                                        QLineEdit::Normal, "每日目标：", &isOk);
    if(!isOk) return;
    if(str.isEmpty() && isOk){
        QMessageBox::information(this, "提示", "输入为空，添加失败");
        return;
    }
    if (str.length() > 20) {
        QMessageBox::warning(this, "提示", "名称请控制在20个字符内");
        return;
    }

    QListWidgetItem *item = new QListWidgetItem(str);
    item->setIcon(itemNIcon_);
    ui->listWidget->addItem(item);
}

void TargetDialog::on_btn_changeTarget_clicked()
{
    int index = ui->listWidget->currentRow();
    if(index < 0) return;

    QString origin = ui->listWidget->currentItem()->text();
    bool isOk = false;
    QString str = QInputDialog::getText(this, "修改目标", "请输入文本",
                                        QLineEdit::Normal, origin, &isOk);
    if(!isOk) return;
    if(str.isEmpty() && isOk){
        QMessageBox::information(this, "提示", "输入为空，修改失败");
        return;
    }
    if (str.length() > 20) {
        QMessageBox::warning(this, "提示", "名称请控制在20个字符内");
        return;
    }
    ui->listWidget->currentItem()->setText(str);
}

void TargetDialog::on_btn_clockIn_clicked()
{
    int index = ui->listWidget->currentRow();
    if(index < 0) return;

    QListWidgetItem *item = ui->listWidget->currentItem();

    if(targetFlagV_.at(index) == true) {
        QMessageBox::information(this, "提示", "此目标今日已打卡");
        return;
    }

    TMessageBox msg("今日要打卡此目标吗?", false);
    msg.exec();
    if (msg.clickedButton() == msg.okButton()) {
        QString sql = QString("insert into record(date, target%1) values('%2', 1)");

        if(!query_->exec(sql.arg(index).arg(todayStr_))){
            QMessageBox::critical(this, "警告", "目标完成情况保存失败：\n"+query_->lastError().text());
            return;
        }
        targetFlagV_[index] = true;
        item->setBackground(itemColor_);
        item->setIcon(itemIcon_);
        // 写入db
        qDebug()<<"目标完成情况保存成功";
    }
}

void TargetDialog::listWidgetInit(const QStringList &targetList)
{
    QString sql = QString("select target%1 from record where date = '%2' ");

    for(int i = 0; i < targetList.size(); i++) {
        // 查询今日是否完成
        if(!query_->exec(sql.arg(i).arg(todayStr_))){
            QMessageBox::critical(this, "警告", "查询目标失败：\n" + query_->lastError().text());
            return;
        }
        QListWidgetItem *item = new QListWidgetItem(targetList.at(i));
        bool isComplete = false;
        /// 读出今日所有此目标记录，存在1，则已打卡
        while(query_->next()){
            if(query_->value(0) == 1){
                isComplete = true;
                break;
            }
        }
        if(isComplete){
            targetFlagV_[i] = true;
            // 设置背景为绿色和打卡图标
            item->setBackground(itemColor_);
            item->setIcon(itemIcon_);
        }
        else{
            item->setIcon(itemNIcon_);
        }
        ui->listWidget->addItem(item);
    }
}

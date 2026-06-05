#include "widget.h"
#include "ui_widget.h"
#include "timedialog.h"
#include "settingdialog.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSqlError>
#include <QSqlRecord>
#include <QScrollBar>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(400, 480);
    qDebug()<<this->width()<<" "<<this->height();
    this->setWindowTitle(QString("工作时钟-v%1").arg(APP_VERSION));
    this->setWindowIcon(QIcon(ICON_CLOCK1));
    ui->tabWidget->setCurrentIndex(0);

    queryModel_ = new RecordModel(this);

    appConfig_ = new AppConfig(this);
    config_ = appConfig_->readConfig();

    timer_ = new QTimer(this);
    timer_->setInterval(1000);
    connect(timer_, &QTimer::timeout, this, &Widget::do_timerTimeout);

    QDate date = QDate::currentDate();
    dateStr = date.toString("yyyy-MM-dd");

    sqliteInit();
    textFileRead();
    uiTimeShowInit();
    uiRecordRefresh();
    uiChartRefresh();
    uiPage4Init();
}

Widget::~Widget()
{
    delete ui;
}

// 初始化sqlite数据库文件
void Widget::sqliteInit()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    query_ = new QSqlQuery(db_);

    QString sqlFilePath = QDir::currentPath() + "/" + DATABASE_FILE_NAME;
    db_.setDatabaseName(sqlFilePath);
    // 运行期间数据库全程打开
    if(!db_.open()){
        QString errorInfo = db_.lastError().text();
        QMessageBox::critical(this, "警告", "数据库打开或创建失败：\n"+ errorInfo);
        return;
    }
    qDebug()<<"数据库打开或创建成功";
    // 初始化数据库表格和字段 (编号/日期/秒数/目标完成情况)
    QString sql = R"(
        create table if not exists record (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            seconds INTEGER DEFAULT 0,
        )
    )";
    if(!query_->exec(sql)){
        QString errorInfo = query_->lastError().text();
        QMessageBox::critical(this, "警告", "数据库初始化失败：\n"+ errorInfo);
        return;
    }
    qDebug()<<"数据库初始化成功";
    // 读出数据库各自定义目标字段完成情况(从第5个字段开始读，暂时这么处理)
    QString query = "select * from record";
    queryModel_->setQuery(query);
    int colCnt = queryModel_->columnCount();

    if(config_.targetVec.size() != colCnt - 4){
        QMessageBox::critical(this, "警告", "数据库字段数错误");
        return;
    }
    QSqlRecord record = queryModel_->record();
    for(int i = 4; i < colCnt; i++){
        QString name = record.fieldName(i);
        stringVec.push_back(name);
        // 查询字段名对应今日完成情况
        QString sql2 = QString("select %1 from record where date = '%2'").arg(name, dateStr);
        if(!query_->exec(sql2)){
            QString errorInfo = query_->lastError().text();
            QMessageBox::critical(this, "警告", "数据库查询失败：\n"+ errorInfo);
            return;
        }
        if(query_->first()){
            config_.targetVec[i-4].isComplete = query_->value(name).toBool();
            qDebug()<<query_->value(name).toBool();
        }
    }
}

// 读取出之前文本文件的记录
void Widget::textFileRead()
{
    if(config_.hasImported){
        qDebug()<<"文本文件已导入";
        return;
    }

    QString testFilePath = QDir::currentPath() + "/EverydayRecord.txt";
    QFile file(testFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"无文件记录";
        return;
    }
    QString content = QString::fromUtf8(file.readAll());
    content = content.trimmed();
    QStringList stringList = content.split('\n', Qt::SkipEmptyParts);

    // 数据库大量操作前开启事务
    if(!db_.transaction()){
        QString errorInfo = db_.lastError().text();
        QMessageBox::critical(this, "警告", "事务开启失败，文本文件未保存进db_：\n"+ errorInfo);
        return;
    }

    // 一条记录
    foreach(QString record, stringList){
        // 记录中的三个字段：日期、秒数、时长
        QStringList field3 = record.split('=',Qt::SkipEmptyParts);

        if (field3.size() < 2) {
            qDebug() << "格式错误：" << record;
            continue;
        }
        // prepare中sql语句只能是一行
        QString sql = "insert into record (date, seconds) values(:date, :seconds)";

        bool ok = query_->prepare(sql);
        if(!ok){
            QString errorInfo = query_->lastError().text();
            QMessageBox::critical(this, "警告", "数据库保存text文件数据失败：\n"+ errorInfo);
            db_.rollback();
            return;
        }
        query_->bindValue(":date", field3.at(0));
        // 先取出秒数字符串
        QString secStr = field3.at(1);
        secStr.remove("秒");
        int sec = secStr.toInt(&ok);

        if (!ok) continue;

        query_->bindValue(":seconds", sec);

        if(!query_->exec()){
            QMessageBox::critical(this, "警告", "数据库插入失败：\n" + query_->lastError().text());
            db_.rollback();
            return;
        }
    }
    if(!db_.commit()){
        QMessageBox::critical(this, "警告", "文本数据导入数据库失败：\n" + db_.lastError().text());
        return;
    }
    QMessageBox::information(this, "提示", "文本数据导入数据库成功");
    // 只读取一次，后写入配置
    config_.hasImported = true;
}

void Widget::uiTimeShowInit()
{
    /********************* 计时时钟 *******************/
    QDate date = QDate::currentDate();
    QStringList weekDays = {"一", "二", "三", "四", "五", "六" ,"日",};
    QString weekStr = weekDays.at(date.dayOfWeek() - 1);
    ui->label_date->setText(date.toString("yyyy年M月d日 星期" + weekStr));

    /********************* 按钮状态 *******************/
    updateTimerState();

    /********************* 今日记录 *******************/
    updateTodayTime();
}

// 使用QSqlTableModel 和 QSqlQueryModel 统计显示每日数据
void Widget::uiRecordRefresh()
{
    QStringList addFields;
    foreach (QString field, stringVec) {
        addFields << QString("MAX(%1) as %1").arg(field);
    }
    QString fieldStr = addFields.isEmpty() ? "" : "," + addFields.join(",");

    QString sql = QString(
        "SELECT date, SUM(seconds) as seconds"
        "FROM record GROUP BY date ORDER BY date asc"
    ).arg(fieldStr);

    queryModel_->setQuery(sql);
    if (queryModel_->lastError().isValid()) {
        QMessageBox::critical(this, "警告", "记录显示失败：\n" + queryModel_->lastError().text());
        return;
    }

    ui->tableView->setModel(queryModel_);
    ui->tableView->scrollToBottom();
}

void Widget::uiChartRefresh()
{

}

void Widget::uiPage4Init()
{

}

void Widget::on_btn_startStop_clicked()
{
    isTiming_ = !isTiming_;
    updateTimerState();
}

void Widget::on_btn_save_clicked()
{
    if (curTimerSeconds_ > 0)
        saveTimerRecord(curTimerSeconds_);

    curTimerSeconds_ = 0;
    ui->label_clock->setText("00:00:00");

    isTiming_ = false;
    updateTimerState();
}

void Widget::on_btn_change_clicked()
{
    TimeDialog dialog(this);
    int res = dialog.exec();
    if (res == QDialog::Accepted) {
        int changeSeconds = dialog.getChangeMinutes() * 60;
        saveTimerRecord(changeSeconds);
    }
}

void Widget::on_btn_reset_clicked()
{
    auto res = QMessageBox::question(this, "询问",
        "确定要重置本次计时吗？",
        QMessageBox::Ok | QMessageBox::Cancel,
        QMessageBox::Cancel
    );
    if(res == QMessageBox::Ok){
        curTimerSeconds_ = 0;
        ui->label_clock->setText("00:00:00");

        isTiming_ = false;
        updateTimerState();
    }
}

void Widget::on_btn_setting_clicked()
{
    SettingDialog dialog(config_, this);

    // 新字段写入数据库
    connect(&dialog, &SettingDialog::sgn_addField, this, [=](QString fieldName){
        QString addField = "alter table record add %1 text";
        if(!query_->exec(addField.arg(fieldName))){
            QMessageBox::critical(this, "警告", "字段添加失败：\n" + query_->lastError().text());
        }
        uiRecordRefresh();
        qDebug()<<"字段添加成功";
    });

    connect(&dialog, &SettingDialog::sgn_updateTarget, this, [=](QString fieldName){
        // 先插入一条1秒的记录,确保今日有记录
        saveTimerRecord(1);
        QString sql = "update record set %1 = 1 where date = '%2'";
        if(!query_->exec(sql.arg(fieldName, dateStr))){
            QMessageBox::critical(this, "警告", "目标完成情况保存失败：\n"+query_->lastError().text());
        }
        uiRecordRefresh();
        qDebug()<<"目标完成情况保存成功";
    });

    dialog.exec();

    config_.targetHour = dialog.getTargetHour();
    config_.targetVec = dialog.getTargetVec();
    // 修改了目标时长，更新数据库的完成状态
    updateTodayTime();
}

void Widget::do_timerTimeout()
{
    curTimerSeconds_++;
    QTime currentTime = QTime(0,0,0).addSecs(curTimerSeconds_);
    ui->label_clock->setText(currentTime.toString("HH:mm:ss"));
}

void Widget::updateTimerState()
{
    if(isTiming_){
        timer_->start();
        ui->btn_startStop->setText("暂停");
    }
    else{
        timer_->stop();
        ui->btn_startStop->setText("开始");
    }
    // 正在计时，或者有计时，保存和重置按钮可点击，否则不可点击
    bool isEnabled = isTiming_ || (curTimerSeconds_ > 0);
    ui->btn_save->setEnabled(isEnabled);
    ui->btn_reset->setEnabled(isEnabled);
}

void Widget::saveTimerRecord(int seconds)
{
    if(seconds == 0) return;

    QString sql = "insert into record (date, seconds) values(:date, :seconds)";
    if(!query_->prepare(sql)){
        QMessageBox::critical(this, "警告", "记录保存失败：\n" + query_->lastError().text());
        return;
    }
    query_->bindValue(":date", dateStr);
    query_->bindValue(":seconds", seconds);
    if(!query_->exec()){
        QMessageBox::critical(this, "警告", "记录保存失败：\n" + query_->lastError().text());
        return;
    }
    // 今日时长刷新显示
    updateTodayTime();
    // //显示打开期间最近一次的保存时间
    // showLatestClock();
}

void Widget::updateTodayTime()
{
    QString sql = "select seconds from record where date = :curDate";
    if(!query_->prepare(sql)){
        ui->label_today->setText("今日记录获取失败");
    }
    query_->bindValue(":curDate", dateStr);
    if(!query_->exec()){
        ui->label_today->setText("今日记录获取失败");
    }
    int totolSeconds = 0;

    while(query_->next()){
        totolSeconds += query_->value("seconds").toInt();
    }
    qDebug()<<totolSeconds;
    if(totolSeconds < 0)
        totolSeconds = 0;

    // 判断今日时长是否完成，写入db
    bool isComplete = totolSeconds > (config_.targetHour * 3600);
    saveTimeTarget(isComplete);

    ui->label_today->setText("今日时长：" + ToHourMinute(totolSeconds));
    uiRecordRefresh();
}

QString Widget::ToHourMinute(int seconds)
{
    QTime time = QTime(0,0,0).addSecs(seconds);
    QString hourMinute = QString("%1小时%2分钟").arg(time.hour()).arg(time.minute());
    return hourMinute;
}

void Widget::saveTimeTarget(bool isComplete)
{
    int val = isComplete;
    // 先查询，若已经写入，则无需提示
    QString select = "select target from record where date = '%1'";
    if(!query_->exec(select.arg(dateStr))){
        QMessageBox::critical(this, "警告", "数据库更新失败：\n"+ query_->lastError().text());
        return;
    }
    if(query_->first()){
        if(query_->value("target").toInt() == val){
            qDebug()<<"无需修改db";
            return;
        }
    }
    QString sql = "update record set target = %1 where date = '%2'";
    if(!query_->exec(sql.arg(val).arg(dateStr))){
        QString errorInfo = query_->lastError().text();
        QMessageBox::critical(this, "警告", "数据库更新失败：\n"+ errorInfo);
    }
    if(val == 1){
        QMessageBox::information(this, "提示", "今日时长已达成！");
    }
    else if(val == 0){
        QMessageBox::information(this, "提示", "目标时长已被修改，今日时长未达成");
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    //判断是否有未保存的本次计时
    bool hasUnsavedTime = (curTimerSeconds_ > 0) || isTiming_;
    if (!hasUnsavedTime) {
        event->accept();
        appConfig_->saveConfig(config_);
        return;
    }

    auto res = QMessageBox::question(this,"提示!",
        "有未保存的计时，是否保存？",
        QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,
        QMessageBox::Save
    );
    // 取消关闭窗口
    if(res == QMessageBox::Cancel){
        event->ignore();
        return;
    }
    else if (res == QMessageBox::Save) {
        saveTimerRecord(curTimerSeconds_);
        event->accept();
    }
    else if (res == QMessageBox::Discard)
        event->accept();
    appConfig_->saveConfig(config_);
}

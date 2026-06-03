#include "widget.h"
#include "ui_widget.h"
#include "timedialog.h"
#include "settingdialog.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QSqlError>
#include <QCloseEvent>

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

    appConfig_ = new AppConfig(this);
    timer_ = new QTimer(this);

    timer_->setInterval(1000);
    connect(timer_, &QTimer::timeout, this, &Widget::do_timerTimeout);

    QDate date = QDate::currentDate();
    dateStr = date.toString("yyyy-MM-dd");

    targetHour_ = appConfig_->getTimeTarget();

    sqliteInit();
    textFileRead();
    uiPage1Init();
    uiPage2Init();
    uiPage3Init();
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
    // 初始化数据库表格和字段 (编号/日期/秒数)
    QString sql = R"(
        create table if not exists record (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          date TEXT NOT NULL,
          seconds INTEGER DEFAULT 0
        )
    )";
    if(!query_->exec(sql)){
        QString errorInfo = query_->lastError().text();
        QMessageBox::critical(this, "警告", "数据库初始化失败：\n"+ errorInfo);
        return;
    }
    qDebug()<<"数据库初始化成功";
}

// 读取出之前文本文件的记录
void Widget::textFileRead()
{
    if(appConfig_->getTextFile()){
        qDebug()<<"文本文件已导入过";
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
    appConfig_->setTextFile(true);
}

void Widget::uiPage1Init()
{
    /********************* 计时时钟 *******************/
    QDate date = QDate::currentDate();
    QStringList weekDays = {"一", "二", "三", "四", "五", "六" ,"日",};
    QString weekStr = weekDays.at(date.dayOfWeek() - 1);
    ui->label_date->setText(date.toString("yyyy年M月d日 星期" + weekStr));

    /********************* 按钮状态 *******************/
    updateTimerState();

    /********************* 今日记录 *******************/
    ui->label_today->setText("今日时长：" + getTodayTime());
}

void Widget::uiPage2Init()
{

}

void Widget::uiPage3Init()
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
    SettingDialog dialog(this);
    dialog.exec();
    targetHour_ = dialog.getTimeTarget();

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
    ui->label_today->setText(getTodayTime());
    // //显示打开期间最近一次的保存时间
    // showLatestClock();
}

QString Widget::getTodayTime()
{
    QString sql = "select seconds from record where date = :curDate";
    if(!query_->prepare(sql)){
        return "今日记录获取失败";
    }
    query_->bindValue(":curDate", dateStr);
    if(!query_->exec()){
        return "今日记录获取失败";
    }
    int totolSeconds = 0;

    while(query_->next()){
        totolSeconds += query_->value("seconds").toInt();
    }
    qDebug()<<totolSeconds;
    // 判断今日时长是否完成，写入db


    return ToHourMinute(totolSeconds);
}

QString Widget::ToHourMinute(int seconds)
{
    QTime time = QTime(0,0,0).addSecs(seconds);
    QString hourMinute = QString("%1小时%2分钟").arg(time.hour()).arg(time.minute());
    return hourMinute;
}

void Widget::saveTimeTarget()
{

}

void Widget::closeEvent(QCloseEvent *event)
{
    //判断是否有未保存的本次计时
    bool hasUnsavedTime = (curTimerSeconds_ > 0) || isTiming_;
    if (!hasUnsavedTime) {
        event->accept();
        appConfig_->setTimeTarget(targetHour_);
        return;
    }

    auto res = QMessageBox::question(this,"提示!",
        "有未保存的计时，是否保存？",
        QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,
        QMessageBox::Save
    );
    // 取消关闭窗口
    if(res == QMessageBox::Ignore){
        event->ignore();
        return;
    }
    else if (res == QMessageBox::Save) {
        saveTimerRecord(curTimerSeconds_);
        event->accept();
    }
    else if (res == QMessageBox::Discard)
        event->accept();
    appConfig_->setTimeTarget(targetHour_);
}

#include "widget.h"
#include "ui_widget.h"
#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "timedialog.h"
#include "ui_timedialog.h"

#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDialog>
#include <QTextStream>
#include <QPainter>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,EverydayRecord(FILE_NAME)
{
    ui->setupUi(this);
    this->resize(420,500);
    //确定合适大小
    connect(ui->btn_startPause,&QPushButton::clicked,this,[=](){
        qDebug()<<this->width()<<" "<<this->height();
    });
    this->setWindowTitle(QString("工作时钟-v%1").arg(APP_VERSION));
    this->setWindowIcon(QIcon(ICON_CLOCK1));
    boqi = new QMovie(NOT_REACH_PIX);
    liang.load(REACH_TARGET_PIX);

    dateInit();
    fileInit();
    clockInit();
    StartBtnInit();
    saveBtnInit();
    resetBtnInit();
    settingBtnInit();
    timeBtnInit();
    timerInit();

    showTextRecord();
}

void Widget::fileInit()
{
    if(QFile::exists(FILE_NAME)){
        qDebug()<<"文件存在";
        if(EverydayRecord.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&EverydayRecord);

            QString firstline = in.readLine().trimmed();
            QStringList parts = firstline.split("=");
            if (parts.size() == 2) {
                QString valuePart = parts[1].trimmed().remove("小时");
                bool ok;
                double targetHour = valuePart.toDouble(&ok);
                if (ok) {
                    m_targetHour = targetHour;
                }
            }

            readTodayRecord(in);
            EverydayRecord.close();
        }
        else
            qDebug()<<"文件打开失败"<<EverydayRecord.errorString();
    }
    else{
        qDebug()<<"文件不存在";
        if(EverydayRecord.open(QIODevice::WriteOnly | QIODevice::Text)){
            qDebug()<<"文件成功创建";
            QTextStream out(&EverydayRecord);
            out << "目标时长=" << m_targetHour << "小时";
            EverydayRecord.close();
        }
        else{
            qDebug()<<"文件创建失败";
            return;
        }
    }

    ui->label_target->setText(QString("%1小时").arg(m_targetHour));
    ui->label_target->setStyleSheet(R"(
        #label_target{
            font-family: "Microsoft YaHei";
            font-size:16px;
            color: #2C3E50;
        }
    )");

    QTime totalTime = QTime(0,0,0).addSecs(m_totalSeconds);
    ui->label_already->setText(QString("%1小时%2分钟")
                                   .arg(totalTime.hour())
                                   .arg(totalTime.minute()));
    ui->label_already->setStyleSheet(R"(
        #label_already{
            font-family: "Microsoft YaHei";
            font-size: 16px;
            color: #2C3E50;
        }
    )");
}

void Widget::clockInit()
{
    ui->label_clock->setText("00:00:00");
    ui->label_clock->setAlignment(Qt::AlignCenter);
    ui->label_clock->setStyleSheet(R"(
        #label_clock{
            font-family: "Microsoft YaHei";
            font-size: 50px;
            font-weight: bold;
            color: #2C3E50;
        }
    )");
}

void Widget::StartBtnInit()
{
    connect(ui->btn_startPause,&QPushButton::clicked,this,[=](){
        if(m_isTiming == false){
            m_timer->start();
            ui->btn_startPause->setText("暂停");
            m_isTiming = true;
            m_isSaveLock = false;
            m_isResetLock = false;
            ui->btn_save->setDisabled(m_isSaveLock);
            ui->btn_reset->setDisabled(m_isResetLock);
        }
        else{
            m_timer->stop();
            ui->btn_startPause->setText("开始");
            m_isTiming = false;
        }
    });
}

void Widget::saveBtnInit()
{
    ui->btn_save->setDisabled(m_isSaveLock);

    connect(ui->btn_save,&QPushButton::clicked,this,[=](){
        if (m_currentTimerSeconds > 0) {
            m_totalSeconds += m_currentTimerSeconds;
            saveClockRecord();
        }

        m_currentTimerSeconds = 0;
        ui->label_clock->setText("00:00:00");
        m_timer->stop();
        ui->btn_startPause->setText("开始");
        m_isTiming = false;
        m_isSaveLock = true;
        m_isResetLock = true;
        ui->btn_save->setDisabled(m_isSaveLock);
        ui->btn_reset->setDisabled(m_isResetLock);
    });
}

void Widget::resetBtnInit()
{
    ui->btn_reset->setDisabled(m_isResetLock);

    connect(ui->btn_reset,&QPushButton::clicked,this,[=](){
        QMessageBox::StandardButton confirmRes = QMessageBox::question(
            this,
            "确认重置",
            "确定要重置本次计时吗？",
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Cancel
            );
        if(confirmRes == QMessageBox::Ok){
            m_currentTimerSeconds = 0;
            ui->label_clock->setText("00:00:00");
            m_timer->stop();
            ui->btn_startPause->setText("开始");
            m_isTiming = false;
            m_isSaveLock = true;
            m_isResetLock = true;
            ui->btn_save->setDisabled(m_isSaveLock);
            ui->btn_reset->setDisabled(m_isResetLock);
        }
        else{
            qDebug()<<"已取消重置";
        }
    });
}

void Widget::settingBtnInit()
{
    connect(ui->btn_setting,&QPushButton::clicked,this,[=](){
        SettingDialog setting(this);
        setting.ui->doubleSpinBox->setValue(m_targetHour);
        int dialogResult = setting.exec();
        if (dialogResult == QDialog::Accepted) {
            m_targetHour = setting.getTargetHour();
            ui->label_target->setText(QString("%1小时").arg(m_targetHour));
            saveTarget();
        }
    });
}

void Widget::timeBtnInit()
{
    connect(ui->btn_time,&QPushButton::clicked,this,[=](){
        TimeDialog time(this);
        int dialogResult = time.exec();
        if (dialogResult == QDialog::Accepted) {
            qDebug()<<"点击了ok";
            int change = time.ui->spinBox->value();
            bool isAdd = time.ui->radioButton_add->isChecked();
            if(isAdd)
                m_currentTimerSeconds += change * 60;
            else
                m_currentTimerSeconds -= change * 60;
        }
    });
}

void Widget::timerInit()
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,&QTimer::timeout,this,&Widget::timerTimeout);
}

void Widget::dateInit()
{
    m_date = QDate::currentDate();
    QStringList weekDays = {"一", "二", "三", "四", "五", "六" ,"日",};
    QString weekDayStr = weekDays[m_date.dayOfWeek() - 1];
    QString dateStr = QString("%1年%2月%3日，星期%4")
                          .arg(m_date.year())
                          .arg(m_date.month())
                          .arg(m_date.day())
                          .arg(weekDayStr);
    ui->label_date->setText(dateStr);
    ui->label_date->setStyleSheet(R"(
        #label_date{
            font-size: 16px;
            font-weight: bold;
            color: #333333;
        }
    )");
}

void Widget::showTextRecord()
{
    ui->plainTextEdit->setReadOnly(true);
    QFile file(FILE_NAME);
    if(file.open(QIODeviceBase::ReadOnly))
        qDebug()<<"读取文本成功";
    else
        qDebug()<<"读取文本失败";
    QByteArray allRecord = file.readAll();
    file.close();
    ui->plainTextEdit->setPlainText(allRecord);
}

void Widget::readTodayRecord(QTextStream &in)
{
    QStringList allLines;
    QString line;
    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            allLines.append(line);
        }
    }

    if (allLines.isEmpty()) {
        qDebug() << "暂无每日记录，今日累计=0";
        m_totalSeconds = 0;
        return;
    }

    QString lastLine = allLines.at(allLines.size() - 1);
    qDebug() << "最后一行记录：" << lastLine;

    QStringList parts = lastLine.split("=");
    if (parts.size() < 2) {
        qDebug() << "记录格式错误，今日累计=0";
        m_totalSeconds = 0;
        return;
    }

    QString dateStr = parts[0].trimmed();
    QDate recordDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    if (!recordDate.isValid() || recordDate != m_date) {
        qDebug() << "最后一行不是今天的记录，今日累计=0";
        m_totalSeconds = 0;
        return;
    }

    QString secondStr = parts[1].trimmed().remove("秒");
    bool ok;
    int totalSeconds = secondStr.toInt(&ok);
    if (ok) {
        m_totalSeconds = totalSeconds;
        qDebug() << "读取今日累计秒数：" << m_totalSeconds;
    } else {
        m_totalSeconds = 0;
        qDebug() << "秒数解析失败，今日累计=0";
    }
}


void Widget::saveTarget()
{
    QStringList allLines;
    if(EverydayRecord.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&EverydayRecord);
        while(!in.atEnd()){
            allLines.append(in.readLine());
        }
        EverydayRecord.close();
    }
    else{
        qDebug()<<"文件未成功打开，保存目标失败"<<EverydayRecord.errorString();
        return;
    }

    if (allLines.isEmpty()) {
        allLines.append("目标时长=" + QString::number(m_targetHour) + "小时");
    } else {
        // 用replace替换，比直接[]更安全
        allLines.replace(0, "目标时长=" + QString::number(m_targetHour) + "小时");
    }

    if(EverydayRecord.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        QTextStream out(&EverydayRecord);
        for (int i = 0; i < allLines.size(); i++){
            out << allLines[i];
            if(i != allLines.size() - 1){
                out << "\n";
            }
        }
        EverydayRecord.close();
    }
}

void Widget::saveClockRecord()
{
    QStringList allLines;
    if(EverydayRecord.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&EverydayRecord);
        while(!in.atEnd()){
            allLines.append(in.readLine().trimmed());
        }
        EverydayRecord.close();
    }

    QTime totalTime = QTime(0,0,0).addSecs(m_totalSeconds);
    QString showText = QString("%1小时%2分钟")
                           .arg(totalTime.hour())
                           .arg(totalTime.minute());
    QString todayDateStr = m_date.toString("yyyy-MM-dd");
    bool hasTodayRecord = false;

    if (!allLines.isEmpty()) {
        QString lastLine = allLines.last();
        if (lastLine.startsWith(todayDateStr)) {
            // 用replace替换最后一行，避免size()-1的索引风险
            allLines.replace(allLines.size() - 1,
                             QString("%1=%2秒=%3")
                                 .arg(todayDateStr)
                                 .arg(m_totalSeconds)
                                 .arg(showText));
            hasTodayRecord = true;
        }
    }

    if (!hasTodayRecord) {
        allLines.append(QString("%1=%2秒=%3")
                            .arg(todayDateStr)
                            .arg(m_totalSeconds)
                            .arg(showText));
    }

    if(EverydayRecord.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        QTextStream out(&EverydayRecord);
        for (int i = 0; i < allLines.size(); i++){
            out << allLines[i];
            if(i != allLines.size() - 1){
                out << "\n";
            }
        }
        EverydayRecord.close();
        qDebug() << "保存今日累计：" << m_totalSeconds << "秒 → " << showText;

        ui->label_already->setText(showText);
    } else {
        qDebug() << "保存今日记录失败：" << EverydayRecord.errorString();
    }
    //保存完了这次的，直接读到展示栏里，不用再重启
    showTextRecord();
}

void Widget::timerTimeout()
{
    m_currentTimerSeconds++;

    QTime currentTime = QTime(0,0,0).addSecs(m_currentTimerSeconds);
    ui->label_clock->setText(currentTime.toString("HH:mm:ss"));

    if (m_currentTimerSeconds <= 0) {
        m_isSaveLock = true;
        ui->btn_save->setDisabled(m_isSaveLock);
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    //判断是否有未保存的本次计时（m_currentTimerSeconds > 0 或正在计时）
    bool hasUnsavedTime = (m_currentTimerSeconds > 0) || m_isTiming;
    if (!hasUnsavedTime) {
        // 无未保存时长，直接关闭
        event->accept();
        return;
    }

    //有未保存时长，弹出询问弹窗
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("提示");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("计时未保存!");

    QPushButton *saveBtn = msgBox.addButton(QMessageBox::Save);
    QPushButton *discardBtn = msgBox.addButton(QMessageBox::Discard);
    QPushButton *cancelBtn = msgBox.addButton(QMessageBox::Cancel);

    msgBox.setDefaultButton(saveBtn);

    //执行弹窗并处理结果
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
        // 保存后关闭：先累加本次时长→保存→接受关闭
        m_totalSeconds += m_currentTimerSeconds; // 本次时长累加到今日累计
        saveClockRecord(); // 保存今日累计到文件
        event->accept(); // 允许关闭
    } else if (ret == QMessageBox::Discard) {
        // 直接关闭：放弃未保存时长，允许关闭
        event->accept();
    } else {
        // 取消关闭：拒绝关闭事件
        event->ignore();
    }
}

Widget::~Widget()
{
    delete ui;
}

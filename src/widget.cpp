#include "widget.h"
#include "ui_widget.h"
#include "dialog/timedialog.h"
#include "dialog/targetdialog.h"
#include "dialog/tmessagebox.h"

#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QScrollBar>
#include <QMenu>
#include <QSystemTrayIcon>

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);
	this->resize(390, 480);
	
	qDebug() << "当前窗口大小：" << this->width() << " "<< this->height();
	this->setWindowTitle(QString("工作时钟-v%1").arg(APP_VERSION));
	this->setWindowIcon(QIcon(ICON_CLOCK1));
    
	ui->tabWidget->setCurrentIndex(TabPage_e::ClockPage);

	appConfig_ = new AppConfig(this);
	config_ = appConfig_->readConfig();

	recordModel_ = new RecordModel(this);
	recordChart_ = new RecordChart(this);
    connect(recordChart_, &RecordChart::sgn_messageBox, this, [=](QString info){
        QMessageBox::critical(this, "警告", info);
    });
    windowsManager_ = new WindowsManager;
    connect(windowsManager_, &WindowsManager::sgn_hotKeyTrigger, this, &Widget::do_showHideUi);

	timer_ = new QTimer(this);
	timer_->setInterval(1000);
	connect(timer_, &QTimer::timeout, this, &Widget::do_timerTimeout);

	dateStr_ = QDate::currentDate().toString("yyyy-MM-dd");

	sqliteInit();
	uiTimeShowInit();
	uiRecordInit();
	uiChartInit();
	uiToolInit();
    trayInit();
 	hotkeyInit();

	ui->btn_startStop->setProperty("action", "stop");
	ui->label_clock->setProperty("action", "stop");
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
    // 初始化数据库表格和字段 (编号/日期/秒数/3个自定义目标)
    QString sql = R"(
        create table if not exists record (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            seconds INTEGER DEFAULT 0,
            target0 INTEGER,
            target1 INTEGER,
            target2 INTEGER
        )
    )";
    if(!query_->exec(sql)){
        QString errorInfo = query_->lastError().text();
        QMessageBox::critical(this, "警告", "数据库初始化失败：\n"+ errorInfo);
        return;
    }
    qDebug()<<"数据库初始化成功";

    fillMissingDays();
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
    // 聚合函数查询，此后维护totalSeconds_
    QString sql = "select sum(seconds) from record where date = :curDate";

    if(!query_->prepare(sql)){
        ui->label_today->setText("今日记录获取失败");
        return;
    }
    query_->bindValue(":curDate", dateStr_);
    if(!query_->exec()){
        ui->label_today->setText("今日记录获取失败");
        return;
    }
    if(query_->next()){
        totalSeconds_  = query_->value(0).toInt();
    }
    qDebug()<<"读取今日总时长："<<totalSeconds_;

    ui->label_today->setText("今日时长：" + RecordModel::formatSeconds(totalSeconds_));
}

// 表格显示每日数据，不显示今日
void Widget::uiRecordInit()
{
    QString sql = "select date, sum(seconds), sum(target0), sum(target1), sum(target2) "
                                    "from record group by date order by date asc";

    // qDebug()<<config_.targetNameList;
    recordModel_->setTargetNames(config_.targetNameList);
    recordModel_->setQuery(sql);

    if (recordModel_->lastError().isValid()) {
        QMessageBox::critical(this, "警告", "记录表显示失败：\n" + recordModel_->lastError().text());
        return;
    }

    ui->tableView->setModel(recordModel_);

    for (int i = 2; i < recordModel_->columnCount(); i++) {
        ui->tableView->setItemDelegateForColumn(i, new IconDelegate(ui->tableView));
    }

    ui->tableView->resizeColumnsToContents();
    ui->tableView->scrollToBottom();
    // 前两个字段固定显示
    for (int i = 2; i < recordModel_->columnCount(); i++) {
        ui->tableView->setColumnHidden(i, false);
    }
    // 隐藏未含有的目标项
    for (int i = 2 + config_.targetNameList.count(); i < recordModel_->columnCount(); ++i) {
        ui->tableView->setColumnHidden(i, true);
    }
}

void Widget::uiChartInit()
{
    QChart *dayChart = new QChart();
    QChart *weekChart = new QChart();
    QChart *monthChart = new QChart();
    QList<QChart*> chartList = {dayChart, weekChart, monthChart};

    recordChart_->chartLoad(query_, chartList);

    ui->chartView_day->setChart(chartList.at(0));
    ui->chartView_week->setChart(chartList.at(1));
    ui->chartView_month->setChart(chartList.at(2));

    QList<QScrollBar*> barList = {
        ui->scrollArea_day->verticalScrollBar(),
        ui->scrollArea_week->verticalScrollBar(),
        ui->scrollArea_month->verticalScrollBar(),
    };
    for(int i = 0; i < chartList.size(); i++) {
        // 等 chart 开始加载 10ms 后, 执行滚动到底部 + 添加时长标签
        connect(chartList.at(i), &QChart::plotAreaChanged, this, [=](const QRectF &area) {
            if (area.isEmpty()) return;
            QTimer::singleShot(10, [=](){
                QScrollBar *scrollBar = barList.at(i);
                int max = scrollBar->maximum();
                scrollBar->setValue(max);
                // recordChart_->addBarLabels(*chart);
            });
        }, Qt::SingleShotConnection);
    }

    ui->label_latest->setText("上次暂停/保存时间：" + config_.lastSaveTime.toString());
}

void Widget::uiToolInit()
{
    QFile file(QDir::currentPath() + "/temp.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"文件打开失败";
        return;
    }

    ui->textEdit->setText(QString::fromUtf8(file.readAll()));
}

void Widget::trayInit()
{
    // 托盘右键菜单
    QMenu *trayMenu = new QMenu(this);

    startStop_ = new QAction(this);
    startStop_->setText("开始");
    connect(startStop_, &QAction::triggered, this, &Widget::on_btn_startStop_clicked);

    trayMenu->addAction(startStop_);
    // trayMenu->addAction()
    // trayMenu->addAction("开始/暂停", this, [=]() {
    //     on_btn_startStop_clicked();
    // });
    trayMenu->addAction("保存", this, [=]() {
        on_btn_save_clicked();
    });

    // 先保存配置再退出
    trayMenu->addAction("退出", this, [=]() {
        aboutToExit();
    });
    // trayMenu->action()
    // 托盘图标
    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setIcon(QIcon(":/picture/clock1_white.ico"));
    trayIcon_->setToolTip("WorkClock");
    trayIcon_->setContextMenu(trayMenu);
    connect(trayIcon_, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            this->showNormal();
            this->activateWindow();
        }
    });
    trayIcon_->show();
}

void Widget::hotkeyInit()
{
    if(config_.showHideHotkey.isEmpty()) return;
    
    windowsManager_->registerGlobalHotkey(config_.showHideHotkey, this->winId());
}

void Widget::on_btn_startStop_clicked()
{
    isTiming_ = !isTiming_;
    updateTimerState();

    if(!isTiming_)
        updateLastSave();
}

void Widget::on_btn_save_clicked()
{
    if(curTimerSeconds_ == 0) return;
    saveTimerRecord(curTimerSeconds_);

    curTimerSeconds_ = 0;
    ui->label_clock->setText("00:00:00");

    isTiming_ = false;
    updateTimerState();

    config_.lastSaveTime = QTime::currentTime();
    ui->label_latest->setText("上次暂停/保存时间："+config_.lastSaveTime.toString());
}

void Widget::on_btn_change_clicked()
{
    TimeDialog dialog(this, config_.isDirectExit, config_.showHideHotkey);

    int res = dialog.exec();

    if (res == QDialog::Accepted) {
        int changeSeconds = dialog.getChangeMinutes() * 60;
        saveTimerRecord(changeSeconds);

        config_.isDirectExit = dialog.getIsDirectExit();
        QKeySequence curKey = dialog.getShowHideHotkey();
        if(curKey != config_.showHideHotkey) {
            qInfo() << "修改了快捷键: " << config_.showHideHotkey.toString() \
            << "-> " << curKey.toString();

            config_.showHideHotkey = curKey;

            windowsManager_->unregisterGlobalHotkey(this->winId());

            if(!config_.showHideHotkey.isEmpty())
                if(!windowsManager_->registerGlobalHotkey(curKey, this->winId()))
                    QMessageBox::information(this, "提示", curKey.toString() + 
                    " 注册失败,\n请检查是否被其他程序占用");
        }
    }
}

void Widget::on_btn_reset_clicked()
{
    auto res = QMessageBox::question(this, "question",
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

void Widget::on_btn_target_clicked()
{
    TargetDialog dialog(config_.targetNameList, query_, this);

    dialog.exec();

    config_.targetNameList = dialog.getTargetList();
}

void Widget::do_timerTimeout()
{
    curTimerSeconds_++;
    QTime currentTime = QTime(0,0,0).addSecs(curTimerSeconds_);
    ui->label_clock->setText(currentTime.toString("HH:mm:ss"));
}

void Widget::do_showHideUi()
{
    if(this->isHidden()) {
        this->showNormal();
        this->activateWindow();
    } else {
        this->hide();
    }
}

void Widget::updateTimerState()
{
    if(isTiming_){
        timer_->start();
        ui->btn_startStop->setProperty("action", "start");
        ui->label_clock->setProperty("action", "start");
        if(trayIcon_)
            trayIcon_->setIcon(QIcon(":/picture/clock1.ico"));
        if(startStop_)
            startStop_->setText("暂停");
    }
    else{
        timer_->stop();
        ui->btn_startStop->setProperty("action", "stop");
        ui->label_clock->setProperty("action", "stop");
        if(trayIcon_)
            trayIcon_->setIcon(QIcon(":/picture/clock1_white.ico"));
        if(startStop_)
            startStop_->setText("开始");
    }
    ui->btn_startStop->style()->unpolish(ui->btn_startStop);
    ui->btn_startStop->style()->polish(ui->btn_startStop);

    ui->label_clock->style()->unpolish(ui->label_clock);
    ui->label_clock->style()->polish(ui->label_clock);

    // 正在计时，或者有计时，保存和重置按钮可点击，否则不可点击
    bool isEnabled = isTiming_ || (curTimerSeconds_ > 0);
    ui->btn_save->setEnabled(isEnabled);
    ui->btn_reset->setEnabled(isEnabled);
}

void Widget::saveTimerRecord(int seconds)
{
    if(seconds == 0) return;
    if(seconds + totalSeconds_ < 0){
        QMessageBox::critical(this, "提示", "数值过大，修改失败");
        return;
    }

    QString sql = "insert into record(date, seconds) values(:date, :seconds)";
    if(!query_->prepare(sql)){
        QMessageBox::critical(this, "警告", "记录保存失败：\n" + query_->lastError().text());
        return;
    }
    query_->bindValue(":date", dateStr_);
    query_->bindValue(":seconds", seconds);
    if(!query_->exec()){
        QMessageBox::critical(this, "警告", "记录保存失败：\n" + query_->lastError().text());
        return;
    }
    totalSeconds_ += seconds;
    // 今日时长刷新显示
    ui->label_today->setText("今日时长：" + RecordModel::formatSeconds(totalSeconds_));
}

void Widget::fillMissingDays()
{
    QDate today = QDate::currentDate();
    QDate lastDate = config_.lastSaveDate;

    qDebug()<<"最近保存日期:" << today;
    qDebug()<<"今日日期:" << lastDate;
    // 今天已经保存过，不需要补
    if (lastDate == today) return;

    int days = lastDate.daysTo(today);
    qDebug() << "相差天数：" << days;

    if(!db_.transaction()){
        QString errorInfo = db_.lastError().text();
        QMessageBox::critical(this, "警告", "补全天数过程中事务开启失败：\n"+ errorInfo);
        return;
    }

    // 从 lastDate 的后一天开始，补充到今天
    for (int i = 1; i <= days; i++) {
        QDate fillDate = lastDate.addDays(i);
        QString dateStr = fillDate.toString("yyyy-MM-dd");

        // 只填 date seconds默认为 0，target 默认为 NULL
        QString sql = "insert into record (date) values (:date)";
        if(!query_->prepare(sql)){
            QString errorInfo = db_.lastError().text();
            QMessageBox::critical(this, "警告", "补全天数过程中插入失败：\n"+ errorInfo);
            return;
        }
        query_->addBindValue(dateStr);
        if(!query_->exec()){
            QString errorInfo = db_.lastError().text();
            QMessageBox::critical(this, "警告", "补全天数过程中插入失败：\n"+ errorInfo);
            return;
        }
    }
    if(!db_.commit()){
        QString errorInfo = db_.lastError().text();
        QMessageBox::critical(this, "警告", "补全天数过程中事务提交失败：\n"+ errorInfo);
        return;
    }

    config_.lastSaveDate = today;
}

void Widget::updateLastSave()
{
    config_.lastSaveTime = QTime::currentTime();
    ui->label_latest->setText("上次暂停/保存时间："+config_.lastSaveTime.toString());
}

void Widget::saveTempFile()
{
    QString string = ui->textEdit->toPlainText();
    QFile file;
    file.setFileName(QDir::currentPath() + "/temp.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"文件打开失败";
        return;
    }
    file.write(string.toUtf8());
    file.close();
}

// 弹窗确认：有未保存时长时返回按钮角色，无未保存时长直接返回 -1
int Widget::confirmExit()
{
    bool hasUnsavedTime = (curTimerSeconds_ > 0) || isTiming_;
    if (!hasUnsavedTime)
        return -1;

    TMessageBox msg("有未保存的时长，是否保存？", true, this);
    msg.exec();

    QAbstractButton *clicked = msg.clickedButton();
    if (clicked == msg.okButton())
        return QMessageBox::AcceptRole;
    if (clicked == msg.discardButton())
        return QMessageBox::DestructiveRole;
    // 取消按钮
    return QMessageBox::RejectRole; 
}

// 托盘关闭调用
void Widget::aboutToExit()
{
    int role = confirmExit();

    if (role == QMessageBox::AcceptRole)
        saveTimerRecord(curTimerSeconds_);

    if (role == QMessageBox::RejectRole)
        return;

    appConfig_->saveConfig(config_);
    saveTempFile();
    exitByTary_ = true;
    qApp->quit();
}

void Widget::closeEvent(QCloseEvent *event)
{
    // 如果主窗口还存在, 通过托盘退出, 跳过后续判断
    if(exitByTary_) return;

    // 隐藏窗口
    if(!config_.isDirectExit) {
        this->hide();
        event->ignore();

        // 第一次最小化, 弹窗提示
        if(config_.isFirstTray) {
            config_.isFirstTray = false;
            trayIcon_->showMessage("提示", "WorkClock已最小化托盘", QSystemTrayIcon::Information, 1000);
        }
        return;
    }

    // 判断退出
    int role = confirmExit();

    if (role == QMessageBox::AcceptRole)
        saveTimerRecord(curTimerSeconds_);

    if (role == QMessageBox::RejectRole) {
        event->ignore();
        return;
    }

    event->accept();
    appConfig_->saveConfig(config_);
    saveTempFile();
}

bool Widget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    if(windowsManager_->eventProcess(eventType, message)) return true;

    return QWidget::nativeEvent(eventType, message, result);
}

void Widget::on_btn_toUpper_clicked()
{
    QString str = ui->lineEdit_toUpper->text().toUpper();
    str.replace(" ", "_");
    ui->lineEdit_toUpper->setText(str);
}

void Widget::on_btn_toLower_clicked()
{
    QString str = ui->lineEdit_toLower->text().toLower();

    ui->lineEdit_toLower->setText(str);
}

void Widget::on_btn_path_clicked()
{
    QString str = ui->lineEdit_path->text();
    str.replace('\\', '/');
    ui->lineEdit_path->setText(str);
}

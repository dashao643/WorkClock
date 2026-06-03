#ifndef WIDGET_H
#define WIDGET_H

#include "appconfig.h"

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

const QString DATABASE_FILE_NAME = "workClock.db";

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    const QString ICON_CLOCK1 = ":/PicRes/clock1.ico";
    const QString FILE_NAME = "EverydayRecord.txt";

    QSqlQuery *query_;
    AppConfig *appConfig_;

    QSqlDatabase db_;
    QTimer *timer_ = nullptr;
    QString dateStr = "";
    int curTimerSeconds_ = 0;
    bool isTiming_ = false;
    double targetHour_ = 0;

    Widget(QWidget *parent = nullptr);
    ~Widget();

    void sqliteInit();
    void textFileRead();
    void uiPage1Init();
    void uiPage2Init();
    void uiPage3Init();
    void uiPage4Init();
private slots:
    void on_btn_startStop_clicked();
    void on_btn_save_clicked();
    void on_btn_change_clicked();
    void on_btn_reset_clicked();
    void on_btn_setting_clicked();

    void do_timerTimeout();
private:
    void updateTimerState();
    void saveTimerRecord(int seconds);
    QString getTodayTime();
    QString ToHourMinute(int seconds);
    void saveTimeTarget();                  // 今日目标是否完成，写入数据库

    Ui::Widget *ui;
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // WIDGET_H

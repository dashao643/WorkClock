#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QFile>
#include <QMovie>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    const QString ICON_CLOCK1 = ":/PicRes/clock1.ico";
    const QString ICON_CLOCK2 = ":/PicRes/clock2.ico";
    const QString FILE_NAME = "EverydayRecord.txt";
    const QString REACH_TARGET_PIX = ":/PicRes/liang.jpg";
    const QString NOT_REACH_PIX = ":/PicRes/boqi03.gif";

    QDate m_date;
    QTimer *m_timer;
    QFile EverydayRecord;
    QMovie *boqi;
    QPixmap liang;

    int m_totalSeconds = 0;
    int m_currentTimerSeconds = 0;
    bool m_isTiming = false;
    bool m_isSaveLock = true;
    bool m_isResetLock = true;
    double m_targetHour = 4;

    void fileInit();
    void clockInit();
    void StartBtnInit();
    void saveBtnInit();
    void resetBtnInit();
    void settingBtnInit();
    void timeBtnInit();
    void timerInit();
    void dateInit();
    void showTextRecord();
    void readTodayRecord(QTextStream &in);
private slots:
    void saveTarget();
    void saveClockRecord();
    void timerTimeout();
protected:
    // void paintEvent(QPaintEvent *event)override;
    //重写关闭事件函数
    void closeEvent(QCloseEvent *event) override;
};
#endif // WIDGET_H

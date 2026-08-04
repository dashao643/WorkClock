#ifndef WIDGET_H
#define WIDGET_H

#include "appconfig.h"
#include "recordmodel.h"
#include "recordchart.h"
#include "windows_manager.h"

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QSystemTrayIcon>

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
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    const QString ICON_CLOCK1 = ":/picture/clock1.ico";
    const QString FILE_NAME = "EverydayRecord.txt";

    AppConfig *appConfig_ = nullptr;
    RecordModel *recordModel_ = nullptr;
    RecordChart *recordChart_ = nullptr;
    WindowsManager *windowsManager_ = nullptr;

    QSqlQuery *query_;
    QSqlDatabase db_;
    QTimer *timer_ = nullptr;
    QSystemTrayIcon *trayIcon_ = nullptr;
    QAction *startStop_ = nullptr;
    QString dateStr_ = "";
    int curTimerSeconds_ = 0;
    int totalSeconds_ = 0;
    bool isTiming_ = false;
    bool exitByTary_ = false;
    Config_t config_;
    Ui::Widget *ui;

    void sqliteInit();
    void uiTimeShowInit();
    void uiRecordInit();
    void uiChartInit();
    void uiToolInit();
	void trayInit();
 	void hotkeyInit();

    void updateTimerState();
    void saveTimerRecord(int seconds);
    void fillMissingDays();
    void updateLastSave();
    void saveTempFile();
    int confirmExit();
    void aboutToExit();

private slots:
    void on_btn_startStop_clicked();
    void on_btn_save_clicked();
    void on_btn_change_clicked();
    void on_btn_reset_clicked();
    void on_btn_target_clicked();
    void on_btn_toUpper_clicked();
    void on_btn_toLower_clicked();
    void on_btn_path_clicked();

    void do_timerTimeout();
    void do_showHideUi();
    
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // WIDGET_H



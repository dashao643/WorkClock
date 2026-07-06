#ifndef WIDGET_H
#define WIDGET_H

#include "appconfig.h"
#include "recordmodel.h"
#include "recordchart.h"

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
  const QString ICON_CLOCK1 = ":/picture/clock1.ico";
  const QString FILE_NAME = "EverydayRecord.txt";

  AppConfig *appConfig_;
  RecordModel *recordModel_;
  RecordChart *recordChart_;

  QSqlQuery *query_;
  QSqlDatabase db_;
  QTimer *timer_ = nullptr;
  QString dateStr_ = "";
  int curTimerSeconds_ = 0;
  int totalSeconds_ = 0;
  bool isTiming_ = false;

  Config_t config_;

  Widget(QWidget *parent = nullptr);
  ~Widget();

  void sqliteInit();
  void uiTimeShowInit();
  void uiRecordInit();
  void uiChartInit();
  void uiToolInit();

private slots:
  void on_btn_startStop_clicked();
  void on_btn_save_clicked();
  void on_btn_change_clicked();
  void on_btn_reset_clicked();
  void on_btn_target_clicked();
  void on_btn_toUpper_clicked();
  void on_btn_toLower_clicked();

  void do_timerTimeout();

  // void on_btn_clicked();

private:
  void updateTimerState();
  void saveTimerRecord(int seconds);
  void fillMissingDays();
  void updateLastSave();
  void saveTempFile();

  Ui::Widget *ui;
protected:
  void closeEvent(QCloseEvent *event) override;
};
#endif // WIDGET_H

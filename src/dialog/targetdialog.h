#ifndef TARGETDIALOG_H
#define TARGETDIALOG_H

// #include <dataStructure.h>

#include <QDialog>
#include <QListWidgetItem>
#include <QSqlQuery>
// #include <QSqlDatabase>

namespace Ui {
class TargetDialog;
}

class TargetDialog : public QDialog
{
  Q_OBJECT

public:
  explicit TargetDialog(const QStringList &targetList, QSqlQuery *query, QWidget *parent = nullptr);
  ~TargetDialog();

  QStringList getTargetList();

signals:

private slots:
  void on_btn_addTarget_clicked();
  void on_btn_changeTarget_clicked();
  void on_btn_clockIn_clicked();

private:
  // QSqlDatabase db_;

  QSqlQuery *query_;
  QString todayStr_;
  QColor itemColor_;
  QIcon itemIcon_;
  QIcon itemNIcon_;
  QVector<bool> targetFlagV_;

  void listWidgetInit(const QStringList &targetList);

  Ui::TargetDialog *ui;
};

#endif // TARGETDIALOG_H

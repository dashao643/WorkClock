#ifndef RECORDCHART_H
#define RECORDCHART_H

#include "recordmodel.h"

#include <QObject>
#include <QChart>
#include <QBarSet>
#include <QBarCategoryAxis>

class RecordChart : public QObject
{
  Q_OBJECT
public:
  explicit RecordChart(QObject *parent = nullptr);

  void chartLoad(RecordModel &recordModel, QList<QChart*> &chartList);

private:
  QList<double> dailyHours_;
  QList<QString> dateStrings_;
  int barHeight_ = 30;
  int topBottomReserve_ = 120;

  QBarSet* dayChartLoad(QChart *dayChart, QBarCategoryAxis *axisY);
  QBarSet* weekChartLoad(QChart *weekChart, QBarCategoryAxis *axisY);
  QBarSet* monthChartLoad(QChart *monthChart, QBarCategoryAxis *axisY);
};

#endif // RECORDCHART_H

#ifndef RECORDCHART_H
#define RECORDCHART_H

// #include "recordmodel.h"

#include <QObject>
#include <QSqlQuery>
#include <QChart>
#include <QBarSet>
#include <QBarCategoryAxis>

constexpr int CHART_CNT = 3;

class RecordChart : public QObject
{
  Q_OBJECT
public:
  explicit RecordChart(QObject *parent = nullptr);

  void chartLoad(QSqlQuery *query, QList<QChart*> &chartList);

signals:
  void sgn_messageBox(QString info);
  
private:
  QSqlQuery *query_;
  
  int xMax_ = 5;
  int barHeight_ = 30;
  int reserve_ = 120;

  QBarSet* dayChartLoad(QChart *dayChart, QBarCategoryAxis *axisY);
  QBarSet* weekChartLoad(QChart *weekChart, QBarCategoryAxis *axisY);
  QBarSet* monthChartLoad(QChart *monthChart, QBarCategoryAxis *axisY);
};

#endif // RECORDCHART_H

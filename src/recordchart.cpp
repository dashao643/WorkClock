#include "recordchart.h"

#include <QHorizontalBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <QSqlRecord>
#include <QSqlField>
#include <QFont>
#include <QDate>
#include <QtMath>

RecordChart::RecordChart(QObject *parent)
  : QObject{parent}
{
}

void RecordChart::chartLoad(RecordModel &recordModel, QChart &chart)
{
  int rowCount = recordModel.rowCount();
  if (rowCount <= 1) return;

  QList<double> dailyHours;
  QList<QString> dateStrings;

  double maxHours = 0;

  // 构建图表顺序为从下向上, 所以先存最近的日期和时长(排除今天)
  for (int row = rowCount - 2; row >= 0; row--) {
    QSqlRecord record = recordModel.record(row);
    QString dateStr = record.field(0).value().toString();
    int seconds = record.field(1).value().toInt();
    double hours = seconds / 3600.0;

    dateStrings.append(dateStr);
    dailyHours.append(hours);

    if (hours > maxHours)
      maxHours = hours;
  }

  // X 轴范围 0 到 maxHours向上取整再 + 1
  int xMax = qCeil(maxHours) + 1;

  // QBarSet: 第 i 个值 → QBarCategoryAxis 的第 i 个 category
  // 一个 QBarSet 只能有一种颜色
  QBarSet *dailySet = new QBarSet("每日时长");

  for (int i = 0; i < dailyHours.size(); i++) {
    *dailySet << dailyHours[i];
  }

  QHorizontalBarSeries *series = new QHorizontalBarSeries();
  series->append(dailySet);
  chart.addSeries(series);

  // X 轴: x小时
  QValueAxis *axisX = new QValueAxis();
  axisX->setRange(0, xMax);
  axisX->setTickInterval(1);
  axisX->setLabelFormat("%d");
  axisX->setTitleText("时长（小时）");
  chart.addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  // Y 轴: 每日日期
  QBarCategoryAxis *axisY = new QBarCategoryAxis();
  axisY->append(dateStrings);
  chart.addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  series->setLabelsVisible(true);
  series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
  series->setLabelsPrecision(3);

  chart.legend()->setVisible(false);

  int barHeight = 30;
  int topBottomReserve = 120;
  int totalHeight = dateStrings.size() * barHeight + topBottomReserve;
  chart.setMinimumHeight(totalHeight);
}

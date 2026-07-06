#include "recordchart.h"

#include <QHorizontalBarSeries>

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

void RecordChart::chartLoad(RecordModel &recordModel, QList<QChart*> &chartList)
{
  if (chartList.size() != 3) return;
  int rowCount = recordModel.rowCount();
  if (rowCount <= 1) return;

  dailyHours_.clear();
  dateStrings_.clear();

  double maxHours = 0;

  // 构建图表顺序为从下向上, 所以先存最近的日期和时长(排除今天)
  for (int row = rowCount - 2; row >= 0; row--) {
    QSqlRecord record = recordModel.record(row);
    QString dateStr = record.field(0).value().toString();
    int seconds = record.field(1).value().toInt();
    double hours = seconds / 3600.0;

    dateStrings_.append(dateStr);
    dailyHours_.append(hours);

    if (hours > maxHours)
      maxHours = hours;
  }

  // X 轴范围 0 到 maxHours向上取整再 + 1
  int xMax = qCeil(maxHours) + 1;

  for(int i = 0; i < 3; i++) {
    QBarSet* resSet = nullptr;
    QBarCategoryAxis *axisY = new QBarCategoryAxis();

    if(i == 0)
      resSet = dayChartLoad(chartList[0], axisY);
    else if(i == 1)
      resSet = weekChartLoad(chartList[1], axisY);
    else if(i == 2)
      resSet = monthChartLoad(chartList[2], axisY);

    // X 轴与 series 为公共部分
    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(resSet);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, xMax);
    axisX->setTickInterval(1);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("时长（小时）");

    chartList[i]->addAxis(axisX, Qt::AlignBottom);
    chartList[i]->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);
    chartList[i]->addSeries(series);

    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsPrecision(3);
    chartList[i]->legend()->setVisible(false);
  }
}

QBarSet* RecordChart::dayChartLoad(QChart *dayChart, QBarCategoryAxis *axisY)
{
  // QBarSet: 第 i 个值 → QBarCategoryAxis 的第 i 个 category
  /// 一个 QBarSet 只能有一种颜色
  QBarSet *dailySet = new QBarSet("每日时长");

  for (int i = 0; i < dailyHours_.size(); i++) {
    *dailySet << dailyHours_[i];
  }

  // Y 轴: 每日日期
  axisY->append(dateStrings_);
  dayChart->addAxis(axisY, Qt::AlignLeft);

  int totalHeight = dateStrings_.size() * barHeight_ + topBottomReserve_;
  dayChart->setMinimumHeight(totalHeight);

  return dailySet;
}

QBarSet* RecordChart::weekChartLoad(QChart *weekChart, QBarCategoryAxis *axisY)
{
  // 1. 按周分组汇总数据
  QMap<int, double> weekSumMap;  // weekNum → 该周总工时
  QList<int> weekOrder;          // 记录周的先后顺序

  for (int i = 0; i < dailyHours_.size(); i++) {
      QDate date = QDate::fromString(dateStrings_[i], "yyyy-MM-dd");
      int weekNum = date.weekNumber();

      if (!weekSumMap.contains(weekNum)) {
          weekOrder.append(weekNum);
      }
      weekSumMap[weekNum] += dailyHours_[i];
  }

  // 2. 创建 QBarSet，每个周一个值
  QBarSet *weekSet = new QBarSet("周均时长");
  QStringList weekLabels;

  for (int weekNum : weekOrder) {
      *weekSet << weekSumMap[weekNum];
      weekLabels.append(QString("第%1周").arg(weekNum));
  }

  // 3. Y 轴用周标签
  axisY->append(weekLabels);
  weekChart->addAxis(axisY, Qt::AlignLeft);

  // 4. 高度自适应
  int totalHeight = weekLabels.size() * barHeight_ + topBottomReserve_;
  weekChart->setMinimumHeight(totalHeight);

  return weekSet;

  // QBarSet *weekSet = new QBarSet("周均时长");

  // int totalHeight = dateStrings_.size() / 7 * barHeight_ + topBottomReserve_;
  // weekChart->setMinimumHeight(totalHeight);

  // return weekSet;
}

QBarSet* RecordChart::monthChartLoad(QChart *monthChart, QBarCategoryAxis *axisY)
{
  QBarSet *monthSet = new QBarSet("月均时长");

  int totalHeight = dateStrings_.size() / 30 * barHeight_ + topBottomReserve_;
  monthChart->setMinimumHeight(totalHeight);

  return monthSet;
}

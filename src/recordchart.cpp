#include "recordchart.h"

#include <QHorizontalBarSeries>
#include <QValueAxis>

#include <QSqlError>

#include <QSqlRecord>
#include <QSqlField>
#include <QDate>
#include <QtMath>

RecordChart::RecordChart(QObject *parent)
    : QObject{parent}
{
}

void RecordChart::chartLoad(QSqlQuery *query, QList<QChart*> &chartList)
{
    if (chartList.size() != CHART_CNT) return;
    query_ = query;

    for(int i = 0; i < CHART_CNT; i++) {
        QBarSet* resSet = nullptr;
        QBarCategoryAxis *axisY = new QBarCategoryAxis();

        if(i == 0)
            resSet = dayChartLoad(chartList[0], axisY);
        else if(i == 1)
            resSet = weekChartLoad(chartList[1], axisY);
        else if(i == 2)
            resSet = monthChartLoad(chartList[2], axisY);

        if(resSet == nullptr) return;
        // X 轴与 series 为公共部分
        QHorizontalBarSeries *series = new QHorizontalBarSeries();
        series->append(resSet);

        QValueAxis *axisX = new QValueAxis();
        axisX->setRange(0, xMax_);
        axisX->setTickInterval(1);
        axisX->setLabelFormat("%d");
        axisX->setTitleText("时长（小时）");
        axisX->setTitleVisible(false);

        chartList[i]->addAxis(axisX, Qt::AlignBottom);
        chartList[i]->addAxis(axisY, Qt::AlignLeft);

        // 先 addSeries，再 attachAxis
        chartList[i]->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);

        series->setLabelsVisible(true);
        series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
        series->setLabelsPrecision(3);
        chartList[i]->legend()->setVisible(false);

        resSet = nullptr;
    }
}

// 按日期汇总
QBarSet* RecordChart::dayChartLoad(QChart *dayChart, QBarCategoryAxis *axisY)
{
    QString sql = "SELECT date, SUM(seconds) FROM record GROUP BY date ORDER BY date DESC";
    
    if(!query_->exec(sql)){
        QString errorInfo = query_->lastError().text();
        emit sgn_messageBox("每日图表加载中查询失败:\n"+ errorInfo);
        return nullptr;
    }

    QList<double> dailyHours;
    QList<QString> dateStrings;
    double maxHours = 0;

    while(query_->next()) {
        dateStrings.append(query_->value(0).toString());

        double dayHour = query_->value(1).toInt() / 3600.0;
        dailyHours.append(dayHour);

        if(dayHour > maxHours) 
            maxHours = dayHour;
    }

    // X 轴范围 0 到 maxHours向上取整再 + 1
    if(maxHours > 5) {
        xMax_ = qCeil(maxHours) + 1;
        qMax(xMax_, 24);
    }

    // QBarSet: 第 i 个值 → QBarCategoryAxis 的第 i 个 category
    // 一个 QBarSet 只能有一种颜色
    QBarSet *dailySet = new QBarSet("每日时长");
    // dailySet->set
    dailySet->setColor(QColor("#3498DB"));

    for (int i = 0; i < dailyHours.size(); i++) {
        *dailySet << dailyHours[i];
    }

    // Y 轴: 每日日期
    axisY->append(dateStrings);

    int totalHeight = dateStrings.size() * barHeight_ + reserve_;
    dayChart->setMinimumHeight(totalHeight);


    return dailySet;
}

QBarSet* RecordChart::weekChartLoad(QChart *weekChart, QBarCategoryAxis *axisY)
{
    QString sql = R"(
        SELECT
            strftime('%Y-W%W', date) AS week_idx,
            SUM(seconds) / COUNT(DISTINCT date) AS avg_hours
        FROM record
        WHERE date != date('now')
        GROUP BY week_idx
        ORDER BY week_idx DESC;
    )";
    
    if(!query_->exec(sql)){
        QString errorInfo = query_->lastError().text();
        emit sgn_messageBox("周均图表加载中查询失败:\n"+ errorInfo);
        return nullptr;
    }

    QList<double> avgHours;
    QList<QString> dateStrings;

    // double maxHours = 0;

    while(query_->next()) {
        dateStrings.append(query_->value(0).toString());

        double avg = query_->value(1).toInt() / 3600.0;
        avgHours.append(avg);

        // if(avg > maxHours) 
        //   maxHours = avg;
    }
    // if(maxHours > 5)
    //   xMax_ = qCeil(maxHours) + 1;

    QBarSet *weekSet = new QBarSet("每日时长");
    weekSet->setColor(QColor("#0984E3"));

    for (int i = 0; i < avgHours.size(); i++) {
        *weekSet << avgHours[i];
    }

    // Y 轴: 周索引
    axisY->append(dateStrings);

    int totalHeight = dateStrings.size() * barHeight_ + reserve_;
    weekChart->setMinimumHeight(totalHeight);

    return weekSet;
}

QBarSet* RecordChart::monthChartLoad(QChart *monthChart, QBarCategoryAxis *axisY)
{
    QString sql = R"(
        SELECT
            strftime('%Y-%m', date) AS month_idx,
            SUM(seconds) / 3600.0 / COUNT(DISTINCT date) AS avg_hours
        FROM record
        WHERE date != date('now')
        GROUP BY month_idx
        ORDER BY month_idx DESC;
    )";

    if (!query_->exec(sql)) {
        emit sgn_messageBox("月均图表加载中查询失败:\n" + query_->lastError().text());
        return nullptr;
    }

    QList<double> avgHours;
    QList<QString> monthLabels;
    // double maxHours = 0;

    while (query_->next()) {
        QString monthLabel = query_->value(0).toString();
        double avg = query_->value(1).toDouble();

        monthLabels.append(monthLabel);
        avgHours.append(avg);

        // if (avg > maxHours) 
        //   maxHours = avg;
    }
    // if(maxHours > 5)
    //   xMax_ = qCeil(maxHours) + 1;

    QBarSet *monthSet = new QBarSet("月均时长");
    monthSet->setColor(QColor("#0652DD"));

    for (int i = 0; i < avgHours.size(); i++) {
        *monthSet << avgHours[i];
    }

    axisY->append(monthLabels);

    int totalHeight = monthLabels.size() * barHeight_ + reserve_;
    monthChart->setMinimumHeight(totalHeight);

    return monthSet;
}

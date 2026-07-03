#include "recordchart.h"

#include <QHorizontalBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>

#include <QGraphicsSimpleTextItem>
#include <QGraphicsScene>

#include <QValueAxis>
#include <QSqlRecord>
#include <QSqlField>
#include <QFont>

#include <QtMath>
#include <algorithm>

RecordChart::RecordChart(QObject *parent)
    : QObject{parent}
{
    dailyHours_.clear();
    secondsList_.clear();
}

void RecordChart::chartInit(RecordModel &recordModel, QChart &chart)
{
    int rowCount = recordModel.rowCount();
    if (rowCount == 0)
        return;

    QStringList dateStrings;
    double maxHours = 0;

    // 从 model 收集排除今天的所有日期和时长
    for (int row = 0; row < rowCount - 1; row++) {
        QSqlRecord rec = recordModel.record(row);
        QString dateStr = rec.field(0).value().toString();
        int seconds = rec.field(1).value().toInt();
        double hours = seconds / 3600.0;

        // 存日期
        dateStrings.append(dateStr);
        // 存小时
        dailyHours_.append(hours);
        // 存秒数
        secondsList_.append(seconds);

        if (hours > maxHours)
            maxHours = hours;
    }

    // Y 轴反转：dateStrings 目前是 ASC(最早→最晚)，反转后最近日期在底部
    std::reverse(dateStrings.begin(), dateStrings.end());
    std::reverse(dailyHours_.begin(), dailyHours_.end());
    std::reverse(secondsList_.begin(), secondsList_.end());

    // X 轴范围 0 到 maxHours向上取整再 + 1,最小为8小时
    int xMax = qCeil(maxHours) + 1;
    if (xMax < 8)
        xMax = 8;

    // QBarSet: 第 i 个值 → QBarCategoryAxis 的第 i 个 category
    QBarSet *dailySet = new QBarSet("每日时长");

    for (int i = 0; i < dailyHours_.size(); i++) {
        *dailySet << dailyHours_[i];
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

    chart.legend()->setVisible(true);
    chart.legend()->setAlignment(Qt::AlignBottom);

    // 设置图表最小高度，配合 QScrollArea 实现滚动
    int barHeight = 30;
    int topBottomReserve = 120;
    int totalHeight = dateStrings.size() * barHeight + topBottomReserve;
    chart.setMinimumHeight(totalHeight);
}

// 1.时间格式改成 X:XX
// 2.图表间距调为最小
// 3.滚动条还是没滚到最下
void RecordChart::addBarLabels(QChart &chart)
{
    if (dailyHours_.isEmpty())
        return;

    // 获取唯一的 QHorizontalBarSeries（chartInit 只创建了一个）
    auto seriesList = chart.series();
    if (seriesList.isEmpty())
        return;
    QHorizontalBarSeries *series = qobject_cast<QHorizontalBarSeries *>(seriesList.first());
    if (!series)
        return;

    QFont font;
    font.setPointSize(9);

    for (int i = 0; i < dailyHours_.size(); i++) {
        QString text = RecordModel::formatSeconds(secondsList_[i]);
        // 使用 mapToPosition 把数据坐标转为 scene 坐标
        QPointF barEnd = chart.mapToPosition(QPointF(dailyHours_[i], (qreal)i), series);

        QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(text);
        label->setFont(font);
        label->setBrush(QColor(80, 80, 80));

        QRectF rect = label->boundingRect();
        label->setPos(barEnd.x() + 4, barEnd.y() - rect.height() / 2);
        chart.scene()->addItem(label);
    }
}

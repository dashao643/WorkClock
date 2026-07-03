#ifndef RECORDCHART_H
#define RECORDCHART_H

#include "recordmodel.h"

#include <QObject>
#include <QChart>

class RecordChart : public QObject
{
    Q_OBJECT
public:
    explicit RecordChart(QObject *parent = nullptr);

    void chartInit(RecordModel &recordModel, QChart &chart);
    void addBarLabels(QChart &chart);

private:
    QList<double> dailyHours_;      // 存储xx.xx小时
    QList<int> secondsList_;        // 存储总秒数
};

#endif // RECORDCHART_H

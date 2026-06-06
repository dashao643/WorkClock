#include "recordmodel.h"

#include <QColor>
#include <QTime>

RecordModel::RecordModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QVariant RecordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int col = index.column();
    QVariant raw = QSqlQueryModel::data(index, Qt::DisplayRole);
    bool isNull = raw.isNull();

    // Column 0: date — display as-is
    // Column 1: seconds — format as XX时XX分
    // Column 2+: target and custom fields — 1=已完成(green), 0=未完成(red), NULL=empty

    if (role == Qt::DisplayRole) {
        if (col == 1 && !isNull) {
            int seconds = raw.toInt();
            return formatSeconds(seconds);
        }
        if (col >= 2) {
            if (isNull)
                return QString();
            int val = raw.toInt();
            return val == 1 ? QString("完成") : QString("未完成");
        }
        return raw;
    }

    if (role == Qt::BackgroundRole && col >= 2 && !isNull) {
        int val = raw.toInt();
        if (val == 1)
            return QColor(45, 188, 175);   // green
        return QColor(255, 140, 0);        // red
    }

    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QSqlQueryModel::data(index, role);
}

QVariant RecordModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return QString("日期");
        if (section == 1)
            return QString("时长");
        if (section == 2)
            return QString("目标时长");
        // For custom fields, use the database field name
    }
    return QSqlQueryModel::headerData(section, orientation, role);
}

QString RecordModel::formatSeconds(int totalSeconds) const
{
    QTime time = QTime(0, 0, 0).addSecs(totalSeconds);
    return QString("%1时%2分").arg(time.hour()).arg(time.minute());
}

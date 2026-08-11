#include "recordmodel.h"

#include <QTime>
#include <QColor>
#include <QPainter>
#include <QApplication>
#include <QStyle>

RecordModel::RecordModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    clockInIcon_ = QIcon(":/picture/target.png");
}

// QModelIndex: 定位表格中的某格
QVariant RecordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int col = index.column();

    QVariant raw = QSqlQueryModel::data(index, Qt::DisplayRole);
    bool isNull = raw.isNull();

    if (role == Qt::DisplayRole) {
        // 第1列(seconds): 数据库存的是秒数，显示成 "X小时Y分钟"
        if (col == 1 && !isNull) {
            int seconds = raw.toInt();
            return formatSeconds(seconds);
        }
        if (col >= 2 && !isNull) {
            return QString();
        }
        return raw;
    }

    if (role == Qt::DecorationRole && col >= 2 && !isNull) {
        if (raw.toInt() >= 1)
            return clockInIcon_;
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
        for (int i = 0; i < targetNames_.count(); i++) {
            if (section == (i + 2))
                return targetNames_.at(i);
        }
    }
    return QSqlQueryModel::headerData(section, orientation, role);
}

void RecordModel::setTargetNames(const QStringList &nameList)
{
    targetNames_ = nameList;
}

QString RecordModel::formatSeconds(int totalSeconds)
{
    QTime time = QTime(0, 0, 0).addSecs(totalSeconds);
    return QString("%1小时%2分钟").arg(time.hour()).arg(time.minute());
}


IconDelegate::IconDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void IconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{
    QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    if (icon.isNull()) return;
    
    QSize iconSize = option.decorationSize;
    if (iconSize.isEmpty()) 
        iconSize = QSize(24, 24);
    
    QRect center = option.rect;
    int x = center.x() + (center.width() - iconSize.width()) / 2;
    int y = center.y() + (center.height() - iconSize.height()) / 2;
    
    icon.paint(painter, x, y, iconSize.width(), iconSize.height());
}

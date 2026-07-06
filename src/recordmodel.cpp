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

void RecordModel::setTargetNames(const QStringList &names)
{
  targetNames_ = names;
}

QVariant RecordModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

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

QString RecordModel::formatSeconds(int totalSeconds)
{
  QTime time = QTime(0, 0, 0).addSecs(totalSeconds);
  return QString("%1小时%2分钟").arg(time.hour()).arg(time.minute());
}

void IconDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index) const
{
  // 第1步: 画选中/悬停的背景色
  if (option.state & QStyle::State_Selected) {
    // 单元格被选中 → 蓝色高亮背景
    painter->fillRect(option.rect, option.palette.highlight());
  } else if (option.state & QStyle::State_MouseOver) {
    // 鼠标悬停 → 浅色背景
    painter->fillRect(option.rect, option.palette.window().color().lighter(110));
  }

  // 第2步: 从模型取图标
  QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
  if (!icon.isNull()) {
    // 图标尺寸: 优先用视图设定的 iconSize，否则默认 16x16
    QSize iconSize = option.decorationSize;
    if (iconSize.isEmpty())
      iconSize = QSize(16, 16);

    // 计算让图标在单元格正中间的位置
    int x = option.rect.center().x() - iconSize.width() / 2;
    int y = option.rect.center().y() - iconSize.height() / 2;

    // 根据选中状态选择图标的绘制模式
    QIcon::Mode mode = (option.state & QStyle::State_Selected)
               ? QIcon::Selected
               : QIcon::Normal;

    icon.paint(painter, x, y, iconSize.width(), iconSize.height(),
         Qt::AlignCenter, mode);
  }

  // 第3步: 键盘焦点指示（虚线框）
  if (option.state & QStyle::State_HasFocus) {
    QApplication::style()->drawPrimitive(
      QStyle::PE_FrameFocusRect, &option, painter);
  }
}

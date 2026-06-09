#ifndef RECORDMODEL_H
#define RECORDMODEL_H

#include <QSqlQueryModel>
#include <QIcon>
#include <QStyledItemDelegate>

class RecordModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit RecordModel(QObject *parent = nullptr);

    void setTargetNames(const QStringList &names);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
private:
    QStringList targetNames_;
    QIcon clockInIcon_;

    QString formatSeconds(int totalSeconds) const;
};

class IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
};

#endif // RECORDMODEL_H

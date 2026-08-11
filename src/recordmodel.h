#ifndef RECORDMODEL_H
#define RECORDMODEL_H

#include <QSqlQueryModel>
#include <QStyledItemDelegate>
#include <QIcon>

class RecordModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit RecordModel(QObject *parent = nullptr);

private:
    QStringList targetNames_;
    QIcon clockInIcon_;

protected:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public:
    void setTargetNames(const QStringList &nameList);
    static QString formatSeconds(int totalSeconds);
};

class IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    IconDelegate(QObject *parent = nullptr);
    // using QStyledItemDelegate::QStyledItemDelegate;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const override;
    // void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

#endif // RECORDMODEL_H

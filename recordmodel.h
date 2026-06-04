#ifndef RECORDMODEL_H
#define RECORDMODEL_H

#include <QSqlQueryModel>

class RecordModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit RecordModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QString formatSeconds(int totalSeconds) const;
};

#endif // RECORDMODEL_H

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

  void chartLoad(RecordModel &recordModel, QChart &chart);

private:
};

#endif // RECORDCHART_H

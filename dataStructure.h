#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QString>
#include <QDate>
// #include <QSet>

typedef enum {
    ClockPage = 0,
    RecordPage,
    ChartPage,
    ToolPage
} TabPage_e;

typedef struct {
    bool hasImported;               // 文本文件是否被导入过
    QDate lastSaveDate;             // 最近一次保存过记录的日期
    QStringList targetNameList;     // 自定义目标的名称
} Config_t;

#endif // DATASTRUCTURE_H

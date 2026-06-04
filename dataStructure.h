#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QString>
#include <QVector>
#include <QSet>

typedef enum {
    ClockPage = 0,
    RecordPage,
    ChartPage,
    ToolPage
} TabPage_e;

typedef struct {
    QString targetName;
    bool isComplete;
} TargetItem_t;

typedef struct {
    bool hasImported;           // 文本文件是否被导入过
    double targetHour;          // 每日目标时长
    QVector<TargetItem_t> targetVec;
} Config_t;

const QSet<QString> keywords = {
    "select", "insert", "update", "delete", "from", "where", "order",
    "group", "by", "having", "join", "table", "index", "trigger",
    "view", "database", "alter", "drop", "create", "default",
    "null", "not", "primary", "key", "unique", "check", "foreign"
};

#endif // DATASTRUCTURE_H

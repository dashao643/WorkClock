#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QString>
#include <QDate>
#include <QKeySequence>

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
    QTime lastSaveTime;             // 最近一次保存过的时间
    bool isDirectExit;              // 是否直接退出
    QKeySequence showHideHotkey;    // 显示/隐藏快捷键
    bool isFirstTray;               // 是否是第一次最小化
} Config_t;

#endif // DATASTRUCTURE_H

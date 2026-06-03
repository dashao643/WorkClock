#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef enum {
    ClockPage = 0,
    RecordPage,
    ChartPage,
    ToolPage
} TabPage_e;

typedef struct {
    bool hasImported;           // 文本文件是否被导入过
    double targetHour;          // 每日目标时长
} Config_t;

#endif // DATASTRUCTURE_H

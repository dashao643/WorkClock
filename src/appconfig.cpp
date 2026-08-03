#include "appconfig.h"

#include <QDate>

AppConfig::AppConfig(QObject *parent)
    : QObject{parent}
    , setting_(CONFIG_FILE_NAME, QSettings::IniFormat)
{
}

Config_t AppConfig::readConfig()
{
    Config_t config;
    // 是否导入过文本文件
    config.hasImported = setting_.value("textFile/imported", false).toBool();
    // 最近一次保存时间，默认为昨天
    QString defaultDate = QDate::currentDate().addDays(-1).toString("yyyy-MM-dd");
    QString dateStr = setting_.value("saveRecord/lastDate", defaultDate).toString();
    config.lastSaveDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    // 自定义保存项的数量
    int targetCnt = setting_.value("target/count", 0).toInt();

    if(targetCnt > 3) targetCnt = 3;
    /// 根据数量读取名称
    for(int i = 0; i < targetCnt; i++){
        QString name = setting_.value(QString("target/name%1").arg(i), QString("目标%1").arg(i+1)).toString();
        config.targetNameList.push_back(name);
    }
    QString timeStr = setting_.value("saveRecord/lastTime", "").toString();
    config.lastSaveTime = QTime::fromString(timeStr);

    config.isDirectExit = setting_.value("settings/isDirectExit", false).toBool();

    QString showHideHotkeyStr = setting_.value("settings/showHideHotkey", "").toString();
    config.showHideHotkey = QKeySequence::fromString(showHideHotkeyStr);

    return config;
}

void AppConfig::saveConfig(const Config_t &config)
{
    setting_.setValue("textFile/imported", config.hasImported);

    QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");
    setting_.setValue("saveRecord/lastDate", dateStr);

    int targetCnt = config.targetNameList.size();
    setting_.setValue("target/count", targetCnt);
    for(int i = 0; i < targetCnt; i++) {
        setting_.setValue(QString("target/name%1").arg(i), config.targetNameList.at(i));
    }

    QString timeStr = config.lastSaveTime.toString();
    setting_.setValue("saveRecord/lastTime", timeStr);

    setting_.setValue("settings/isDirectExit", config.isDirectExit);

    QString showHideHotkeyStr = config.showHideHotkey.toString();
    setting_.setValue("settings/showHideHotkey", showHideHotkeyStr);
}

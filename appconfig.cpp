#include "appconfig.h"

AppConfig::AppConfig(QObject *parent)
    : QObject{parent}
    , setting_(CONFIG_FILE_NAME, QSettings::IniFormat)
{
}

Config_t AppConfig::readConfig()
{
    Config_t config;
    config.hasImported = setting_.value("textFile/imported", false).toBool();
    config.targetHour = setting_.value("target/time", 2.0).toDouble();

    int customCnt = setting_.value("target/customCnt", 0).toInt();
    for(int i = 1; i <= customCnt; i++){
        TargetItem_t item;
        item.targetName = setting_.value(QString("target/custom%1").arg(i),
                                         QString("目标%1").arg(i)).toString();
        // 默认未完成，之后根据数据库再修改
        item.isComplete = false;
        config.targetVec.push_back(item);
    }

    return config;
}

void AppConfig::saveConfig(const Config_t &config)
{
    setting_.setValue("target/time", config.targetHour);
    setting_.setValue("textFile/imported", config.hasImported);
    // 存储新增的自定义项的数量
    setting_.setValue("target/customCnt", config.targetVec.size());
    // 配置文件存储自定义目标项的文本名称，每日是否完成从db中读取
    int i = 1;
    foreach(TargetItem_t item, config.targetVec){
        setting_.setValue(QString("target/custom%1").arg(i++), item.targetName);
    }
}

// Config_t AppConfig::getConfig() const
// {
//     return config_;
// }

// void AppConfig::setTextFile(bool imported)
// {
//     config_.hasImported = imported;
// }

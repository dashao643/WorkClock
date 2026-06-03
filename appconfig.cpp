#include "appconfig.h"

AppConfig::AppConfig(QObject *parent)
    : QObject{parent}
    , setting_(CONFIG_FILE_NAME, QSettings::IniFormat)
{
}

void AppConfig::setTextFile(bool isSaved)
{
    setting_.setValue("textFile/imported", isSaved);
}

bool AppConfig::getTextFile() const
{
    return setting_.value("textFile/imported", false).toBool();
}

void AppConfig::setTimeTarget(double target)
{
    setting_.setValue("target/time", target);
}

double AppConfig::getTimeTarget() const
{
    return setting_.value("target/time", 2.0).toDouble();
}

Config_t AppConfig::getConfig()
{

}

void AppConfig::saveConfig()
{

}

#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "dataStructure.h"

#include <QObject>
#include <QSettings>

const QString CONFIG_FILE_NAME = "setting.ini";

class AppConfig : public QObject
{
    Q_OBJECT
public:
    explicit AppConfig(QObject *parent = nullptr);

    //
    // bool getTextFile() const;

    // void setTimeTarget(double target);
    // double getTimeTarget() const;

    Config_t readConfig();
    void saveConfig(const Config_t &config);

    // Config_t getConfig() const;
    // void setTextFile(bool isSaved);
signals:

private:
    QSettings setting_;
    // Config_t config_;
};

#endif // APPCONFIG_H

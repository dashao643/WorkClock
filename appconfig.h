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

    void setTextFile(bool isSaved);
    bool getTextFile() const;

    void setTimeTarget(double target);
    double getTimeTarget() const;

    Config_t getConfig();
    void saveConfig();
signals:

private:
    QSettings setting_;
    Config_t config_;
};

#endif // APPCONFIG_H

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

  Config_t readConfig();
  void saveConfig(const Config_t &config);

signals:

private:
  QSettings setting_;
};

#endif // APPCONFIG_H

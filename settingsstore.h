#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H
#include <singleton.h>
#include <QSettings>
#include<QMap>
//#include <exception>

using namespace std;

class SettingsStoreException: public exception
{

public:
  virtual const char* what() const throw()
  {
    return "Settings corrupt or Not found.";
  }
};


class SettingsStore : public Singleton<SettingsStore>
{
public:
    static void loadSettings();
    static void saveSettings();
    static void addDeviceSpecificSetting(QString device, QString key, QString value);
    static QVariant getDeviceSpecificSettings(QString device, QString key, const QVariant &defaultValue);
    static QSettings *settings;
};




#endif // SETTINGSSTORE_H

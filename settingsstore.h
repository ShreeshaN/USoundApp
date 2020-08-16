#ifndef SETTINGSSTORE_H
#define SETTINGSSTORE_H
#include <singleton.h>
#include <QSettings>


class SettingsStore : public Singleton<SettingsStore>
{
public:
    static void loadSettings();
    static void saveSettings();
    static QSettings *settings;
};




#endif // SETTINGSSTORE_H

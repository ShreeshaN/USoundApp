#include<settingsstore.h>
#include<defaults.h>
#include<QDebug>
#include<usoundutils.h>

using namespace std;

QSettings *SettingsStore::settings = 0;

void SettingsStore::loadSettings()
{
    try {
        settings = new QSettings("/Users/prathyushsp/Git/chlst/USoundApp/USoundSettings.ini", QSettings::IniFormat);
//        settings->allKeys();

        // Set Directories
        DIRECTORIES::APPDIR = settings->value("DIRECTORIES/APPDIR", DIRECTORIES::APPDIR).toString();

        // Set Logging Configuration
        LOGGING_CONFIGURATION::FILE_NAME = settings->value("LOGGING_CONFIGURATION/FILE_NAME", LOGGING_CONFIGURATION::FILE_NAME).toString();
        LOGGING_CONFIGURATION::FILE_FORMAT = settings->value("LOGGING_CONFIGURATION/FILE_FORMAT", LOGGING_CONFIGURATION::FILE_FORMAT).toString();

        // Set logfilepath
        LOGGING_CONFIGURATION::LOG_FILE_PATH=DIRECTORIES::APPDIR+"/"+LOGGING_CONFIGURATION::FILE_NAME+QString(generateTimeStamp().c_str())+"."+LOGGING_CONFIGURATION::FILE_FORMAT;

        // Set loglevel
        auto logLevel = settings->value("LOGGING_CONFIGURATION/LOG_LEVEL", LOGGING_CONFIGURATION::LOG_LEVEL).toString();
        if (logLevel=="DEBUG"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=0;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtDebugMsg;
        }
        else if(logLevel == "INFO"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=1;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtInfoMsg;
        }
        else if(logLevel == "WARN"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=2;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtWarningMsg;
        }
        else if(logLevel == "CRITICAL"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=3;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtCriticalMsg;
        }
        else if(logLevel == "FATAL"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=4;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtFatalMsg;
        }
        qDebug() << "Successfully loaded settings";
    }  catch (std::exception &e) {
        qDebug() << e.what();
        qCritical() << "Failed to load configuration from INI file. Using defaults!";
    }

}


void SettingsStore::saveSettings()
{
    settings->sync();
}

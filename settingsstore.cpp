#include<settingsstore.h>
#include<defaults.h>
#include<QDebug>
#include<usoundutils.h>


using namespace std;

QSettings *SettingsStore::settings = 0;

void SettingsStore::loadSettings()
{
    try {
        settings = new QSettings("USoundSettings.ini", QSettings::IniFormat);        
        //Set Version
        META::APP_VERSION = settings->value("META/APP_VERSION", META::APP_VERSION).toString();
        META::APP_NAME = settings->value("META/APP_NAME", META::APP_NAME).toString();

        // Set Directories
        DIRECTORIES::APPDIR = settings->value("DIRECTORIES/APPDIR", DIRECTORIES::APPDIR).toString();
        DIRECTORIES::DATADIR = settings->value("DIRECTORIES/DATADIR", DIRECTORIES::DATADIR).toString();
        DIRECTORIES::IMAGESAVEDIR = settings->value("DIRECTORIES/IMAGESAVEDIR", DIRECTORIES::IMAGESAVEDIR).toString();
        DIRECTORIES::VIDEOSAVEDIR = settings->value("DIRECTORIES/VIDEOSAVEDIR", DIRECTORIES::VIDEOSAVEDIR).toString();

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

        // Set Image Configuration        
        auto imageFormat = settings->value("IMAGE_CONFIGURATION/IMAGE_FORMAT", IMAGE_CONFIGURATION::IMAGE_FORMAT).toString();
        if (imageFormat=="tiff"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=0;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::TIFF;
        }
        else if(imageFormat == "jpeg"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=1;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::JPEG;
        }
        else if(imageFormat == "bmp"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=2;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::BMP;
        }
        else if(imageFormat == "jp2"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=3;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::JP2;
        }
        else if(imageFormat == "jpegxr"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=4;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::JPEGXR;
        }
        else if(imageFormat == "png"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=5;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::PNG;
        }
        else if(imageFormat == "hobj"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=4;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::HOBJ;
        }
        else if(imageFormat == "ima"){
            IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX=4;
            IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::IMA;
        }
        IMAGE_CONFIGURATION::IMAGE_GRID_ROWS=settings->value("IMAGE_CONFIGURATION/IMAGE_GRID_ROWS", IMAGE_CONFIGURATION::IMAGE_GRID_ROWS).toInt();
        IMAGE_CONFIGURATION::IMAGE_GRID_COLUMNS=settings->value("IMAGE_CONFIGURATION/IMAGE_GRID_COLUMNS", IMAGE_CONFIGURATION::IMAGE_GRID_COLUMNS).toInt();
        IMAGE_CONFIGURATION::IMAGE_RESOLUTION_WIDTH=settings->value("IMAGE_CONFIGURATION/IMAGE_RESOLUTION_WIDTH", IMAGE_CONFIGURATION::IMAGE_RESOLUTION_WIDTH).toInt();
        IMAGE_CONFIGURATION::IMAGE_RESOLUTION_HEIGHT=settings->value("IMAGE_CONFIGURATION/IMAGE_RESOLUTION_HEIGHT", IMAGE_CONFIGURATION::IMAGE_RESOLUTION_HEIGHT).toInt();

        qDebug() << "Successfully loaded settings";
    }  catch (std::exception &e) {
        qCritical() << e.what();
        qCritical() << "Failed to load configuration from INI file. Using defaults!";
        throw SettingsStoreException();
    }

}

void SettingsStore::addDeviceSpecificSetting(QString device, QString key, QString value){
    auto sKey = device+"/"+key;
    if(settings->childGroups().contains(sKey, Qt::CaseInsensitive)){
        qDebug() << QString("Key not found. Adding key: %1").arg(sKey);
        settings->beginGroup(sKey);
    }
    qDebug() << QString("Update %1 key with value %2").arg(sKey).arg(value);
    settings->setValue(sKey, value);
    SettingsStore::saveSettings();
}

QVariant SettingsStore::getDeviceSpecificSettings(QString device, QString key, const QVariant &defaultValue){
    auto sKey = device+"/"+key;
    return settings->value(sKey, defaultValue);
}

void SettingsStore::saveSettings()
{
    settings->sync();
}

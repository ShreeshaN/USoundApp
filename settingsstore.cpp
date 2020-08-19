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
        // Set loglevel
        auto imageFormat = settings->value("IMAGE_CONFIGURATION/IMAGEFORMAT", IMAGE_CONFIGURATION::IMAGEFORMAT).toString();
        if (imageFormat=="tiff"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=0;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::TIFF;
        }
        else if(imageFormat == "jpeg"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=1;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::JPEG;
        }
        else if(imageFormat == "bmp"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=2;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::BMP;
        }
        else if(imageFormat == "jp2"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=3;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::JP2;
        }
        else if(imageFormat == "jpegxr"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=4;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::JPEGXR;
        }
        else if(imageFormat == "png"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=5;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::PNG;
        }
        else if(imageFormat == "hobj"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=4;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::HOBJ;
        }
        else if(imageFormat == "ima"){
            IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=4;
            IMAGE_CONFIGURATION::IMAGEFORMAT=AvailableImageFormats::IMA;
        }
        qDebug() << "Successfully loaded settings";
    }  catch (std::exception &e) {
        qCritical() << e.what();
        qCritical() << "Failed to load configuration from INI file. Using defaults!";
        throw SettingsStoreException();
    }

}


void SettingsStore::saveSettings()
{
    settings->sync();
}

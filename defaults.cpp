#include "defaults.h"
#include<usoundutils.h>
#include <QDir>

//Set App Version
QString META::APP_VERSION = "";
QString META::APP_NAME = "USound";

//Set App Directory
QString DIRECTORIES::APPDIR = QDir::tempPath() +"/USoundApp";
// Data directories configuration
QString DIRECTORIES::DATADIR = "/data";
QString DIRECTORIES::IMAGESAVEDIR = "/images";
QString DIRECTORIES::VIDEOSAVEDIR = "/videos";

// Logging configuration
QString LOGGING_CONFIGURATION::FILE_NAME = "USoundApp";
QString LOGGING_CONFIGURATION::FILE_FORMAT = "log";
QString LOGGING_CONFIGURATION::LOG_FILE_PATH = DIRECTORIES::APPDIR+"/"+LOGGING_CONFIGURATION::FILE_NAME+QString(generateTimeStamp().c_str())+"."+LOGGING_CONFIGURATION::FILE_FORMAT;;
QtMsgType LOGGING_CONFIGURATION::LOG_LEVEL = QtInfoMsg;
int LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=1;

// Image Configuration
QString IMAGE_CONFIGURATION::IMAGEFORMAT = AvailableImageFormats::TIFF;
int IMAGE_CONFIGURATION::IMAGEFORMAT_INDEX=0;



std::string CameraParameters::getAUTOEXPOSURE() const
{
    return AUTOEXPOSURE;
}

void CameraParameters::setAUTOEXPOSURE(const std::string &value)
{
    AUTOEXPOSURE = value;
}

std::string CameraParameters::getGAMMA() const
{
    return GAMMA;
}

void CameraParameters::setGAMMA(const std::string &value)
{
    GAMMA = value;
}

std::string CameraParameters::getACQUISITIONFRAMERATE() const
{
    return ACQUISITIONFRAMERATE;
}

void CameraParameters::setACQUISITIONFRAMERATE(const std::string &value)
{
    ACQUISITIONFRAMERATE = value;
}

std::string CameraParameters::getACQUISITIONFRAMERATEENABLE() const
{
    return ACQUISITIONFRAMERATEENABLE;
}

void CameraParameters::setACQUISITIONFRAMERATEENABLE(const std::string &value)
{
    ACQUISITIONFRAMERATEENABLE = value;
}

std::string CameraParameters::getRESULTINGFRAMERATE() const
{
    return RESULTINGFRAMERATE;
}

void CameraParameters::setRESULTINGFRAMERATE(const std::string &value)
{
    RESULTINGFRAMERATE = value;
}

std::string CameraParameters::getGAIN() const
{
    return GAIN;
}

void CameraParameters::setGAIN(const std::string &value)
{
    GAIN = value;
}

std::string CameraParameters::getAUTOGAIN() const
{
    return AUTOGAIN;
}

void CameraParameters::setAUTOGAIN(const std::string &value)
{
    AUTOGAIN = value;
}

std::string CameraParameters::getHUE() const
{
    return HUE;
}

void CameraParameters::setHUE(const std::string &value)
{
    HUE = value;
}

std::string CameraParameters::getSATURATION() const
{
    return SATURATION;
}

void CameraParameters::setSATURATION(const std::string &value)
{
    SATURATION = value;
}

std::string CameraParameters::getBRIGHTNESS() const
{
    return BRIGHTNESS;
}

void CameraParameters::setBRIGHTNESS(const std::string &value)
{
    BRIGHTNESS = value;
}

std::string CameraParameters::getCONTRAST() const
{
    return CONTRAST;
}

void CameraParameters::setCONTRAST(const std::string &value)
{
    CONTRAST = value;
}

std::string CameraParameters::getMONOCHROME() const
{
    return MONOCHROME;
}

void CameraParameters::setMONOCHROME(const std::string &value)
{
    MONOCHROME = value;
}

std::string CameraParameters::getRGB() const
{
    return RGB;
}

void CameraParameters::setRGB(const std::string &value)
{
    RGB = value;
}

std::string CameraParameters::getEXPOSURETIME() const
{
    return EXPOSURETIME;
}

void CameraParameters::setEXPOSURETIME(const std::string &value)
{
    EXPOSURETIME = value;
}

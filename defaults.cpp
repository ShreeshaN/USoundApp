#include "defaults.h"
#include<usoundutils.h>
#include <QDir>


CameraParameterNames::CameraParameterNames(){}

std::string CameraParameterNames::getRGB() const
{
    return RGB;
}

void CameraParameterNames::setRGB(const std::string &value)
{
    RGB = value;
}

std::string CameraParameterNames::getMONOCHROME() const
{
    return MONOCHROME;
}

void CameraParameterNames::setMONOCHROME(const std::string &value)
{
    MONOCHROME = value;
}

std::string CameraParameterNames::getCONTRAST() const
{
    return CONTRAST;
}

void CameraParameterNames::setCONTRAST(const std::string &value)
{
    CONTRAST = value;
}

std::string CameraParameterNames::getBRIGHTNESS() const
{
    return BRIGHTNESS;
}

void CameraParameterNames::setBRIGHTNESS(const std::string &value)
{
    BRIGHTNESS = value;
}

std::string CameraParameterNames::getSATURATION() const
{
    return SATURATION;
}

void CameraParameterNames::setSATURATION(const std::string &value)
{
    SATURATION = value;
}

std::string CameraParameterNames::getAUTOGAIN() const
{
    return AUTOGAIN;
}

void CameraParameterNames::setAUTOGAIN(const std::string &value)
{
    AUTOGAIN = value;
}

std::string CameraParameterNames::getGAIN() const
{
    return GAIN;
}

void CameraParameterNames::setGAIN(const std::string &value)
{
    GAIN = value;
}

std::string CameraParameterNames::getRESULTINGFRAMERATE() const
{
    return RESULTINGFRAMERATE;
}

void CameraParameterNames::setRESULTINGFRAMERATE(const std::string &value)
{
    RESULTINGFRAMERATE = value;
}

std::string CameraParameterNames::getACQUISITIONFRAMERATEENABLE() const
{
    return ACQUISITIONFRAMERATEENABLE;
}

void CameraParameterNames::setACQUISITIONFRAMERATEENABLE(const std::string &value)
{
    ACQUISITIONFRAMERATEENABLE = value;
}

std::string CameraParameterNames::getACQUISITIONFRAMERATE() const
{
    return ACQUISITIONFRAMERATE;
}

void CameraParameterNames::setACQUISITIONFRAMERATE(const std::string &value)
{
    ACQUISITIONFRAMERATE = value;
}

std::string CameraParameterNames::getGAMMA() const
{
    return GAMMA;
}

void CameraParameterNames::setGAMMA(const std::string &value)
{
    GAMMA = value;
}

std::string CameraParameterNames::getAUTOEXPOSURE() const
{
    return AUTOEXPOSURE;
}

void CameraParameterNames::setAUTOEXPOSURE(const std::string &value)
{
    AUTOEXPOSURE = value;
}

std::string CameraParameterNames::getEXPOSURETIME() const
{
    return EXPOSURETIME;
}

void CameraParameterNames::setEXPOSURETIME(const std::string &value)
{
    EXPOSURETIME = value;
}

std::string CameraParameterNames::getHUE() const
{
    return HUE;
}

void CameraParameterNames::setHUE(const std::string &value)
{
    HUE = value;
}
BaslerCameraParameterNames::BaslerCameraParameterNames():CameraParameterNames(){}

std::string BaslerCameraParameterNames::getHUE() const
{
    return HUE;
}

void BaslerCameraParameterNames::setHUE(const std::string &value)
{
    HUE = value;
}
AlliedVisionCameraParameterNames::AlliedVisionCameraParameterNames():CameraParameterNames(){}

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



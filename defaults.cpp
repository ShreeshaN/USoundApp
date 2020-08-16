#include "defaults.h"
#include <QDir>


const std::string BaslerCameraParameterNames::EXPOSURETIME = "ExposureTime";
const std::string BaslerCameraParameterNames::AUTOEXPOSURE = "ExposureAuto";
const std::string BaslerCameraParameterNames::GAMMA = "Gamma";
const std::string BaslerCameraParameterNames::ACQUISITIONFRAMERATE = "AcquisitionFrameRate";
const std::string BaslerCameraParameterNames::ACQUISITIONFRAMERATEENABLE = "AcquisitionFrameRateEnable";
const std::string BaslerCameraParameterNames::RESULTINGFRAMERATE = "ResultingFrameRate";
const std::string BaslerCameraParameterNames::GAIN = "Gain";
const std::string BaslerCameraParameterNames::AUTOGAIN = "GainAuto";

// Although, I did not find these controls in acquisition handle's SetFramegrabberParam method
// Initializing just to maintain equality
const std::string BaslerCameraParameterNames::SATURATION="BslSaturation";
const std::string BaslerCameraParameterNames::HUE="BslHue";
const std::string BaslerCameraParameterNames::BRIGHTNESS="brightness";
const std::string BaslerCameraParameterNames::CONTRAST="constrast";
const std::string BaslerCameraParameterNames::MONOCHROME="monochrome";
const std::string BaslerCameraParameterNames::RGB="rgb";

//Set App Directory
QString DIRECTORIES::APPDIR = QDir::tempPath() +"/USoundApp";
// Data directories configuration
QString DIRECTORIES::DATADIR = APPDIR+"/data";
QString DIRECTORIES::IMAGEFORMAT = AvailableImageFormats::TIFF;
QString DIRECTORIES::IMAGESAVEDIR = "/images";
QString DIRECTORIES::VIDEOSAVEDIR = "/videos";

// Logging configuration
QString LOGGING_CONFIGURATION::FILE_NAME = "USoundApp";
QString LOGGING_CONFIGURATION::FILE_FORMAT = "log";
QtMsgType LOGGING_CONFIGURATION::LOG_LEVEL = QtInfoMsg;
int LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=1;



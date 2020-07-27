#include "defaults.h"
#include <QDir>


const std::string HalconCameraParameterNames::EXPOSURETIME = "ExposureTime";
const std::string HalconCameraParameterNames::AUTOEXPOSURE = "ExposureAuto";
const std::string HalconCameraParameterNames::GAMMA = "Gamma";
const std::string HalconCameraParameterNames::ACQUISITIONFRAMERATE = "AcquisitionFrameRate";
const std::string HalconCameraParameterNames::ACQUISITIONFRAMERATEENABLE = "AcquisitionFrameRateEnable";
const std::string HalconCameraParameterNames::RESULTINGFRAMERATE = "ResultingFrameRate";
const std::string HalconCameraParameterNames::GAIN = "Gain";

// Although, I did not find these controls in acquisition handle's SetFramegrabberParam method
// Initializing just to maintain equality
const std::string SATURATION="saturation";
const std::string HUE="hue";
const std::string BRIGHTNESS="brightness";
const std::string CONTRAST="constrast";
const std::string MONOCHROME="monochrome";
const std::string RGB="rgb";

//Set App Directory
const QString Directories::APPDIR = QDir::tempPath() +"/USoundApp";
// Logging configuration
const QString Directories::LOGDIR = APPDIR;
const QString Directories::LOGFILENAME = "USoundApp";
const QString Directories::LOGFILEFORMAT = "log";

// Data directories configuration
const QString Directories::DATADIR = APPDIR+"/data";
const QString Directories::IMAGEFORMAT = AvailableImageFormats::PNG;
const QString Directories::IMAGESAVEDIR = "/images";
const QString Directories::VIDEOSAVEDIR = "/videos";


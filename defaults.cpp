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
const std::string SATURATION="BslSaturation";
const std::string HUE="BslHue";
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
const QString Directories::IMAGEFORMAT = AvailableImageFormats::TIFF;
const QString Directories::IMAGESAVEDIR = "/images";
const QString Directories::VIDEOSAVEDIR = "/videos";


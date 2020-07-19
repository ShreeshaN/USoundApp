#include "defaults.h"


const std::string HalconCameraParameters::EXPOSURETIME = "ExposureTime";
const std::string HalconCameraParameters::AUTOEXPOSURE = "ExposureAuto";
const std::string HalconCameraParameters::GAMMA = "Gamma";
const std::string HalconCameraParameters::ACQUISITIONFRAMERATE = "AcquisitionFrameRate";
const std::string HalconCameraParameters::RESULTINGFRAMERATE = "ResultingFrameRate";
const std::string HalconCameraParameters::GAIN = "Gain";

// Although, I did not find these controls in acquisition handle's SetFramegrabberParam method
// Initializing just to maintain equality
const std::string SATURATION="saturation";
const std::string HUE="hue";
const std::string BRIGHTNESS="brightness";
const std::string CONTRAST="constrast";
const std::string MONOCHROME="monochrome";
const std::string RGB="rgb";

#include "cameracontrols.h"
#include "defaults.h"

#include <QDebug>

double CameraControls::getExposureTime()
{
    return exposureTime;
}

void CameraControls::setExposureTime(double value)
{
    exposureTime = value;
}

double CameraControls::getAnalogGain()
{
    return analogGain;
}

void CameraControls::setAnalogGain(double value)
{
    analogGain = value;
}

float CameraControls::getExposureTarget()
{
    return exposureTarget;
}

void CameraControls::setExposureTarget(float value)
{
    exposureTarget = value;
}

bool CameraControls::getAutoExposure()
{
    return autoExposure;
}

void CameraControls::setAutoExposure(bool value)
{
    autoExposure = value;
}

float CameraControls::getHue()
{
    return hue;
}

void CameraControls::setHue(float value)
{
    hue = value;
}

float CameraControls::getSaturation()
{
    return saturation;
}

void CameraControls::setSaturation(float value)
{
    saturation = value;
}

float CameraControls::getBrightness()
{
    return brightness;
}

void CameraControls::setBrightness(float value)
{
    brightness = value;
}

float CameraControls::getContrast()
{
    return contrast;
}

void CameraControls::setContrast(float value)
{
    contrast = value;
}

double CameraControls::getGamma()
{
    return gamma;
}

void CameraControls::setGamma(double value)
{
    gamma = value;
}

bool CameraControls::getMonochrome()
{
    return monochrome;
}

void CameraControls::setMonochrome(bool value)
{
    monochrome = value;
}

bool CameraControls::getRgb()
{
    return rgb;
}

void CameraControls::setRgb(bool value)
{
    rgb = value;
}

std::string CameraControls::getResolution()
{
    return resolution;
}

void CameraControls::setResolution(std::string value)
{
    resolution = value;
}

long CameraControls::getAcquisitionFrameRate() const
{
    return acquisitionFrameRate;
}

void CameraControls::setAcquisitionFrameRate(long value)
{
    acquisitionFrameRate = value;
}

long CameraControls::getResultingFrameRate() const
{
    return resultingFrameRate;
}

void CameraControls::setResultingFrameRate(long value)
{
    resultingFrameRate = value;
}

bool CameraControls::getAcquisitionFrameRateEnable() const
{
    return acquisitionFrameRateEnable;
}

void CameraControls::setAcquisitionFrameRateEnable(bool value)
{
    acquisitionFrameRateEnable = value;
}

bool CameraControls::getAutoGain() const
{
    return autoGain;
}

void CameraControls::setAutoGain(bool value)
{
    autoGain = value;
}

CameraControls::CameraControls()
{
    CameraControlDefaults cameraControlsDefaults;
    this->resolution = cameraControlsDefaults.RESOLUTION;
    this->exposureTime = cameraControlsDefaults.EXPOSURETIME;
    this->analogGain =cameraControlsDefaults.ANALOGGAIN;
    this->autoGain =cameraControlsDefaults.AUTOGAIN;
    this->exposureTarget =cameraControlsDefaults.EXPOSURETARGET;
    this->autoExposure =cameraControlsDefaults.AUTOEXPOSURE;
    this->hue =cameraControlsDefaults.HUE;
    this->saturation =cameraControlsDefaults.SATURATION;
    this->brightness =cameraControlsDefaults.BRIGHTNESS;
    this->contrast =cameraControlsDefaults.CONTRAST;
    this->gamma =cameraControlsDefaults.GAMMA;
    this->acquisitionFrameRate =cameraControlsDefaults.ACQUISITIONFRAMERATE;
    this->acquisitionFrameRateEnable =cameraControlsDefaults.ACQUISITIONFRAMERATEENABLE;
    this->resultingFrameRate =cameraControlsDefaults.RESULTINGFRAMERATE;
    this->monochrome =cameraControlsDefaults.MONOCHROME;
    this->rgb =cameraControlsDefaults.RGB;
}

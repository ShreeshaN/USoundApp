#include "cameracontrols.h"
#include "defaults.h"

#include <QDebug>

float CameraControls::getExposureTime() const
{
    return exposureTime;
}

void CameraControls::setExposureTime(float value)
{
    exposureTime = value;
}

float CameraControls::getAnalogGain() const
{
    return analogGain;
}

void CameraControls::setAnalogGain(float value)
{
    analogGain = value;
}

float CameraControls::getExposureTarget() const
{
    return exposureTarget;
}

void CameraControls::setExposureTarget(float value)
{
    exposureTarget = value;
}

bool CameraControls::getAutoExposure() const
{
    return autoExposure;
}

void CameraControls::setAutoExposure(bool value)
{
    autoExposure = value;
}

float CameraControls::getHue() const
{
    return hue;
}

void CameraControls::setHue(float value)
{
    hue = value;
}

float CameraControls::getSaturation() const
{
    return saturation;
}

void CameraControls::setSaturation(float value)
{
    saturation = value;
}

float CameraControls::getBrightness() const
{
    return brightness;
}

void CameraControls::setBrightness(float value)
{
    brightness = value;
}

float CameraControls::getContrast() const
{
    return contrast;
}

void CameraControls::setContrast(float value)
{
    contrast = value;
}

float CameraControls::getGamma() const
{
    return gamma;
}

void CameraControls::setGamma(float value)
{
    gamma = value;
}

int CameraControls::getFrameRate() const
{
    return frameRate;
}

void CameraControls::setFrameRate(int value)
{
    frameRate = value;
}

bool CameraControls::getMonochrome() const
{
    return monochrome;
}

void CameraControls::setMonochrome(bool value)
{
    monochrome = value;
}

bool CameraControls::getRgb() const
{
    return rgb;
}

void CameraControls::setRgb(bool value)
{
    rgb = value;
}

CameraControls::CameraControls()
{

}



std::string CameraControls::getResolution() const
{
    return resolution;
}

void CameraControls::setResolution(const std::string &value)
{
    resolution = value;
}

CameraControls::CameraControls(CameraControlDefaults cameraControlsDefaults)
{
    this->resolution = cameraControlsDefaults.RESOLUTION;
    this->exposureTime = cameraControlsDefaults.EXPOSURETIME;
    this->analogGain =cameraControlsDefaults.ANALOGGAIN;
    this->exposureTarget =cameraControlsDefaults.EXPOSURETARGET;
    this->autoExposure =cameraControlsDefaults.AUTOEXPOSURE;
    this->hue =cameraControlsDefaults.HUE;
    this->saturation =cameraControlsDefaults.SATURATION;
    this->brightness =cameraControlsDefaults.BRIGHTNESS;
    this->contrast =cameraControlsDefaults.CONTRAST;
    this->gamma =cameraControlsDefaults.GAMMA;
    this->frameRate =cameraControlsDefaults.FRAMERATE;
    this->monochrome =cameraControlsDefaults.MONOCHROME;
    this->rgb =cameraControlsDefaults.RGB;
}




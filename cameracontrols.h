#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H
#include <string>
#include <defaults.h>
#include <QString>
#include <QThread>

class CameraControls
{
public:

    // functionalities
    void snap();
    void record();

    void rotateClockwise();
    void flipOnX();
    void flipOnY();


    float getExposureTime() const;
    void setExposureTime(float value);

    float getAnalogGain() const;
    void setAnalogGain(float value);

    float getExposureTarget() const;
    void setExposureTarget(float value);

    bool getAutoExposure() const;
    void setAutoExposure(bool value);

    float getHue() const;
    void setHue(float value);

    float getSaturation() const;
    void setSaturation(float value);

    float getBrightness() const;
    void setBrightness(float value);

    float getContrast() const;
    void setContrast(float value);

    float getGamma() const;
    void setGamma(float value);

    int getFrameRate() const;
    void setFrameRate(int value);

    bool getMonochrome() const;
    void setMonochrome(bool value);

    bool getRgb() const;
    void setRgb(bool value);

    CameraControls();
    CameraControls(CameraControlDefaults cameraControlsDefaults);


    void setupCameraControls();

    std::string getResolution() const;
    void setResolution(const std::string &value);

private:
    std::string resolution;

    // exposure controls
    float exposureTime;
    float analogGain;
    float exposureTarget;  // not sure about this implementation
    bool autoExposure;

    // color appearance parameters
    float hue;
    float saturation;
    float brightness;
    float contrast;
    float gamma;
    int frameRate;
    bool monochrome;  // not sure about this implementation
    bool rgb;  // not sure about this implementation
};

#endif // CAMERACONTROLS_H

#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H
#include <string>
#include <defaults.h>
#include <QString>
#include <QThread>
#include <HalconCpp.h>
#include <Halcon.h>
#include <HFramegrabber.h>

class CameraControls
{
public:

    // functionalities
    void snap();
    void record();

    void rotateClockwise();
    void flipOnX();
    void flipOnY();


    long getExposureTime() const;
    void setExposureTime(long value);

    long getAnalogGain() const;
    void setAnalogGain(long value);

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

    long getGamma() const;
    void setGamma(long value);

    double getFrameRate() const;
    void setFrameRate(double value);

    bool getMonochrome() const;
    void setMonochrome(bool value);

    bool getRgb() const;
    void setRgb(bool value);

    CameraControls();
    CameraControls(CameraControlDefaults cameraControlsDefaults);


    void setupCameraControls(HalconCpp::HFramegrabber * acq);

    std::string getResolution() const;
    void setResolution(const std::string &value);

private:
    friend std::ostream& operator<<(std::ostream &strm, const CameraControls &a);

    std::string resolution;

    // exposure controls
    long exposureTime;
    long analogGain;
    float exposureTarget;  // not sure about this implementation
    bool autoExposure;

    // color appearance parameters
    float hue;
    float saturation;
    float brightness;
    float contrast;
    long gamma;
    double frameRate;
    bool monochrome;  // not sure about this implementation
    bool rgb;  // not sure about this implementation
};

static std::ostream& operator<<(std::ostream &strm, const CameraControls &a) {
  return strm << "A(" << a.exposureTime << ")";
}


#endif // CAMERACONTROLS_H

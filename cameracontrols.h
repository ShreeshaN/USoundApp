#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H
#include <string>
#include <defaults.h>
#include <QString>
#include <QThread>

#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "Halcon.h"
#  include "HFramegrabber.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif

//#include <HalconCpp.h>
//#include <Halcon.h>
//#include <HFramegrabber.h>

class CameraControls
{
public:

    // functionalities
    void snap();
    void record();

    void rotateClockwise();
    void flipOnX();
    void flipOnY();


    long getExposureTime();
    void setExposureTime(long value);

    long getAnalogGain();
    void setAnalogGain(long value);

    float getExposureTarget();
    void setExposureTarget(float value);

    bool getAutoExposure();
    void setAutoExposure(bool value);

    float getHue();
    void setHue(float value);

    float getSaturation();
    void setSaturation(float value);

    float getBrightness();
    void setBrightness(float value);

    float getContrast();
    void setContrast(float value);

    long getGamma();
    void setGamma(long value);

    bool getMonochrome();
    void setMonochrome(bool value);

    bool getRgb();
    void setRgb(bool value);
    CameraControls();

    std::string getResolution();
    void setResolution(std::string value);

    long getAcquisitionFrameRate() const;
    void setAcquisitionFrameRate(long value);

    long getResultingFrameRate() const;
    void setResultingFrameRate(long value);

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
    long acquisitionFrameRate;
    long resultingFrameRate;
    bool monochrome;  // not sure about this implementation
    bool rgb;  // not sure about this implementation
};

#endif // CAMERACONTROLS_H

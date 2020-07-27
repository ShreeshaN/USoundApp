#ifndef IMAGEACQUISITION_H
#define IMAGEACQUISITION_H
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
#include <QImage>
#include "cameracontrols.h"
#include "defaults.h"
#include <QSignalMapper>



class ImageAcquisition : public QThread
{
    Q_OBJECT

private:
    HalconCpp::HFramegrabber imageAcquisitionHandle;
    bool stopAcquisition=false;
    int counter=0;
    QString deviceName;
    CameraControls cameraControls;


public:
    // Constructor
    ImageAcquisition(QString deviceName, QObject *parent=0);
    HalconCpp::HImage currentImage;

    // Member Functions
    void setup();
    bool HImage2QImage(HalconCpp::HImage &from, QImage &to);
    void setupCameraControls();
    HalconCpp::HTuple getValueForParam(std::string paramString);
    void setValueForParam(std::string paramString, int paramValue);
    void setValueForParam(std::string paramString, std::string paramValue);
    void setValueForParam(std::string paramString, double paramValue);
    void setValueForParam(std::string paramString, long paramValue);
    void startAquisition();

    // Setters and Getters
    QString getDeviceName() const;
    void setDeviceName(const QString &value);
    int getCounter() const;
    void setCounter(int value);
    HalconCpp::HFramegrabber getImageAcquisitionHandle() const;
    void setImageAcquisitionHandle(const HalconCpp::HFramegrabber &value);
    bool getStopAcquisition() const;
    void setStopAcquisition(bool value);
    CameraControls getCameraControls() const;
    void setCameraControls(const CameraControls &value);

signals:
    void renderImageSignal(QImage);


protected:
    void run();

};

#endif // IMAGEACQUISITION_H

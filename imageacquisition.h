#ifndef IMAGEACQUISITION_H
#define IMAGEACQUISITION_H
#include <QThread>
#include <HalconCpp.h>
#include <Halcon.h>
#include "HFramegrabber.h"
#include <HFramegrabber.h>
#include <QImage>
#include "cameracontrols.h"
#include "defaults.h"



class ImageAcquisition : public QThread
{
    Q_OBJECT

private:
    HalconCpp::HFramegrabber acq;
    bool stop=false;
    int counter=0;
    QString deviceName;
    int windowIndex;


public:
    // Constructor
    ImageAcquisition(QObject *parent=0);

    // Functions
    bool HImage2QImage(HalconCpp::HImage &from, QImage &to);
    CameraControls setupCameraControls();
    HalconCpp::HTuple getValueForParam(std::string paramString);
    void setValueForParam(std::string paramString, int paramValue);
    void setValueForParam(std::string paramString, std::string paramValue);
    void setValueForParam(std::string paramString, double paramValue);

    // Setters and Getters
    int getWindowIndex() const;
    void setWindowIndex(int value);

    QString getDeviceName() const;
    void setDeviceName(const QString &value);


    HalconCpp::HFramegrabber getAcq() const;
    void setAcq(const HalconCpp::HFramegrabber &value);

    bool getStop() const;
    void setStop(bool value);

    int getCounter() const;
    void setCounter(int value);


signals:
    void renderImageSignal(QImage, int);

protected:
    void run();

};

#endif // IMAGEACQUISITION_H

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
#include "defaults.h"
#include <QSignalMapper>
#include <QQueue>
#include<QtCharts/QLineSeries>
using namespace QtCharts;


// Custom class to store image and save path in a queue (Buffer for writing)
class RecordingBuffer
{
    // Access specifier
public:

    // Data Members
    HalconCpp::HImage image;
    QString imageSavePath;

    //Default Constructor
    RecordingBuffer(HalconCpp::HImage image, QString imageSavePath)
    {
        this->image = image;
        this->imageSavePath=imageSavePath;

    }
};

class ImageAcquisition : public QThread
{
    Q_OBJECT

private:
    HalconCpp::HFramegrabber imageAcquisitionHandle;
    bool stopAcquisition=false;
    int counter=0;
    QString deviceName, deviceType, deviceMake;
//    CameraControls cameraControls;
    bool recording=false;
    bool supplyHistogramData=false;



public:
    // Constructor
    ImageAcquisition(QString deviceType, QString deviceMake, QString deviceName, QObject *parent=0);
    HalconCpp::HImage currentImage;
    // Image Buffer
    QQueue<RecordingBuffer> imageBuffer;
//    QString uid;
    QString currentRecordSaveDir;
    int currentBufferImageCounter;
    bool writeInProgress=false;
    double imageRotation = 0.0;
    bool enableGrid = false;
    bool mirrorImageHorizontal=false;
    bool mirrorImageVertical=false;
    double dynamicWidth=100.0, dynamicHeight=100.0;

    // Member Functions
    bool HImage2QImage(HalconCpp::HImage &from, QImage &to);
    void setupCameraControls();
    HalconCpp::HTuple getValueForParam(std::string paramString);
//    void setValueForParam(std::string paramString, int paramValue);
    void setValueForParam(std::string paramString, std::string paramValue);
    void setValueForParam(std::string paramString, double paramValue);
    void setValueForParam(std::string paramString, bool paramState);
//    void setValueForParam(std::string paramString, long paramValue);
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
    void setRecording(bool value){recording=value;}
    bool getRecording(){return recording;}
    bool getSupplyHistogramData() const;
    void setSupplyHistogramData(bool value);

    QString getDeviceType() const;
    void setDeviceType(const QString &value);

    QString getDeviceMake() const;
    void setDeviceMake(const QString &value);

signals:
    void renderImageSignal(QImage);
    void updateStatusBarSignal(QString);
    void renderHistogramSignal(QList<QLineSeries*>, int);


protected:
    void run();

};

#endif // IMAGEACQUISITION_H

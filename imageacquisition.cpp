#include "imageacquisition.h"
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QCoreApplication>
#include "defaults.h"



ImageAcquisition::ImageAcquisition(QObject *parent): QThread(parent)
{
    
}

void ImageAcquisition::run()
{
    QImage qImage;
    using namespace HalconCpp;
    qDebug() << "In run. Connected to device  "<< deviceName;
    while(!stop)
    {
        HImage image = this->getAcq().GrabImage();
        Hlong  width,height;
        image.GetImageSize(&width,&height);
        image = image.ZoomImageSize(600,600,"constant");
        //        image.GetImageSize(&width,&height);
        //        qDebug()<< "Image size halcon "<<width<<height;
        //        HalconCpp::WriteImage(image,"tiff",0,"C:/Users/daruizcadalso/Documents/QTApplications/USoundApp/sample.jpg");
        //        break;
        auto conversionStatus = HImage2QImage(image, qImage);
        if (!conversionStatus)
        {
            // failed to convert himage to qimage. Handle it here
            QCoreApplication::quit();
        }
        //        msleep(1000);
        emit renderImageSignal(qImage, this->getWindowIndex());
        counter++;
        qDebug()<< this->getValueForParam(HalconCameraParameters::RESULTINGFRAMERATE).DArr()[0];

    }



}




//*
// * @brief HImage2QImage Converts Halcon's HImage to Qt's QImage
//       * @param from HImage , currently only supports 8bits grayscale image and 8bits 3 channel color image
//       * @param to QImage , where from and to do not share memory. If the memory size of to is appropriate, then there is no need to redistribute memory. So you can speed up.
//       * @return true means the conversion was successful, false means the conversion failed

bool ImageAcquisition::HImage2QImage(HalconCpp::HImage &from, QImage &to)
{
    using namespace HalconCpp;
    Hlong width;
    Hlong height;
    from.GetImageSize(&width, &height);

    HTuple channels = from.CountChannels();
    HTuple type = from.GetImageType();

    if( strcmp(type[0].S(), "byte" )) // if it is not a byte type, it fails
    {
        return false;
    }

    QImage::Format format;
    switch(channels[0].I())
    {
    case 1:
        format = QImage::Format_Grayscale8;
        break;
    case 3:
        format = QImage::Format_RGB32;
        break;
    default:
        return false;
    }

    if(to.width() != width || to.height() != height || to.format() != format)
    {
        to = QImage(static_cast<int>(width),
                    static_cast<int>(height),
                    format);
    }
    HString Type;
    if(channels[0].I() == 1)
    {
        unsigned char * pSrc = reinterpret_cast<unsigned char *>( from.GetImagePointer1(&Type, &width, &height) );
        memcpy( to.bits(), pSrc, static_cast<size_t>(width) * static_cast<size_t>(height) );
        return true;
    }
    else if(channels[0].I() == 3)
    {
        uchar *R, *G, *B;
        from.GetImagePointer3(reinterpret_cast<void **>(&R),
                              reinterpret_cast<void **>(&G),
                              reinterpret_cast<void **>(&B), &Type, &width, &height);

        for(int row = 0; row < height; row ++)
        {
            QRgb* line = reinterpret_cast<QRgb*>(to.scanLine(row));
            for(int col = 0; col < width; col ++)
            {
                line[col] = qRgb(*R++, *G++, *B++);
            }
        }
        return true;
    }

    return false;
}

CameraControls ImageAcquisition::setupCameraControls()
{

    // First set all default parameters.
    CameraControlDefaults cameraControlsDefaults;
    CameraControls cameraControlSettings(cameraControlsDefaults);
    HalconCpp::HTuple value;

    //        Hue
    //        Saturation
    //        Brightness
    //        Contrast
    //        Monochrome
    //        RGB
    // The above parameters need to be set too, but Basler does not have a keyword matching these params
    // If a wrong parameter name is entered the only watch to gracefully check it is using try catch
    try {
        value = this->getValueForParam(HalconCameraParameters::GAIN);
        cameraControlSettings.setAnalogGain(value.ToDArr()[0]);

        value = this->getValueForParam(HalconCameraParameters::AUTOEXPOSURE);
        QString::compare(value.SArr()[0],"off") == 0?cameraControlSettings.setAutoExposure(false):cameraControlSettings.setAutoExposure(true);

        value = this->getValueForParam(HalconCameraParameters::EXPOSURETIME);
        cameraControlSettings.setExposureTime(value.DArr()[0]);

        value = this->getValueForParam(HalconCameraParameters::GAMMA);
        cameraControlSettings.setGamma(value.DArr()[0]);

        value = this->getValueForParam(HalconCameraParameters::RESULTINGFRAMERATE);
        cameraControlSettings.setFrameRate(value.DArr()[0]);
    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception occured while accessing camera parameter "<<e.ErrorMessage().Text() << e.ErrorCode();
    }

    return cameraControlSettings;

}

HalconCpp::HTuple ImageAcquisition::getValueForParam(std::string paramString)
{
    qDebug() << " Querying ::"<< paramString.c_str();
    return this->acq.GetFramegrabberParam(paramString.c_str());
}

void ImageAcquisition::setValueForParam(std::string paramString, int paramValue)
{
    try {
        qDebug() << "in int set param"<< paramString.c_str() << paramValue;
        this->acq.SetFramegrabberParam(paramString.c_str(), paramValue);

    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:int message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}

void ImageAcquisition::setValueForParam(std::string paramString, double paramValue)
{
    try {
        qDebug() << "double set param"<< paramString.c_str() << paramValue;
        this->acq.SetFramegrabberParam(paramString.c_str(), paramValue);

    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:double message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}

void ImageAcquisition::setValueForParam(std::string paramString, std::string paramValue)
{
    try {
        qDebug() << "string set param"<< paramString.c_str() << paramValue.c_str();
        this->acq.SetFramegrabberParam(paramString.c_str(), paramValue.c_str());

    }  catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:string message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}


// Setters and Getters
bool ImageAcquisition::getStop() const
{
    return stop;
}

void ImageAcquisition::setStop(bool value)
{
    stop = value;
}

HalconCpp::HFramegrabber ImageAcquisition::getAcq() const
{
    return acq;
}

void ImageAcquisition::setAcq(const HalconCpp::HFramegrabber &value)
{
    acq = value;
}

QString ImageAcquisition::getDeviceName() const
{
    return deviceName;
}

void ImageAcquisition::setDeviceName(const QString &value)
{
    deviceName = value;
}

int ImageAcquisition::getWindowIndex() const
{
    return windowIndex;
}

void ImageAcquisition::setWindowIndex(int value)
{
    windowIndex = value;
}

int ImageAcquisition::getCounter() const
{
    return counter;
}

void ImageAcquisition::setCounter(int value)
{
    counter = value;
}


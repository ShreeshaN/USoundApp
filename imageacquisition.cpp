#include "imageacquisition.h"
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QCoreApplication>
#include "defaults.h"
#include <exception>

CameraControls ImageAcquisition::getCameraControls() const
{
    return cameraControls;
}

void ImageAcquisition::setCameraControls(const CameraControls &value)
{
    cameraControls = value;
}

ImageAcquisition::ImageAcquisition(QString deviceName, QObject *parent): QThread(parent)
{
    HalconCpp::HFramegrabber imageAcquisitionHandle("USB3Vision",
                                                    0, 0, 0, 0, 0, 0,
                                                    "progressive", -1, "default",
                                 -1, "false", "default",
                                 HalconCpp::HString(
                                     deviceName.toLocal8Bit().constData()).Text(), 0, -1);
    this->imageAcquisitionHandle = imageAcquisitionHandle;
    this->deviceName = deviceName;
}

void ImageAcquisition::setup()
{
    setupCameraControls();

}

void ImageAcquisition::run()
{
    QImage qImage;
    using namespace HalconCpp;
    while(!stopAcquisition)
    {
//        long int before = GetTickCount();
        currentImage = this->imageAcquisitionHandle.GrabImage();

        Hlong  width,height;
        currentImage.GetImageSize(&width,&height);
        // todo: Prathyush SP currentImage is saved to the disk. Check the importance of the resolution saved?
        HImage zoomedImage = currentImage.ZoomImageSize(600,600,"constant");

        //        image.GetImageSize(&width,&height);
        //        qDebug()<< "Image size halcon "<<width<<height;
        //        HalconCpp::WriteImage(image,"tiff",0,"C:/Users/daruizcadalso/Documents/QTApplications/USoundApp/sample.jpg");
        //        break;
        auto conversionStatus = HImage2QImage(zoomedImage, qImage);


        if (!conversionStatus)
        {
            // failed to convert himage to qimage. Handle it here
            QCoreApplication::quit();
        }
        //        msleep(1000);

        emit renderImageSignal(qImage);
//        long int after = GetTickCount();

        counter++;
//        qDebug() << before << after<<(after-before)/1000.0;

//        qDebug()<< "Frame rate :"<<this->getValueForParam(HalconCameraParameters::RESULTINGFRAMERATE).DArr()[0];

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

void ImageAcquisition::setupCameraControls()
{

    // First set all default parameters.
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
        value = this->getValueForParam(HalconCameraParameterNames::GAIN);
        cameraControls.setAnalogGain(value.ToDArr()[0]);

        value = this->getValueForParam(HalconCameraParameterNames::AUTOEXPOSURE);
        QString::compare(value.SArr()[0],"Off") == 0?cameraControls.setAutoExposure(false):cameraControls.setAutoExposure(true);


        value = this->getValueForParam(HalconCameraParameterNames::EXPOSURETIME);
        cameraControls.setExposureTime(value.D());

        value = this->getValueForParam(HalconCameraParameterNames::GAMMA);
        cameraControls.setGamma(value.D());

        value = this->getValueForParam(HalconCameraParameterNames::ACQUISITIONFRAMERATE);
        cameraControls.setAcquisitionFrameRate(value.D());


        value = this->getValueForParam(HalconCameraParameterNames::RESULTINGFRAMERATE);
        cameraControls.setResultingFrameRate(value.D());

    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception occured while accessing camera parameter "<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}

HalconCpp::HTuple ImageAcquisition::getValueForParam(std::string paramString)
{
    try {
        return imageAcquisitionHandle.GetFramegrabberParam(paramString.c_str());
    } catch (std::exception &e) {
        qDebug() << e.what();
    }


}

void ImageAcquisition::setValueForParam(std::string paramString, int paramValue)
{
    try {
        qDebug() << "in int set param"<< paramString.c_str() << paramValue;
        imageAcquisitionHandle.SetFramegrabberParam(paramString.c_str(), paramValue);

    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:int message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}

void ImageAcquisition::setValueForParam(std::string paramString, double paramValue)
{
    try {
        qDebug() << "double set param"<< paramString.c_str() << paramValue;
        imageAcquisitionHandle.SetFramegrabberParam(paramString.c_str(), paramValue);

    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:double message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}

void ImageAcquisition::setValueForParam(std::string paramString, long paramValue)
{
    try {
        qDebug() << "long set param"<< paramString.c_str() << paramValue;
        imageAcquisitionHandle.SetFramegrabberParam(paramString.c_str(), (Hlong)paramValue);

    } catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:double message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}

void ImageAcquisition::setValueForParam(std::string paramString, std::string paramValue)
{
    try {
//        qDebug() << "string set param"<< paramString.c_str() << paramValue.c_str();
       imageAcquisitionHandle.SetFramegrabberParam(paramString.c_str(), paramValue.c_str());

    }  catch (HalconCpp::HException &e) {
        qDebug() << "Exception in setting param value:string message"<<e.ErrorMessage().Text() << e.ErrorCode();
    }
}


// Setters and Getters
QString ImageAcquisition::getDeviceName() const
{
    return deviceName;
}

void ImageAcquisition::setDeviceName(const QString &value)
{
    deviceName = value;
}

int ImageAcquisition::getCounter() const
{
    return counter;
}

void ImageAcquisition::setCounter(int value)
{
    counter = value;
}

HalconCpp::HFramegrabber ImageAcquisition::getImageAcquisitionHandle() const
{
    return imageAcquisitionHandle;
}

void ImageAcquisition::setImageAcquisitionHandle(const HalconCpp::HFramegrabber &value)
{
    imageAcquisitionHandle = value;
}

bool ImageAcquisition::getStopAcquisition() const
{
    return stopAcquisition;
}

void ImageAcquisition::setStopAcquisition(bool value)
{
    stopAcquisition = value;
}

void ImageAcquisition::startAquisition(){
    ImageAcquisition::run();
}

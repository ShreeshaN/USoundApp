#ifndef IMAGEACQUISITION_H
#define IMAGEACQUISITION_H
#include <QThread>
#include <HalconCpp.h>
#include <Halcon.h>
#include "HFramegrabber.h"
#include <HFramegrabber.h>
#include <QImage>





class ImageAcquisition : public QThread
{
    Q_OBJECT
public:
    ImageAcquisition(QObject *parent=0);
    bool HImage2QImage(HalconCpp::HImage &from, QImage &to);

    HalconCpp::HFramegrabber acq;
    bool stop=false;


    int getWindowIndex() const;
    void setWindowIndex(int value);

    QString getDeviceName() const;
    void setDeviceName(const QString &value);


    HalconCpp::HFramegrabber getAcq() const;
    void setAcq(const HalconCpp::HFramegrabber &value);

private:
    QString deviceName;
    int windowIndex;

signals:
    void renderImageSignal(QImage, int);

protected:
    void run();

};

#endif // IMAGEACQUISITION_H

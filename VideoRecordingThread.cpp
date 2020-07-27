#include<VideoRecordingThread.h>
#include <QDebug>
#include <usoundutils.h>


VideoRecordingThread::VideoRecordingThread(ImageAcquisition *imageAcquisitionThread):QThread()
{
    this->imageAcquisitionThread = imageAcquisitionThread;
    this->uid = QString(generateTimeStamp().c_str());
}


void VideoRecordingThread::run()
{
    QDir dir;
    QString currentVideoDir = getVideoSavePathForDevice(imageAcquisitionThread->getDeviceName())+"/"+uid;
    dir.mkpath(currentVideoDir);
    int counter = 1;
    qDebug() << "Starting record . . .";
    while(imageAcquisitionThread->getRecording()){
        try {
            QString filename=currentVideoDir+"/"+QString(std::to_string(counter).c_str())+"."+Directories::IMAGEFORMAT;
            qDebug() << "Saving frame at "+filename;
            imageAcquisitionThread->currentImage.WriteImage(Directories::IMAGEFORMAT.toStdString().c_str(), 0, filename.toStdString().c_str());
            counter+=1;
        } catch (HalconCpp::HException he) {
            qDebug() << he.ErrorMessage().Text();
        }

    }
    qDebug() << "Recording stopped . . .";

}

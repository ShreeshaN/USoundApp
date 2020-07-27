#include<VideoRecordingThread.h>
#include <QDebug>
#include <usoundutils.h>


VideoRecordingThread::VideoRecordingThread(ImageAcquisition *imageAcquisitionThread):QThread()
{
    this->imageAcquisitionThread = imageAcquisitionThread;
}


void VideoRecordingThread::run()
{
    while(imageAcquisitionThread->getRecording()){
     qDebug() << "hello from worker thread " << imageAcquisitionThread->getDeviceName();
     mssleep(1000);
    }
    qDebug() << "Recodring stopped . . .";

}

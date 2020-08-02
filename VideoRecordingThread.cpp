#include<VideoRecordingThread.h>
#include <QDebug>
#include <usoundutils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include<QQueue>
#include<imageacquisition.h>
#include <iostream>
#include <thread>

using namespace std;


VideoRecordingThread::VideoRecordingThread(ImageAcquisition *imageAcquisitionThread):QThread()
{
    this->imageAcquisitionThread = imageAcquisitionThread;
}

void VideoRecordingThread::run()
{
    QDir dir;


    int imageCounter = 0;
    qDebug() << "Starting record . . .";    
    qDebug() << "Total Images in buffer  " + QString::number(imageAcquisitionThread->imageBuffer.length());
    try {
        while(!imageAcquisitionThread->imageBuffer.isEmpty()){
            RecordingBuffer buffer = imageAcquisitionThread->imageBuffer.dequeue();
            qDebug() << "Saving frame at "+buffer.imageSavePath;
            buffer.image.WriteImage(Directories::IMAGEFORMAT.toStdString().c_str(), 0, buffer.imageSavePath.toStdString().c_str());
            emit imageAcquisitionThread->updateStatusBar(QString("Remaining images to write %1").arg(imageAcquisitionThread->imageBuffer.length()));
            imageCounter+=1;
        }
        qDebug() << "Total Images Written " + QString::number(imageCounter);
    }catch (HalconCpp::HException he) {
        qDebug() << he.ErrorMessage().Text();
    }catch (std::exception &e) {
        qCritical() << e.what();
    }
    qDebug() << "Recording stopped . . .";
}

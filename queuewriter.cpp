#include<queuewriter.h>
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


QueueWriter::QueueWriter(ImageAcquisition *imageAcquisitionThread):QThread()
{
    this->imageAcquisitionThread = imageAcquisitionThread;
}


void QueueWriter::run()
{

    QDir dir;
    int imageCounter = 0;
    void *queueWriterThreadId = this->currentThreadId();
    void *imageAquisitionThreadId = imageAcquisitionThread->currentThreadId();
    qDebug() << QString("Starting to write using %1 thread for imAq %2 thread").arg((long long)queueWriterThreadId, 16).arg((long long)imageAquisitionThreadId, 16);
    qDebug() << "Total Images in buffer  " + QString::number(imageAcquisitionThread->imageBuffer.length());
    imageAcquisitionThread->writeInProgress = true;
    try {
        while(!imageAcquisitionThread->imageBuffer.isEmpty()){
            RecordingBuffer buffer = imageAcquisitionThread->imageBuffer.dequeue();
            qDebug() << "Saving frame at "+buffer.imageSavePath;
            buffer.image.WriteImage(DIRECTORIES::IMAGEFORMAT.toStdString().c_str(), 0, buffer.imageSavePath.toStdString().c_str());            
            emit imageAcquisitionThread->updateStatusBarSignal(QString("Remaining images to write %1").arg(imageAcquisitionThread->imageBuffer.length()));
            imageCounter+=1;
        }
    imageAcquisitionThread->writeInProgress = false;
        qDebug() << "Total Images Written " + QString::number(imageCounter);
    }catch (HalconCpp::HException he) {
        qDebug() << he.ErrorMessage().Text();
    }catch (std::exception &e) {
        qCritical() << e.what();
    }
    catch (const std::bad_alloc&) {
      qCritical("Bad Allocation caught");
      abort();
    }
    qDebug() << "Writing stopped.";
}

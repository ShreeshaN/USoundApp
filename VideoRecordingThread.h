#ifndef VIDEORECORDINGTHREAD_H
#define VIDEORECORDINGTHREAD_H
#include<QThread>
#include <imageacquisition.h>

// hellothread/hellothread.h
class VideoRecordingThread : public QThread
{
    Q_OBJECT
    ImageAcquisition *imageAcquisitionThread;


public:
    // Constructor
    VideoRecordingThread(ImageAcquisition *imageAcquisitionThread);

private:
    void run();
};



#endif // VIDEORECORDINGTHREAD_H


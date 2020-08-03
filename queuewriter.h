#ifndef QUEUEWRITER_H
#define QUEUEWRITER_H
#include<QThread>
#include <imageacquisition.h>


// hellothread/hellothread.h
class QueueWriter : public QThread
{
    Q_OBJECT
    ImageAcquisition *imageAcquisitionThread;

public:
    // Constructor
    QueueWriter(ImageAcquisition *imageAcquisitionThread);

private:
    void run();
};



#endif // VIDEORECORDINGTHREAD_H


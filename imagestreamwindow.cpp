#include "imagestreamwindow.h"
#include "imageacquisition.h"
#include <QMessageBox>
#include "homescreen.h"
#include <QHBoxLayout>
#include <QCheckBox>
#include <QStatusBar>
#include <QMenuBar>
#include<QToolBar>
#include<QDateTime>
#include<QDir>
#include <QtGui>
#include <usoundutils.h>
#include <queuewriter.h>
#include<defaults.h>
#include<QDir>
//#include<QToolTip>


ImageStreamWindow::ImageStreamWindow(QWidget *parent) : QMainWindow(parent)
{
}

void ImageStreamWindow::setupCameraWindow()
{
    imageSaveButton = this->menuBar()->addAction(tr("ImageSaveButton"));
    imageSaveButton->setIcon(QIcon(":icons/icon-single-shot.png"));
    // todo: Prathyush SP -> Fix issue with tooltip display
    imageSaveButton->setToolTip("Save frame");
    connect(imageSaveButton, SIGNAL(triggered()), this, SLOT(saveImage()));

    recordButton = this->menuBar()->addAction(tr("RecordButton"));
    recordButton->setIcon(QIcon(":icons/icon-media-record.png"));
    connect(recordButton, SIGNAL(triggered()), this, SLOT(startVideoRecord()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(writeQueue()));
    timer->start(1000);

    //    recordPauseButton = this->menuBar()->addAction(tr("RecordPauseButton"));
    //    recordPauseButton->setIcon(QIcon(":icons/icon-media-playback-pause.png"));
    //    recordPauseButton->setDisabled(true);
    //    connect(recordPauseButton, SIGNAL(triggered()), this, SLOT(pauseVideoRecord()));

    recordStopButton = this->menuBar()->addAction(tr("RecordStopButton"));
    recordStopButton->setIcon(QIcon(":icons/icon-media-playback-stop.png"));
    recordStopButton->setDisabled(true);
    connect(recordStopButton, SIGNAL(triggered()), this, SLOT(stopVideoRecord()));


    QOverload<int> qOverloadInt;
    QOverload<double> qOverloadDouble;
    QWidget *widget = new QWidget(this);
    QHBoxLayout * hlayout = new QHBoxLayout(widget);


    this->setCentralWidget(widget);
    // Setting up graphics view
    graphicsView = new QGraphicsView();
    //        this->setCentralWidget(graphicsView);
    graphicsScene = new QGraphicsScene();
    graphicsView->setScene(graphicsScene);
    graphicsPixmapItem = new QGraphicsPixmapItem();
    graphicsView->scene()->addItem(graphicsPixmapItem);
    graphicsPixmapItemList.append(graphicsPixmapItem);

    QTreeWidget *ccTreeWidget = new QTreeWidget;
    ccTreeWidget->setColumnCount(2);
    ccTreeWidget->setHeaderLabels(QStringList() <<"Feature"<<"Value");

    QList<QTreeWidgetItem *> topLevelItems;

    // exposure controls
    // parent
    exposureControls = new QTreeWidgetItem();
    exposureControls->setText(0, "Exposure Controls");
    topLevelItems.append(exposureControls);

    // child
    exposureTime = new QTreeWidgetItem(QStringList() << "Exposure Time");
    exposureTimeSpinBox = new QSpinBox;
    exposureTimeSpinBox->setRange(0, 10000);
    exposureTimeSpinBox->setSingleStep(1);
    exposureTimeSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getExposureTime());
    connect(exposureTimeSpinBox, qOverloadInt(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setExposureTime(exposureTimeSpinBox->value());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::EXPOSURETIME,exposureTimeSpinBox->value());
        updateCameraParametersAndDisplay();
    });

    analogGain = new QTreeWidgetItem(QStringList() << "Analog Gain");
    analogGainSpinBox = new QDoubleSpinBox;
    analogGainSpinBox->setRange(0, 36); // 8-bit; for 12-bit device max value is 24; Need a framework to define all parameters
    analogGainSpinBox->setSingleStep(0.1);
    analogGainSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getAnalogGain());
    connect(analogGainSpinBox, qOverloadDouble(&QDoubleSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setAnalogGain(analogGainSpinBox->value());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::GAIN,analogGainSpinBox->value());
        updateCameraParametersAndDisplay();
    });

    autoExposure = new QTreeWidgetItem(QStringList() << "Auto Exposure");
    autoExposureCheckbox = new QCheckBox();
    autoExposureCheckbox->setCheckState(this->imageAcquisitionThread->getCameraControls().getAutoExposure()?Qt::CheckState(2):Qt::CheckState(0));
    connect(autoExposureCheckbox, &QCheckBox::stateChanged, [=]{
        // is this below assignment required here and for every other
        // widget? coz anyway updatecameraparameteranddisplay
        // will query the camera and update cameracontrols obj
        this->imageAcquisitionThread->getCameraControls().setAutoExposure(autoExposureCheckbox->isChecked());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::AUTOEXPOSURE,autoExposureCheckbox->isChecked()?"Once":"Off");
        mssleep(500); // Waiting for the hardware to update exposure time based on Autoexposure
        updateCameraParametersAndDisplay();


    });

    exposureControls->addChild(exposureTime);
    exposureControls->addChild(analogGain);
    exposureControls->addChild(autoExposure);
    ccTreeWidget->setItemWidget(exposureTime, 1, exposureTimeSpinBox);
    ccTreeWidget->setItemWidget(analogGain, 1, analogGainSpinBox);
    ccTreeWidget->setItemWidget(autoExposure, 1, autoExposureCheckbox);

    // color appearance parameters
    // parent
    colorAppearance = new QTreeWidgetItem();
    colorAppearance->setExpanded(true);
    colorAppearance->setText(0, "Color and Appearance");
    topLevelItems.append(colorAppearance);


    // child
    hue = new QTreeWidgetItem(QStringList() << "Hue");
    hueSpinBox = new QSpinBox(this);
    hueSpinBox->setRange(0, 100);
    hueSpinBox->setSingleStep(1);
    hueSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getHue());
    colorAppearance->addChild(hue);
    connect(hueSpinBox, qOverloadInt(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setHue(hueSpinBox->value());
        // imageAcquisitionThread->setValueForParam(HalconCameraParameters::HUE,hueSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(hue, 1, hueSpinBox);


    saturation = new QTreeWidgetItem(QStringList() << "Saturation");
    saturationSpinBox = new QSpinBox;
    saturationSpinBox->setRange(0, 100);
    saturationSpinBox->setSingleStep(1);
    saturationSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getSaturation());
    colorAppearance->addChild(saturation);
    connect(saturationSpinBox, qOverloadInt(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setSaturation(saturationSpinBox->value());
        // imageAcquisitionThread->setValueForParam(HalconCameraParameters::SATURATION,saturationSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(saturation, 1, saturationSpinBox);


    brightness = new QTreeWidgetItem(QStringList() << "Brightness");
    brightnessSpinBox = new QSpinBox;
    brightnessSpinBox->setRange(0, 100);
    brightnessSpinBox->setSingleStep(1);
    brightnessSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getBrightness());
    colorAppearance->addChild(brightness);
    connect(brightnessSpinBox, qOverloadInt(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setBrightness(brightnessSpinBox->value());
        // imageAcquisitionThread->setValueForParam(HalconCameraParameters::BRIGHTNESS,brightnessSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(brightness, 1, brightnessSpinBox);

    contrast = new QTreeWidgetItem(QStringList() << "Contrast");
    contrastSpinBox = new QSpinBox;
    contrastSpinBox->setRange(0, 100);
    contrastSpinBox->setSingleStep(1);
    contrastSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getContrast());
    colorAppearance->addChild(contrast);
    connect(contrastSpinBox, qOverloadInt(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setContrast(contrastSpinBox->value());
        // imageAcquisitionThread->setValueForParam(HalconCameraParameters::CONTRAST,contrastSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(contrast, 1, contrastSpinBox);

    gamma = new QTreeWidgetItem(QStringList() << "Gamma");
    gammaSpinBox = new QDoubleSpinBox;
    gammaSpinBox->setRange(0, 4);
    gammaSpinBox->setSingleStep(0.1);
    gammaSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getGamma());
    colorAppearance->addChild(gamma);
    connect(gammaSpinBox, qOverloadDouble(&QDoubleSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setGamma(gammaSpinBox->value());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::GAMMA,gammaSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(gamma, 1, gammaSpinBox);

    acquisitionFrameRateEnable = new QTreeWidgetItem(QStringList() << "AcqusitionFrameRateEnable");
    acquisitionFrameRateEnableCheckbox = new QCheckBox();
    acquisitionFrameRateEnableCheckbox->setTristate(false);
    acquisitionFrameRateEnableCheckbox->setCheckState( this->imageAcquisitionThread->getCameraControls().getAcquisitionFrameRateEnable()?Qt::CheckState(2):Qt::CheckState(0));
    colorAppearance->addChild(acquisitionFrameRateEnable);
    connect(acquisitionFrameRateEnableCheckbox, &QCheckBox::clicked, [=]{
        this->imageAcquisitionThread->getCameraControls().setAcquisitionFrameRateEnable(acquisitionFrameRateEnableCheckbox->isChecked());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::ACQUISITIONFRAMERATEENABLE,acquisitionFrameRateEnableCheckbox->isChecked());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(acquisitionFrameRateEnable, 1, acquisitionFrameRateEnableCheckbox);

    acquisitionFrameRate = new QTreeWidgetItem(QStringList() << " Acquition Frame Rate");
    acquisitionFramerateSpinBox = new QSpinBox;
    acquisitionFramerateSpinBox->setRange(0, 120);
    acquisitionFramerateSpinBox->setSingleStep(1);
    acquisitionFramerateSpinBox->setValue(this->imageAcquisitionThread->getCameraControls().getAcquisitionFrameRate());

    // SLider experiment
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setFocusPolicy(Qt::StrongFocus);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);
    slider->setSingleStep(1);


    colorAppearance->addChild(acquisitionFrameRate);
    connect(acquisitionFramerateSpinBox, qOverloadInt(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setAcquisitionFrameRate(acquisitionFramerateSpinBox->value());
        qDebug() << "value "<< acquisitionFramerateSpinBox->value();
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::ACQUISITIONFRAMERATE,acquisitionFramerateSpinBox->value());
        //        Sleep(500); // Waiting for the hardware to update exposure time based on Autoexposure
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(acquisitionFrameRate, 1, acquisitionFramerateSpinBox);



    monochrome = new QTreeWidgetItem(QStringList() << "Monochrome");
    monochromeCheckbox = new QCheckBox();
    monochromeCheckbox->setTristate(false);

    monochromeCheckbox->setCheckState( this->imageAcquisitionThread->getCameraControls().getMonochrome()?Qt::CheckState(2):Qt::CheckState(0));
    colorAppearance->addChild(monochrome);
    connect(monochromeCheckbox, &QCheckBox::clicked, [=]{
        this->imageAcquisitionThread->getCameraControls().setMonochrome(monochromeCheckbox->isChecked());
        // imgAcq->setValueForParam(HalconCameraParameters::MONOCHROME,monochromeCheckbox->isChecked());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(monochrome, 1, monochromeCheckbox);

    rgb = new QTreeWidgetItem(QStringList() << "RGB");
    rgbCheckbox = new QCheckBox();
    rgbCheckbox->setTristate(false);
    rgbCheckbox->setCheckState( this->imageAcquisitionThread->getCameraControls().getRgb()?Qt::CheckState(2):Qt::CheckState(0));
    colorAppearance->addChild(rgb);
    connect(rgbCheckbox, &QCheckBox::clicked, [=]{
        this->imageAcquisitionThread->getCameraControls().setRgb(rgbCheckbox->isChecked());
        // imageAcquisitionThread->setValueForParam(HalconCameraParameters::RGB,rgbCheckbox->isChecked());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(rgb, 1, rgbCheckbox);
    ccTreeWidget->addTopLevelItems(topLevelItems);
    exposureControls->setExpanded(true);
    colorAppearance->setExpanded(true);

    qDebug() << "Resulting frame rate" << this->getImageAcquisitionThread()->getCameraControls().getResultingFrameRate();


    // resulting frame rate in status bar
    qDebug() << "setting resulting frame rate";
    this->statusBar()->showMessage("Frame Rate: "+ QString::number(this->getImageAcquisitionThread()->getCameraControls().getResultingFrameRate()));

    hlayout->addWidget(ccTreeWidget);
    hlayout->addWidget(graphicsView);

}



void ImageStreamWindow::updateCameraParametersAndDisplay()
{
    // read all parameters from camera into cameracontrols object
    imageAcquisitionThread->setupCameraControls();

    // update UI based on contents in cameracontrols object
    displayCameraParameters();

}

void ImageStreamWindow::displayCameraParameters()
{
    exposureTimeSpinBox->setValue(imageAcquisitionThread->getCameraControls().getExposureTime());
    analogGainSpinBox->setValue(imageAcquisitionThread->getCameraControls().getAnalogGain());
    autoExposureCheckbox->setCheckState(imageAcquisitionThread->getCameraControls().getAutoExposure()?Qt::CheckState(2):Qt::CheckState(0));

    hueSpinBox->setValue(imageAcquisitionThread->getCameraControls().getHue());
    saturationSpinBox->setValue(imageAcquisitionThread->getCameraControls().getSaturation());
    brightnessSpinBox->setValue(imageAcquisitionThread->getCameraControls().getBrightness());
    contrastSpinBox->setValue(imageAcquisitionThread->getCameraControls().getContrast());
    acquisitionFramerateSpinBox->setValue(imageAcquisitionThread->getCameraControls().getAcquisitionFrameRate());
    acquisitionFrameRateEnableCheckbox->setCheckState(imageAcquisitionThread->getCameraControls().getAcquisitionFrameRateEnable()?Qt::CheckState(2):Qt::CheckState(0));
    monochromeCheckbox->setCheckState(imageAcquisitionThread->getCameraControls().getMonochrome()?Qt::CheckState(2):Qt::CheckState(0));
    rgbCheckbox->setCheckState(imageAcquisitionThread->getCameraControls().getRgb()?Qt::CheckState(2):Qt::CheckState(0));
}

void ImageStreamWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Checking for pending images in buffer";
    if(!recordButton->isEnabled()){
        QMessageBox::critical(this, "Close Camera","Image recording in progress",QMessageBox::Ok);
    }
    else if (!imageAcquisitionThread->imageBuffer.isEmpty()){
        QMessageBox::critical(this, "Close Camera","Image save in progress",QMessageBox::Ok);
    }
    else{
        ImageAcquisition *imageAcquisitionThread = this->imageAcquisitionThread;
        imageAcquisitionThread->setStopAcquisition(true);

        try {
            qDebug() << "Closing device "<< imageAcquisitionThread->getDeviceName()<< ". . . ";
            imageAcquisitionThread->getImageAcquisitionHandle().CloseFramegrabber();
        } catch (HalconCpp::HException &) {
            //todo: add dialog box here
            qDebug() << "Error: Could not close message box";
            QMessageBox::critical(this, "Close Camera","Error. Could not close camera",QMessageBox::Ok);
        }
        imageAcquisitionThread->exit();
        qDebug() << "Counter value: "<<imageAcquisitionThread->getCounter();
    }
}

void ImageStreamWindow::renderImage(QImage qImage)
{

    // QLABEL WAY OF STREAMING
    //    windowWidget = newCameraConnectionWindows.at(widgetIndex);
    //    if(!windowWidget->imageAcquisitionThread->stop)
    //    {
    ////        qDebug() << "qimage size "<<qImage.width()<<qImage.height();
    //        imageLabel = newCameraConnectionLabels.at(widgetIndex);
    //        int height=windowWidget->height(); int width = windowWidget->width();
    //        imageLabel->setPixmap(QPixmap::fromImage(qImage));
    //        imageLabel->setScaledContents(true);
    //        imageLabel->resize(width, height);
    //        windowWidget->show();
    //    }

    // QGRAPHICSVIEW WAY OF STREAMING
    if(!imageAcquisitionThread->getStopAcquisition())
    {
        //        int height=this->height(); int width = this->width();
        graphicsPixmapItem->setPixmap(QPixmap::fromImage(qImage));
        this->show();
    }

    // resulting frame rate in status bar
    //    this->statusBar()->showMessage("Frame Rate: "+ QString::number(this->getImageAcquisitionThread()->getCameraControls().getResultingFrameRate()));



}

void ImageStreamWindow::saveImage()
/*
Save Image
*/
{
    try {
        //todo: Prathyush SP -> Change from tempPath to user set path
        QString filename=getImageSavePathForDevice(imageAcquisitionThread->getDeviceName())+"/"+QString(generateTimeStamp().c_str())+"."+Directories::IMAGEFORMAT;
        qInfo() << "Saving image at "+filename;
        imageAcquisitionThread->currentImage.WriteImage(Directories::IMAGEFORMAT.toStdString().c_str(), 0, filename.toStdString().c_str());
    } catch (HalconCpp::HException he) {
        qDebug() << he.ErrorMessage().Text();
    }
}


void ImageStreamWindow::startVideoRecord(){
    try {
        QDir dir;
        qDebug() << "Starting video record";
        //        qDebug() << "Images in buffer "+ QString(std::to_string(imageAcquisitionThread->imageBuffer.length()).c_str());
        imageAcquisitionThread->currentBufferImageCounter=0;
        imageAcquisitionThread->currentRecordSaveDir = getVideoSavePathForDevice(imageAcquisitionThread->getDeviceName())
                + "/" + QString(generateTimeStamp().c_str()) + "/";
        dir.mkpath(imageAcquisitionThread->currentRecordSaveDir);
        // Disable Record button and enable pause and stop button
        imageAcquisitionThread->setRecording(true);
        this->recordButton->setDisabled(true);
        //        this->recordPauseButton->setEnabled(true);
        this->recordStopButton->setEnabled(true);
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void ImageStreamWindow::stopVideoRecord(){
    try {
        qDebug() << "Stopping video record";
        // Disable Record button and enable pause and stop button
        imageAcquisitionThread->setRecording(false);
        this->recordButton->setEnabled(true);
        //        this->recordPauseButton->setDisabled(true);
        this->recordStopButton->setDisabled(true);

        qDebug() << "Launching thread to record images . . .";
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}


void ImageStreamWindow::pauseVideoRecord(){
    try {
        qDebug() << "Pausing video record";
        //todo: Prathyush SP - Implement pause functionality
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void ImageStreamWindow::updateStatusBar(QString statusMsg){
    try {
        this->statusBar()->showMessage(statusMsg);
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void ImageStreamWindow::writeQueue(){
    try {
        if (!imageAcquisitionThread->imageBuffer.isEmpty() && !imageAcquisitionThread->writeInProgress){
            QueueWriter *thread = new QueueWriter(imageAcquisitionThread);
            thread->start();
        }
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}


// Setters and Getters
ImageAcquisition *ImageStreamWindow::getImageAcquisitionThread() const
{
    return imageAcquisitionThread;
}

void ImageStreamWindow::setImageAcquisitionThread(ImageAcquisition *value)
{
    imageAcquisitionThread = value;
}

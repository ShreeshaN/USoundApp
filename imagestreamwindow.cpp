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
#include "usoundutils.h"
#include "doubleslider.h"
#include <queuewriter.h>
#include<defaults.h>
#include<QDir>
#include "spinboxcontainer.h"
#include "doublespinboxcontainer.h"
#include <typeinfo>
#include "checkboxcontainer.h"
#include <QScrollBar>
#include <defaultcameraparameternames.h>
#include <settingsstore.h>


ImageStreamWindow::ImageStreamWindow(ImageAcquisition* imageAcquisitionThread, QWidget *parent) : QMainWindow(parent)
{
    this->imageAcquisitionThread = imageAcquisitionThread;
    DefaultCameraParameterNames* cameraSettings = DefaultCameraParameterNames::getInstance();
    cameraParameters = cameraSettings->defaultCameraParameterNames.value(this->imageAcquisitionThread->getDeviceMake());
}

CameraParameters ImageStreamWindow::getCameraParameters() const
{
    return cameraParameters;
}

void ImageStreamWindow::setCameraParameters(const CameraParameters &value)
{
    cameraParameters = value;
}

void ImageStreamWindow::restoreDeviceSpecificSettings(){
    qDebug() << "Restoring device specific settings . . .";
    try {
        imageAcquisitionThread->imageRotation = SettingsStore::getDeviceSpecificSettings(imageAcquisitionThread->getDeviceName(),
                                                                                         "rotation", QString("0.0")).toDouble();
        imageAcquisitionThread->mirrorImage = SettingsStore::getDeviceSpecificSettings(imageAcquisitionThread->getDeviceName(),
                                                                                         "mirror", QString("%1").arg(false)).toBool();
        qDebug() << imageAcquisitionThread->mirrorImage;
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
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


    fixedAspectRatioButton = this->menuBar()->addAction(tr("FixedAspectRatioButton"));
    fixedAspectRatioButton->setIcon(QIcon(":icons/icon-fullscreen.png"));
    fixedAspectRatioButton->setCheckable(true);
    connect(fixedAspectRatioButton, SIGNAL(triggered()), this, SLOT(setFixedAspectRatio()));

    // Histogram
    grayHistogramButton = this->menuBar()->addAction(tr("GrayHistogramButton"));
    grayHistogramButton->setIcon(QIcon(":icons/icon-histogram.png"));
    connect(grayHistogramButton, SIGNAL(triggered()), this, SLOT(createHistogramWindow()));

    rotateClockwise90Button = this->menuBar()->addAction(tr("Rotate90Button"));
    rotateClockwise90Button->setIcon(QIcon(":icons/rotate.png"));
    connect(rotateClockwise90Button, SIGNAL(triggered()), this, SLOT(rotateAntiClockwise90Deg()));

    mirrorImage = this->menuBar()->addAction(tr("MirrorImage"));
    mirrorImage->setIcon(QIcon(":icons/flip.png"));
    connect(mirrorImage, SIGNAL(triggered()), this, SLOT(mirrorImageSlot()));


    //    QOverload<int> qOverloadInt;
    //    QOverload<double> qOverloadDouble;
    QWidget *widget = new QWidget(this);
    QHBoxLayout * hlayout = new QHBoxLayout(widget);


    this->setCentralWidget(widget);
    // Setting up graphics view
    graphicsView = new QGraphicsView();
    connect(graphicsView, SIGNAL(QGraphicsView::resizeEvent()), this, SLOT(saveImage()));

    //    connect(graphicsView, SIGNAL(triggered()), this, SLOT(stopVideoRecord()));
    //    graphicsView->setVerticalScrollBar(new QScrollBar());

    //        this->setCentralWidget(graphicsView);
    graphicsScene = new QGraphicsScene();

    graphicsView->setScene(graphicsScene);
    graphicsPixmapItem = new QGraphicsPixmapItem();

    graphicsView->scene()->addItem(graphicsPixmapItem);
    graphicsPixmapItemList.append(graphicsPixmapItem);

    QTreeWidget *ccTreeWidget = new QTreeWidget;
    ccTreeWidget->setColumnCount(2);
    ccTreeWidget->setHeaderLabels(QStringList() <<"Feature"<<"Value");

    // Set column width for ccTree (Dynamic resize only for image)
    ccTreeWidget->setFixedWidth(350);
    ccTreeWidget->setColumnWidth(0, 250);


    QList<QTreeWidgetItem *> topLevelItems;

    // exposure controls
    // parent
    exposureControls = new QTreeWidgetItem();
    exposureControls->setText(0, "Exposure Controls");
    topLevelItems.append(exposureControls);

    // child
    SpinboxContainer *exposureContainer = new SpinboxContainer(3000,cameraParameters.EXPOSURETIME,cameraParameters.EXPOSURETIME,0,1000000,50,imageAcquisitionThread);
    containers.append(exposureContainer);
    exposureControls->addChild(exposureContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(exposureContainer->getQTreeWidgetItem(), 1, exposureContainer->getUiElement());

    DoubleSpinboxContainer *analogGainContainer = new DoubleSpinboxContainer(5,cameraParameters.GAIN,cameraParameters.GAIN,0,36,0.1,imageAcquisitionThread);
    containers.append(analogGainContainer);
    exposureControls->addChild(analogGainContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(analogGainContainer->getQTreeWidgetItem(), 1, analogGainContainer->getUiElement());

    CheckboxContainer *autoGainContainer = new CheckboxContainer(false,cameraParameters.AUTOGAIN,cameraParameters.AUTOGAIN,"Once","Off",imageAcquisitionThread);
    containers.append(autoGainContainer);
    exposureControls->addChild(autoGainContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(autoGainContainer->getQTreeWidgetItem(), 1, autoGainContainer->getUiElement());

    CheckboxContainer *autoExposureContainer = new CheckboxContainer(false,cameraParameters.AUTOEXPOSURE,cameraParameters.AUTOEXPOSURE,"Once","Off",imageAcquisitionThread);
    containers.append(autoExposureContainer);
    exposureControls->addChild(autoExposureContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(autoExposureContainer->getQTreeWidgetItem(), 1, autoExposureContainer->getUiElement());

    // color appearance parameters
    // parent
    colorAppearance = new QTreeWidgetItem();
    colorAppearance->setExpanded(true);
    colorAppearance->setText(0, "Color and Appearance");
    topLevelItems.append(colorAppearance);


    // child
    SpinboxContainer *hueContainer = new SpinboxContainer(0,cameraParameters.HUE, cameraParameters.HUE,-180,180,1, imageAcquisitionThread);
    //    containers.append(hueContainer);
    colorAppearance->addChild(hueContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(hueContainer->getQTreeWidgetItem(), 1, hueContainer->getUiElement());

    SpinboxContainer *saturationContainer = new SpinboxContainer(0,cameraParameters.SATURATION,cameraParameters.SATURATION,0,100,1, imageAcquisitionThread);
    //    containers.append(saturationContainer);
    colorAppearance->addChild(saturationContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(saturationContainer->getQTreeWidgetItem(), 1, saturationContainer->getUiElement());

    SpinboxContainer *brightnessContainer = new SpinboxContainer(0,cameraParameters.BRIGHTNESS, cameraParameters.BRIGHTNESS,0,100,1, imageAcquisitionThread);
    //    containers.append(brightnessContainer);
    colorAppearance->addChild(brightnessContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(brightnessContainer->getQTreeWidgetItem(), 1, brightnessContainer->getUiElement());

    SpinboxContainer *contrastContainer = new SpinboxContainer(0,cameraParameters.CONTRAST,cameraParameters.CONTRAST,0,100,1, imageAcquisitionThread);
    //    containers.append(contrastContainer);
    colorAppearance->addChild(contrastContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(contrastContainer->getQTreeWidgetItem(), 1, contrastContainer->getUiElement());

    // SLider experiment
    //    DoubleSlider *slider = new DoubleSlider(Qt::Horizontal);
    //    slider->setFocusPolicy(Qt::StrongFocus);
    //    slider->setTickPosition(QSlider::TicksBothSides);
    //    slider->setTickInterval(0);
    //    slider->setSingleStep(1);
    //    slider->setRange(0,4);
    //    connect(slider, &DoubleSlider::doubleValueChanged,gammaSpinBox, &QDoubleSpinBox::setValue);
    //    connect(gammaSpinBox, qOverloadDouble(&QDoubleSpinBox::valueChanged),slider,&QSlider::setValue);
    //    QHBoxLayout * hu = new QHBoxLayout(widget);
    //    QWidget *gammaWidget = new QWidget;
    //    gammaWidget->setLayout(hu);
    //    hu->addWidget(gammaSpinBox);
    //    hu->addWidget(slider);
    //    ccTreeWidget->setItemWidget(gamma, 1, gammaWidget);

    //    colorAppearance->addChild(gamma);
    //    connect(gammaSpinBox, qOverloadDouble(&QDoubleSpinBox::valueChanged), [=]{
    //        this->imageAcquisitionThread->getCameraControls().setGamma(gammaSpinBox->value());
    //        imageAcquisitionThread->setValueForParam(cameraParameters.GAMMA,gammaSpinBox->value());
    //        updateCameraParametersAndDisplay();
    //    });

    DoubleSpinboxContainer *gammaContainer = new DoubleSpinboxContainer(0,cameraParameters.GAMMA,cameraParameters.GAMMA,0,4,0.1,imageAcquisitionThread);
    containers.append(gammaContainer);
    colorAppearance->addChild(gammaContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(gammaContainer->getQTreeWidgetItem(), 1, gammaContainer->getUiElement());

    CheckboxContainer *acquisitionFrameRateEnableContainer = new CheckboxContainer(false,cameraParameters.ACQUISITIONFRAMERATEENABLE,
                                                                                   cameraParameters.ACQUISITIONFRAMERATEENABLE,"","",imageAcquisitionThread);
    containers.append(acquisitionFrameRateEnableContainer);
    colorAppearance->addChild(acquisitionFrameRateEnableContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(acquisitionFrameRateEnableContainer->getQTreeWidgetItem(), 1, acquisitionFrameRateEnableContainer->getUiElement());


    SpinboxContainer *acquisitionFrameRateContainer = new SpinboxContainer(5,cameraParameters.ACQUISITIONFRAMERATE,cameraParameters.ACQUISITIONFRAMERATE,5,120,1, imageAcquisitionThread);
    containers.append(acquisitionFrameRateContainer);
    colorAppearance->addChild(acquisitionFrameRateContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(acquisitionFrameRateContainer->getQTreeWidgetItem(), 1, acquisitionFrameRateContainer->getUiElement());


    CheckboxContainer *monochromeContainer = new CheckboxContainer(false,cameraParameters.MONOCHROME,
                                                                   cameraParameters.MONOCHROME,"","",imageAcquisitionThread);
    containers.append(monochromeContainer);
    colorAppearance->addChild(monochromeContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(monochromeContainer->getQTreeWidgetItem(), 1, monochromeContainer->getUiElement());


    CheckboxContainer *rgbContainer = new CheckboxContainer(false,cameraParameters.RGB,
                                                            cameraParameters.RGB,"","",imageAcquisitionThread);
    containers.append(rgbContainer);
    colorAppearance->addChild(rgbContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(rgbContainer->getQTreeWidgetItem(), 1, rgbContainer->getUiElement());
    ccTreeWidget->addTopLevelItems(topLevelItems);

    exposureControls->setExpanded(true);
    colorAppearance->setExpanded(true);

    emit updateStatusBar("Frame Rate: "+ QString::number(this->getImageAcquisitionThread()->getValueForParam(cameraParameters.RESULTINGFRAMERATE).D()));
    // resulting frame rate in status bar
    hlayout->addWidget(ccTreeWidget);
    hlayout->addWidget(graphicsView);

    // TODO: THis is a temp fix. The below line of code currently has to be written for every parameter addition. Going forward find a level of abstraction for it.
    connect(exposureContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(analogGainContainer->getUiElement(), SIGNAL(valueChanged(double)),this,SLOT(updateAllParameters()));
    connect(autoGainContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    connect(autoExposureContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    //    connect(saturationContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    //    connect(hueContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    //    connect(brightnessContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    //    connect(contrastContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(acquisitionFrameRateEnableContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    connect(gammaContainer->getUiElement(), SIGNAL(valueChanged(double)),this,SLOT(updateAllParameters()));
    connect(acquisitionFrameRateContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    //    connect(monochromeContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    //    connect(rgbContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));

    connect(imageAcquisitionThread, SIGNAL(renderHistogramSignal(QList<QLineSeries*>, int)), this, SLOT(renderHistogramSlot(QList<QLineSeries*>, int)));
    restoreDeviceSpecificSettings();
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
            qDebug() << "Closing device " + imageAcquisitionThread->getDeviceName()<< ". . . ";
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
        qImage = qImage.scaled(graphicsView->width(), graphicsView->height(), fixedAspectRatio);        
        graphicsPixmapItem->setPixmap(QPixmap::fromImage(qImage));
        this->show();
    }


}

void ImageStreamWindow::saveImage()
/*
Save Image
*/
{
    try {
        auto dir = getImageSavePathForDevice(imageAcquisitionThread->getDeviceName());
        if (!qDir.exists(dir))   {
            qDebug() << "Creating Application directory at "+dir;
            qDir.mkpath(dir);
        }
        QString filename=dir+"/"+QString(generateTimeStamp().c_str())+"."+IMAGE_CONFIGURATION::IMAGEFORMAT;
        qInfo() << "Saving image at "+filename;
        imageAcquisitionThread->getImageAcquisitionHandle().GrabImageAsync(0).WriteImage(IMAGE_CONFIGURATION::IMAGEFORMAT.toStdString().c_str(), 1, filename.toStdString().c_str());
    } catch (HalconCpp::HException he) {
        qCritical() << he.ErrorMessage().Text();
    }
    catch(std::exception &e)
    {
        qCritical() << e.what();
    }
}


void ImageStreamWindow::startVideoRecord(){
    try {
        QDir dir;
        qDebug() << "Starting video record";
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
        qCritical() << e.what();
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

void ImageStreamWindow::setFixedAspectRatio()
{
    if (fixedAspectRatio == Qt::KeepAspectRatio){
        fixedAspectRatio = Qt::IgnoreAspectRatio;
        fixedAspectRatioButton->setIcon(QIcon(":icons/icon-fullscreen-disabled.png"));
        qDebug() << "Image aspect ratio set to ignore";
    }
    else{
        fixedAspectRatio = Qt::KeepAspectRatio;
        fixedAspectRatioButton->setIcon(QIcon(":icons/icon-fullscreen.png"));
        qDebug() << "Image aspect ratio set to fixed";
    }

}


void ImageStreamWindow::rotateAntiClockwise90Deg()
{
    if (imageAcquisitionThread->imageRotation+90.0 >= 360){
        imageAcquisitionThread->imageRotation = 0;
        qDebug() << "Rotation Clockwise: "+ QString::number(imageAcquisitionThread->imageRotation);
    }
    else{
        imageAcquisitionThread->imageRotation += 90;
        qDebug() << "Rotation Clockwise: "+ QString::number(imageAcquisitionThread->imageRotation);
    }
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "rotation", QString::number(imageAcquisitionThread->imageRotation));
}

void ImageStreamWindow::mirrorImageSlot()
{
    imageAcquisitionThread->mirrorImage = !imageAcquisitionThread->mirrorImage;
    qDebug() << "Mirror Image: "+QString("%1").arg(imageAcquisitionThread->mirrorImage);
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "mirror", QString("%1").arg(imageAcquisitionThread->mirrorImage));
}


void ImageStreamWindow::updateAllParameters()
{
    // query for all the parameters from hardware
    // display all the updated parameters to UI
    for(int i=0;i<containers.size();i++)
    {
        containers.at(i)->updateParamValue();
        containers.at(i)->displayParamValue();
    }

    emit updateStatusBar("Frame Rate: "+ QString::number(this->getImageAcquisitionThread()->getValueForParam(cameraParameters.RESULTINGFRAMERATE).D()));


}


void ImageStreamWindow::createHistogramWindow()
{
    histogramWindow = new HistogramWindow(grayHistogramButton, this);
    histogramWindow->setImageAcquisitionThread(imageAcquisitionThread);
    QChart *chart = new QChart();
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 255);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    histogramWindow->setCentralWidget(chartView);
    histogramWindow->resize(500, 400);
    histogramWindow->setWindowTitle(this->imageAcquisitionThread->getDeviceName());
    this->imageAcquisitionThread->setSupplyHistogramData(true);
}

void ImageStreamWindow::renderHistogramSlot(QList<QLineSeries*> frequencies, int max)
{

    chartView->chart()->removeAllSeries();
//    for(int i=0;i<frequencies.size();i++)
//    {
//        series->append(i, frequencies[i]);
//    }
//        chartView->chart()->addSeries(series);

    for(int i=0;i<frequencies.size();i++)
    {
        chartView->chart()->addSeries(frequencies.at(i));
    }
    chartView->chart()->axes(Qt::Vertical).first()->setRange(0, QVariant(max));
//    chartView->chart()->legend()->markers(series)[0]->setVisible(false);
    grayHistogramButton->setDisabled(true);
    histogramWindow->show();
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

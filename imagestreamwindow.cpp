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
#include <customscene.h>
#include <QSplitter>

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
        imageAcquisitionThread->mirrorImageHorizontal = SettingsStore::getDeviceSpecificSettings(imageAcquisitionThread->getDeviceName(),
                                                                                                 "mirrorH", QString("%1").arg(false)).toBool();
        imageAcquisitionThread->mirrorImageVertical = SettingsStore::getDeviceSpecificSettings(imageAcquisitionThread->getDeviceName(),
                                                                                               "mirrorV", QString("%1").arg(false)).toBool();
    } catch (std::exception &e) {
        qDebug() << e.what();
    }
}

void ImageStreamWindow::setupCameraWindow()
{
    // Add new ToolBar
    toolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, toolBar);

    imageSaveButton = toolBar->addAction(tr("ImageSaveButton"));
    imageSaveButton->setIcon(QIcon(":icons/icon-single-shot.png"));        
    imageSaveButton->setToolTip("Save frame");
    connect(imageSaveButton, SIGNAL(triggered()), this, SLOT(saveImage()));

    recordButton = toolBar->addAction(tr("RecordButton"));
    recordButton->setIcon(QIcon(":icons/icon-media-record.png"));
    recordButton->setToolTip("Start recording frames");
    connect(recordButton, SIGNAL(triggered()), this, SLOT(startVideoRecord()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(writeQueue()));
    timer->start(1000);

    //    recordPauseButton = this->menuBar()->addAction(tr("RecordPauseButton"));
    //    recordPauseButton->setIcon(QIcon(":icons/icon-media-playback-pause.png"));
    //    recordPauseButton->setDisabled(true);
    //    connect(recordPauseButton, SIGNAL(triggered()), this, SLOT(pauseVideoRecord()));

    recordStopButton = toolBar->addAction(tr("RecordStopButton"));
    recordStopButton->setIcon(QIcon(":icons/icon-media-playback-stop.png"));
    recordStopButton->setDisabled(true);
    recordStopButton->setToolTip("Stop recording frames");
    connect(recordStopButton, SIGNAL(triggered()), this, SLOT(stopVideoRecord()));


    //    fixedAspectRatioButton = this->menuBar()->addAction(tr("FixedAspectRatioButton"));
    //    fixedAspectRatioButton->setIcon(QIcon(":icons/icon-fullscreen.png"));
    //    fixedAspectRatioButton->setCheckable(true);
    //    connect(fixedAspectRatioButton, SIGNAL(triggered()), this, SLOT(setFixedAspectRatio()));

    // Histogram
    grayHistogramButton = toolBar->addAction(tr("GrayHistogramButton"));
    grayHistogramButton->setIcon(QIcon(":icons/icon-histogram.png"));
    grayHistogramButton->setToolTip("Open Histogram");
    connect(grayHistogramButton, SIGNAL(triggered()), this, SLOT(createHistogramWindow()));

    rotateClockwise90Button = toolBar->addAction(tr("Rotate90Button"));
    rotateClockwise90Button->setIcon(QIcon(":icons/rotate.png"));
    rotateClockwise90Button->setToolTip("Rotate anticlockwise by 90");
    connect(rotateClockwise90Button, SIGNAL(triggered()), this, SLOT(rotateAntiClockwise90Deg()));

    mirrorImageHorizontalButton = toolBar->addAction(tr("MirrorImageHorizontalButton"));
    mirrorImageHorizontalButton->setIcon(QIcon(":icons/flip-horizontal.png"));
    mirrorImageHorizontalButton->setToolTip("Flip horizontal");
    mirrorImageHorizontalButton->setCheckable(true);
    connect(mirrorImageHorizontalButton, SIGNAL(triggered()), this, SLOT(mirrorImageHorizontalSlot()));

    mirrorImageVerticalButton = toolBar->addAction(tr("MirrorImageVerticalButton"));
    mirrorImageVerticalButton->setIcon(QIcon(":icons/flip-vertical.png"));
    mirrorImageVerticalButton->setToolTip("Flip Vertical");
    mirrorImageVerticalButton->setCheckable(true);
    connect(mirrorImageVerticalButton, SIGNAL(triggered()), this, SLOT(mirrorImageVerticalSlot()));

    gridButton = toolBar->addAction(tr("GridImageButton"));
    gridButton->setIcon(QIcon(":icons/icon-grid.png"));
    gridButton->setToolTip("Enable Grid Lines");
    gridButton->setCheckable(true);
    connect(gridButton, SIGNAL(triggered()), this, SLOT(gridSlot()));

    resetImageButton = toolBar->addAction(tr("ResetImageButton"));
    resetImageButton->setIcon(QIcon(":icons/reset.png"));
    resetImageButton->setToolTip("Reset");
    connect(resetImageButton, SIGNAL(triggered()), this, SLOT(resetImageSlot()));

    selectButton =toolBar->addAction(tr("SelectButton"));
    selectButton->setIcon(QIcon(":icons/select.png"));
    selectButton->setToolTip("Select Lines");
    selectButton->setCheckable(true);
    selectButton->setData(int(CustomScene::SelectObject));
//    connect(selectButton, SIGNAL(triggered()), this, SLOT(selectSlot()));

    lineButton = toolBar->addAction(tr("LineButton"));
    lineButton->setIcon(QIcon(":icons/line.png"));
    lineButton->setToolTip("Draw Lines");
    lineButton->setCheckable(true);
    lineButton->setData(int(CustomScene::DrawLine));
//    connect(lineButton, SIGNAL(triggered()), this, SLOT(lineSlot()));

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(lineButton);
    actionGroup->addAction(selectButton);

    connect(actionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(actionGroupClicked(QAction*)));


    // Setting up graphics view
    graphicsView = new QGraphicsView();
    graphicsScene = new CustomScene(this);

    QOverload<double> qOverloadDouble;
    QOverload<int> qOverloadInt;

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    this->setCentralWidget(splitter);

    graphicsView->setScene(graphicsScene);
    graphicsView->setRenderHints(QPainter::Antialiasing);
    graphicsPixmapItem = new QGraphicsPixmapItem();
    graphicsView->scene()->addItem(graphicsPixmapItem);

    QTreeWidget *ccTreeWidget = new QTreeWidget;
    ccTreeWidget->setColumnCount(2);
    ccTreeWidget->setHeaderLabels(QStringList() <<"Feature"<<"Value");

    // Set column width for ccTree (Dynamic resize only for image)
    ccTreeWidget->setFixedWidth(350);
    ccTreeWidget->setColumnWidth(0, 250);

    // Set the two UI elements on the camera acquisition screen
    splitter->addWidget(ccTreeWidget);
    splitter->addWidget(graphicsView);


    QList<QTreeWidgetItem *> topLevelItems;

    // exposure controls
    // parent
    exposureControls = new QTreeWidgetItem();
    exposureControls->setText(0, "Exposure Controls");
    topLevelItems.append(exposureControls);

    // child
    // EXPOSURE
    QHBoxLayout * exposureLayout = new QHBoxLayout(splitter);
    QWidget *exposureWidget = new QWidget;
    exposureWidget->setLayout(exposureLayout);
    exposureLayout->setSpacing(0);
    exposureLayout->setMargin(0);
    SpinboxContainer *exposureContainer = new SpinboxContainer(3000,cameraParameters.EXPOSURETIME,cameraParameters.EXPOSURETIME,0,1000000,50,imageAcquisitionThread);
    addToContainer(exposureContainer,exposureContainer->getUiElement()->isEnabled());
    exposureLayout->addWidget(exposureContainer->getUiElement());
    exposureLayout->addWidget(exposureContainer->getIntSlider());
    exposureControls->addChild(exposureContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(exposureContainer->getQTreeWidgetItem(), 1, exposureWidget);


    // ANALOG GAIN
    QHBoxLayout * gainLayout = new QHBoxLayout(splitter);
    QWidget *gainWidget = new QWidget;
    gainWidget->setLayout(gainLayout);
    DoubleSpinboxContainer *analogGainContainer = new DoubleSpinboxContainer(5,cameraParameters.GAIN,cameraParameters.GAIN,0,36,0.1,10,imageAcquisitionThread);
    addToContainer(analogGainContainer,analogGainContainer->getUiElement()->isEnabled());
    gainLayout->addWidget(analogGainContainer->getUiElement());
    gainLayout->addWidget(analogGainContainer->getDoubleSlider());
    exposureControls->addChild(analogGainContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(analogGainContainer->getQTreeWidgetItem(), 1, gainWidget);

    // AUTO GAIN
    QHBoxLayout * autoGainLayout = new QHBoxLayout(splitter);
    QWidget *autoGainWidget = new QWidget;
    autoGainWidget->setLayout(autoGainLayout);
    CheckboxContainer *autoGainContainer = new CheckboxContainer(false,cameraParameters.AUTOGAIN,cameraParameters.AUTOGAIN,"Once","Off",imageAcquisitionThread);
    addToContainer(autoGainContainer,autoGainContainer->getUiElement()->isEnabled());
    autoGainLayout->addWidget(autoGainContainer->getUiElement());
    exposureControls->addChild(autoGainContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(autoGainContainer->getQTreeWidgetItem(), 1, autoGainWidget);

    // AUTO EXPOSURE
    QHBoxLayout * autoExposureLayout = new QHBoxLayout(splitter);
    QWidget *autoExposureWidget = new QWidget;
    autoExposureWidget->setLayout(autoExposureLayout);
    CheckboxContainer *autoExposureContainer = new CheckboxContainer(false,cameraParameters.AUTOEXPOSURE,cameraParameters.AUTOEXPOSURE,"Once","Off",imageAcquisitionThread);
    addToContainer(autoExposureContainer,autoExposureContainer->getUiElement()->isEnabled());
    autoExposureLayout->addWidget(autoExposureContainer->getUiElement());
    exposureControls->addChild(autoExposureContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(autoExposureContainer->getQTreeWidgetItem(), 1, autoExposureWidget);

    // color appearance parameters
    // parent
    colorAppearance = new QTreeWidgetItem();
    colorAppearance->setExpanded(true);
    colorAppearance->setText(0, "Color and Appearance");
    topLevelItems.append(colorAppearance);


    // child
    // HUE
    QHBoxLayout * hueLayout = new QHBoxLayout(splitter);
    QWidget *hueWidget = new QWidget;
    hueWidget->setLayout(hueLayout);
    SpinboxContainer *hueContainer = new SpinboxContainer(0,cameraParameters.HUE, cameraParameters.HUE,-40,40,1, imageAcquisitionThread);
    addToContainer(hueContainer,hueContainer->getUiElement()->isEnabled());
    hueLayout->addWidget(hueContainer->getUiElement());
    hueLayout->addWidget(hueContainer->getIntSlider());
    colorAppearance->addChild(hueContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(hueContainer->getQTreeWidgetItem(), 1, hueWidget);

    // SATURATION
    QHBoxLayout * saturationLayout = new QHBoxLayout(splitter);
    QWidget *saturationWidget = new QWidget;
    saturationWidget->setLayout(saturationLayout);
    DoubleSpinboxContainer *saturationContainer = new DoubleSpinboxContainer(0,cameraParameters.SATURATION,cameraParameters.SATURATION,0,2,1, 10, imageAcquisitionThread);
    addToContainer(saturationContainer,saturationContainer->getUiElement()->isEnabled());
    saturationLayout->addWidget(saturationContainer->getUiElement());
    saturationLayout->addWidget(saturationContainer->getDoubleSlider());
    colorAppearance->addChild(saturationContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(saturationContainer->getQTreeWidgetItem(), 1, saturationWidget);


    // BRIGHTNESS
    QHBoxLayout * brightnessLayout = new QHBoxLayout(splitter);
    QWidget *brightnessWidget = new QWidget;
    brightnessWidget->setLayout(brightnessLayout);
    SpinboxContainer *brightnessContainer = new SpinboxContainer(0,cameraParameters.BRIGHTNESS, cameraParameters.BRIGHTNESS,0,100,1, imageAcquisitionThread);
    addToContainer(brightnessContainer,brightnessContainer->getUiElement()->isEnabled());
    brightnessLayout->addWidget(brightnessContainer->getUiElement());
    brightnessLayout->addWidget(brightnessContainer->getIntSlider());
    colorAppearance->addChild(brightnessContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(brightnessContainer->getQTreeWidgetItem(), 1, brightnessWidget);

    // CONTRAST
    QHBoxLayout * contrastLayout = new QHBoxLayout(splitter);
    QWidget *contrastWidget = new QWidget;
    contrastWidget->setLayout(contrastLayout);
    SpinboxContainer *contrastContainer = new SpinboxContainer(0,cameraParameters.CONTRAST,cameraParameters.CONTRAST,0,100,1, imageAcquisitionThread);
    addToContainer(contrastContainer,contrastContainer->getUiElement()->isEnabled());
    contrastLayout->addWidget(contrastContainer->getUiElement());
    contrastLayout->addWidget(contrastContainer->getIntSlider());
    colorAppearance->addChild(contrastContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(contrastContainer->getQTreeWidgetItem(), 1, contrastWidget);


    // GAMMA
    QHBoxLayout * gammaLayout = new QHBoxLayout(splitter);
    QWidget *gammaWidget = new QWidget;
    gammaWidget->setLayout(gammaLayout);
    DoubleSpinboxContainer *gammaContainer = new DoubleSpinboxContainer(0,cameraParameters.GAMMA,cameraParameters.GAMMA,0,4,0.1,10,imageAcquisitionThread);
    addToContainer(gammaContainer,gammaContainer->getUiElement()->isEnabled());
    gammaLayout->addWidget(gammaContainer->getUiElement());
    gammaLayout->addWidget(gammaContainer->getDoubleSlider());
    colorAppearance->addChild(gammaContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(gammaContainer->getQTreeWidgetItem(), 1, gammaWidget);




    // ACQUISITION FRAME RATE ENABLER
    QHBoxLayout * acquisitionFrameRateEnableLayout = new QHBoxLayout(splitter);
    QWidget *acquisitionFrameRateEnableWidget = new QWidget;
    acquisitionFrameRateEnableWidget->setLayout(acquisitionFrameRateEnableLayout);
    CheckboxContainer *acquisitionFrameRateEnableContainer = new CheckboxContainer(false,cameraParameters.ACQUISITIONFRAMERATEENABLE,
                                                                                   cameraParameters.ACQUISITIONFRAMERATEENABLE,"","",imageAcquisitionThread);
    addToContainer(acquisitionFrameRateEnableContainer,acquisitionFrameRateEnableContainer->getUiElement()->isEnabled());
    acquisitionFrameRateEnableLayout->addWidget(acquisitionFrameRateEnableContainer->getUiElement());
    colorAppearance->addChild(acquisitionFrameRateEnableContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(acquisitionFrameRateEnableContainer->getQTreeWidgetItem(), 1, acquisitionFrameRateEnableWidget);



    // ACQUISITION FRAME RATE
    QHBoxLayout * acquisitionFrameRateLayout = new QHBoxLayout(splitter);
    QWidget *acquisitionFrameRateWidget = new QWidget;
    acquisitionFrameRateWidget->setLayout(acquisitionFrameRateLayout);
    SpinboxContainer *acquisitionFrameRateContainer = new SpinboxContainer(10,cameraParameters.ACQUISITIONFRAMERATE,cameraParameters.ACQUISITIONFRAMERATE,1,120,1, imageAcquisitionThread);
    addToContainer(acquisitionFrameRateContainer,acquisitionFrameRateContainer->getUiElement()->isEnabled());
    acquisitionFrameRateLayout->addWidget(acquisitionFrameRateContainer->getUiElement());
    acquisitionFrameRateLayout->addWidget(acquisitionFrameRateContainer->getIntSlider());
    colorAppearance->addChild(acquisitionFrameRateContainer->getQTreeWidgetItem());
    ccTreeWidget->setItemWidget(acquisitionFrameRateContainer->getQTreeWidgetItem(), 1, acquisitionFrameRateWidget);


//    // MONOCHROME
//    CheckboxContainer *monochromeContainer = new CheckboxContainer(false,cameraParameters.MONOCHROME,
//                                                                   cameraParameters.MONOCHROME,"","",imageAcquisitionThread);
//    addToContainer(monochromeContainer,monochromeContainer->getUiElement()->isEnabled());
//    colorAppearance->addChild(monochromeContainer->getQTreeWidgetItem());
//    ccTreeWidget->setItemWidget(monochromeContainer->getQTreeWidgetItem(), 1, monochromeContainer->getUiElement());


//    // RGB
//    CheckboxContainer *rgbContainer = new CheckboxContainer(false,cameraParameters.RGB,
//                                                            cameraParameters.RGB,"","",imageAcquisitionThread);
//    addToContainer(rgbContainer,rgbContainer->getUiElement()->isEnabled());
//    colorAppearance->addChild(rgbContainer->getQTreeWidgetItem());
//    ccTreeWidget->setItemWidget(rgbContainer->getQTreeWidgetItem(), 1, rgbContainer->getUiElement());

    ccTreeWidget->addTopLevelItems(topLevelItems);
    exposureControls->setExpanded(true);
    colorAppearance->setExpanded(true);

    emit updateStatusBar("Frame Rate: "+ QString::number(this->getImageAcquisitionThread()->getValueForParam(cameraParameters.RESULTINGFRAMERATE).D()));
    // resulting frame rate in status bar


    // TODO: THis is a temp fix. The below line of code currently has to be written for every parameter addition. Going forward find a level of abstraction for it.
    connect(exposureContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(analogGainContainer->getUiElement(), SIGNAL(valueChanged(double)),this,SLOT(updateAllParameters()));
    connect(autoGainContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    connect(autoExposureContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    connect(saturationContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(hueContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(brightnessContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(contrastContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));
    connect(acquisitionFrameRateEnableContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    connect(gammaContainer->getUiElement(), SIGNAL(valueChanged(double)),this,SLOT(updateAllParameters()));
    connect(acquisitionFrameRateContainer->getUiElement(), SIGNAL(valueChanged(int)),this,SLOT(updateAllParameters()));

    //    connect(monochromeContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));
    //    connect(rgbContainer->getUiElement(), SIGNAL(clicked(bool)),this,SLOT(updateAllParameters()));

    connect(imageAcquisitionThread, SIGNAL(renderHistogramSignal(QList<QLineSeries*>, int)), this, SLOT(renderHistogramSlot(QList<QLineSeries*>, int)));
    restoreDeviceSpecificSettings();
//    createToolBar();
}

void ImageStreamWindow::createToolBar(){
    toolBar = new QToolBar;
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->addAction(imageSaveButton);
    toolBar->addAction(recordButton);
    toolBar->addAction(recordStopButton);
    toolBar->addAction(grayHistogramButton);
    toolBar->addAction(rotateClockwise90Button);
    toolBar->addAction(mirrorImageVerticalButton);
    toolBar->addAction(mirrorImageHorizontalButton);
    toolBar->addAction(gridButton);
    toolBar->addAction(selectButton);
    toolBar->addAction(lineButton);
//    toolBar->addAction(lineAction);
}

void ImageStreamWindow::addToContainer(ParameterContainer *container, bool exists)
{
    if(exists)
        containers.append(container);
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
        imageAcquisitionThread->dynamicWidth = graphicsView->width();
        imageAcquisitionThread->dynamicHeight = graphicsView->height();
        //        qImage = qImage.scaled(graphicsView->width(), graphicsView->height(), fixedAspectRatio);
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
        QString filename=dir+"/"+QString(generateTimeStamp().c_str())+"."+IMAGE_CONFIGURATION::IMAGE_FORMAT;
        qInfo() << "Saving image at "+filename;
        imageAcquisitionThread->getImageAcquisitionHandle().GrabImageAsync(0).WriteImage(IMAGE_CONFIGURATION::IMAGE_FORMAT.toStdString().c_str(), 1, filename.toStdString().c_str());
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

void ImageStreamWindow::mirrorImageHorizontalSlot()
{
    imageAcquisitionThread->mirrorImageHorizontal = !imageAcquisitionThread->mirrorImageHorizontal;
    qDebug() << "Mirror Image: "+QString("%1").arg(imageAcquisitionThread->mirrorImageHorizontal);
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "mirrorH", QString("%1").arg(imageAcquisitionThread->mirrorImageHorizontal));
}

void ImageStreamWindow::mirrorImageVerticalSlot()
{
    imageAcquisitionThread->mirrorImageVertical = !imageAcquisitionThread->mirrorImageVertical;
    qDebug() << "Mirror Image: "+QString("%1").arg(imageAcquisitionThread->mirrorImageVertical);
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "mirrorV", QString("%1").arg(imageAcquisitionThread->mirrorImageVertical));
}

void ImageStreamWindow::gridSlot()
{
    imageAcquisitionThread->enableGrid = !imageAcquisitionThread->enableGrid;
    qDebug() << "Grid:  "+QString("%1").arg(imageAcquisitionThread->enableGrid);
}


void ImageStreamWindow::resetImageSlot()
{
    qDebug() << "Reset called on device: " + imageAcquisitionThread->getDeviceName();
    imageAcquisitionThread->mirrorImageHorizontal = false;
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "mirrorH", QString("%1").arg(imageAcquisitionThread->mirrorImageHorizontal));
    qDebug() << "Reset Horizontal Mirror Image:  "+QString("%1").arg(imageAcquisitionThread->mirrorImageHorizontal);
    imageAcquisitionThread->mirrorImageVertical = false;
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "mirrorV", QString("%1").arg(imageAcquisitionThread->mirrorImageVertical));
    qDebug() << "Reset Vertical Mirror Image:  "+QString("%1").arg(imageAcquisitionThread->mirrorImageVertical);
    imageAcquisitionThread->imageRotation = 0.0;
    SettingsStore::addDeviceSpecificSetting(imageAcquisitionThread->getDeviceName(), "rotation", QString::number(imageAcquisitionThread->imageRotation));
    qDebug() << "Reset Image Rotation:  "+QString("%1").arg(imageAcquisitionThread->imageRotation);
    imageAcquisitionThread->enableGrid = false;
    qDebug() << "Reset Grid:  "+QString("%1").arg(imageAcquisitionThread->enableGrid);
}

void ImageStreamWindow::selectSlot(){
    qDebug() << "Select Clicked";
}

void ImageStreamWindow::lineSlot(){
    qDebug() << "Line Clicked";
}

void ImageStreamWindow::actionGroupClicked(QAction *action){
    qDebug() << "In here";
    graphicsScene->setMode(CustomScene::Mode(action->data().toInt()));
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

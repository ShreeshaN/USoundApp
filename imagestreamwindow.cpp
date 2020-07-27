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
//#include<QToolTip>


//class Menu : public QMenu
//{
//    Q_OBJECT
//public:
//    Menu(){}
//    bool event (QEvent * e)
//    {
//        const QHelpEvent *helpEvent = static_cast <QHelpEvent *>(e);
//         if (helpEvent->type() == QEvent::ToolTip && activeAction() != 0)
//         {
//              QToolTip::showText(helpEvent->globalPos(), activeAction()->toolTip());
//         } else
//         {
//              QToolTip::hideText();
//         }
//         return QMenu::event(e);
//    }
//};


ImageStreamWindow::ImageStreamWindow(QWidget *parent) : QMainWindow(parent)
{
}

void ImageStreamWindow::setupCameraWindow()
{
    QAction *imageSaveButton = this->menuBar()->addAction(tr("ImageSaveButton"));
    QPixmap pixmap("icons/icon-single-shot.png");
    imageSaveButton->setIcon(QIcon(":icons/icon-single-shot.png"));
    // todo: Prathyush SP -> Fix issue with tooltip display
    imageSaveButton->setToolTip("Save frame");
    connect(imageSaveButton, SIGNAL(triggered()), this, SLOT(saveImage()));
    // Example ends


    QOverload<int> qOverload;
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
    connect(exposureTimeSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setExposureTime(exposureTimeSpinBox->value());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::EXPOSURETIME,exposureTimeSpinBox->value());
        updateCameraParametersAndDisplay();
    });

    analogGain = new QTreeWidgetItem(QStringList() << "Analog Gain");
    analogGainSpinBox = new QSpinBox;
    analogGainSpinBox->setRange(0, 100);
    analogGainSpinBox->setSingleStep(1);
    analogGainSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getAnalogGain());
    connect(analogGainSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
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
        updateCameraParametersAndDisplay();
        qDebug() << "Auto exposure "<< this->imageAcquisitionThread->getCameraControls().getAutoExposure();

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
    connect(hueSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
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
    connect(saturationSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
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
    connect(brightnessSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
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
    connect(contrastSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setContrast(contrastSpinBox->value());
        // imageAcquisitionThread->setValueForParam(HalconCameraParameters::CONTRAST,contrastSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(contrast, 1, contrastSpinBox);

    gamma = new QTreeWidgetItem(QStringList() << "Gamma");
    QSpinBox *gammaSpinBox = new QSpinBox;
    gammaSpinBox->setRange(0, 100);
    gammaSpinBox->setSingleStep(1);
    gammaSpinBox->setValue( this->imageAcquisitionThread->getCameraControls().getGamma());
    colorAppearance->addChild(gamma);
    connect(gammaSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setGamma(gammaSpinBox->value());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::GAMMA,gammaSpinBox->value());
        updateCameraParametersAndDisplay();
    });
    ccTreeWidget->setItemWidget(gamma, 1, gammaSpinBox);

    acquisitionFrameRate = new QTreeWidgetItem(QStringList() << " Acquition Frame Rate");
    acquisitionFramerateSpinBox = new QSpinBox;
    acquisitionFramerateSpinBox->setRange(0, 120);
    acquisitionFramerateSpinBox->setSingleStep(1);
    acquisitionFramerateSpinBox->setValue(this->imageAcquisitionThread->getCameraControls().getAcquisitionFrameRate());
    colorAppearance->addChild(acquisitionFrameRate);
    connect(acquisitionFramerateSpinBox, qOverload(&QSpinBox::valueChanged), [=]{
        this->imageAcquisitionThread->getCameraControls().setAcquisitionFrameRate(acquisitionFramerateSpinBox->value());
        imageAcquisitionThread->setValueForParam(HalconCameraParameterNames::ACQUISITIONFRAMERATE,acquisitionFramerateSpinBox->value());
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
    monochromeCheckbox->setCheckState(imageAcquisitionThread->getCameraControls().getMonochrome()?Qt::CheckState(2):Qt::CheckState(0));
    rgbCheckbox->setCheckState(imageAcquisitionThread->getCameraControls().getRgb()?Qt::CheckState(2):Qt::CheckState(0));
}

void ImageStreamWindow::closeEvent(QCloseEvent *event)
{
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
    qDebug() << "COunter valalue: "<<imageAcquisitionThread->getCounter();



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
    this->statusBar()->showMessage("Frame Rate: "+ QString::number(this->getImageAcquisitionThread()->getCameraControls().getResultingFrameRate()));



}

void ImageStreamWindow::saveImage()
/*
Save Image
*/
{
    try {

        //todo: Prathyush SP -> Change from tempPath to user set path
        std::string filename=QDir::tempPath().toStdString()+"/"+imageAcquisitionThread->getDeviceName().toStdString();
        filename = filename+"-"+generateTimeStamp()+".jpeg";
        qInfo() << ("Saving image at "+filename).c_str();
        imageAcquisitionThread->currentImage.WriteImage("jpeg", 0,filename.c_str());
    } catch (HalconCpp::HException he) {
        qDebug() << he.ErrorMessage().Text();
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

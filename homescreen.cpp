#include "homescreen.h"
#include "ui_homescreen.h"
#include "cameracontrols.h"
#include "imageacquisition.h"

#include <HalconCpp.h>
#include <Halcon.h>
#include "HFramegrabber.h"
#include <HFramegrabber.h>

#include <QDebug>
#include <QPushButton>
#include <QTreeWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStandardItem>
#include <QAction>
#include <QDesktopWidget>
#include <QException>
#include <exception>
#include<QDialog>


Homescreen::Homescreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Homescreen)

{
    ui->setupUi(this);
    onApplicationStartup();

}

CameraControls Homescreen::getDeviceInSelection(){
    return cameraControlsMap["camera_1"];
}

void Homescreen::showCameraControlsUI(QString deviceName){

    ui->cameraControlsTreeWidget->clear();

    QVBoxLayout* mainLayout = new QVBoxLayout();
    QList<QTreeWidgetItem *> topLevelItems;
    CameraControls selectedCameraControls;
    if(QString::compare(deviceName, "")!=0)
    {
        selectedCameraControls = cameraControlsMap[deviceName];
    }

    // exposure controls
    // parent
    exposureControls = new QTreeWidgetItem();
    exposureControls->setText(0, "Exposure Controls");
    topLevelItems.append(exposureControls);

    // child
    exposureTime = new QTreeWidgetItem(QStringList() << "Exposure Time");
    QSpinBox *exposureTimeSpinBox = new QSpinBox;
    exposureTimeSpinBox->setRange(0, 100);
    exposureTimeSpinBox->setSingleStep(1);
    exposureTimeSpinBox->setValue(selectedCameraControls.getExposureTime());
    connect(exposureTimeSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setExposureTime(exposureTimeSpinBox->value());
    });

    analogGain = new QTreeWidgetItem(QStringList() << "Analog Gain");
    QSpinBox *analogGainSpinBox = new QSpinBox;
    analogGainSpinBox->setRange(0, 100);
    analogGainSpinBox->setSingleStep(1);
    analogGainSpinBox->setValue(selectedCameraControls.getAnalogGain());
    connect(analogGainSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setAnalogGain(analogGainSpinBox->value());
    });

    auto autoExposure = new QTreeWidgetItem(QStringList() << "Auto Exposure");
    QCheckBox *autoExposureCheckbox = new QCheckBox();
    autoExposureCheckbox->setCheckState(Qt::Checked);
    connect(autoExposureCheckbox, &QCheckBox::clicked, [=]{
        cameraControlsMap[cameraInSelection].setAutoExposure(autoExposureCheckbox->isChecked());
    });

    exposureControls->addChild(exposureTime);
    exposureControls->addChild(analogGain);
    exposureControls->addChild(autoExposure);
    ui->cameraControlsTreeWidget->setItemWidget(exposureTime, 1, exposureTimeSpinBox);
    ui->cameraControlsTreeWidget->setItemWidget(analogGain, 1, analogGainSpinBox);
    ui->cameraControlsTreeWidget->setItemWidget(autoExposure, 1, autoExposureCheckbox);

    // color appearance parameters
    // parent
    colorAppearance = new QTreeWidgetItem();
    colorAppearance->setExpanded(true);
    colorAppearance->setText(0, "Color and Appearance");
    topLevelItems.append(colorAppearance);


    // child    
    hue = new QTreeWidgetItem(QStringList() << "Hue");
    QSpinBox *hueSpinBox = new QSpinBox(this);
    hueSpinBox->setRange(0, 100);
    hueSpinBox->setSingleStep(1);
    hueSpinBox->setValue(selectedCameraControls.getHue());
    colorAppearance->addChild(hue);
    connect(hueSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setHue(hueSpinBox->value());
    });
    ui->cameraControlsTreeWidget->setItemWidget(hue, 1, hueSpinBox);


    saturation = new QTreeWidgetItem(QStringList() << "Saturation");
    QSpinBox *saturationSpinBox = new QSpinBox;
    saturationSpinBox->setRange(0, 100);
    saturationSpinBox->setSingleStep(1);
    saturationSpinBox->setValue(selectedCameraControls.getSaturation());
    colorAppearance->addChild(saturation);
    connect(saturationSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setSaturation(saturationSpinBox->value());
    });
    ui->cameraControlsTreeWidget->setItemWidget(saturation, 1, saturationSpinBox);


    auto brightness = new QTreeWidgetItem(QStringList() << "Brightness");
    QSpinBox *brightnessSpinBox = new QSpinBox;
    brightnessSpinBox->setRange(0, 100);
    brightnessSpinBox->setSingleStep(1);
    brightnessSpinBox->setValue(selectedCameraControls.getBrightness());
    colorAppearance->addChild(brightness);
    connect(brightnessSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setBrightness(brightnessSpinBox->value());
    });
    ui->cameraControlsTreeWidget->setItemWidget(brightness, 1, brightnessSpinBox);

    contrast = new QTreeWidgetItem(QStringList() << "Contrast");
    QSpinBox *contrastSpinBox = new QSpinBox;
    contrastSpinBox->setRange(0, 100);
    contrastSpinBox->setSingleStep(1);
    contrastSpinBox->setValue(selectedCameraControls.getContrast());
    colorAppearance->addChild(contrast);
    connect(contrastSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setContrast(contrastSpinBox->value());
    });
    ui->cameraControlsTreeWidget->setItemWidget(contrast, 1, contrastSpinBox);

    gamma = new QTreeWidgetItem(QStringList() << "Gamma");
    QSpinBox *gammaSpinBox = new QSpinBox;
    gammaSpinBox->setRange(0, 100);
    gammaSpinBox->setSingleStep(1);
    gammaSpinBox->setValue(selectedCameraControls.getGamma());
    colorAppearance->addChild(gamma);
    connect(gammaSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setGamma(gammaSpinBox->value());
    });
    ui->cameraControlsTreeWidget->setItemWidget(gamma, 1, gammaSpinBox);

    framerate = new QTreeWidgetItem(QStringList() << "Frame Rate");
    QSpinBox *framerateSpinBox = new QSpinBox;
    framerateSpinBox->setRange(0, 100);
    framerateSpinBox->setSingleStep(1);
    framerateSpinBox->setValue(selectedCameraControls.getFrameRate());
    colorAppearance->addChild(framerate);
    connect(framerateSpinBox, qOverload<int>(&QSpinBox::valueChanged), [=]{
        cameraControlsMap[cameraInSelection].setFrameRate(framerateSpinBox->value());
    });
    ui->cameraControlsTreeWidget->setItemWidget(framerate, 1, framerateSpinBox);



    monochrome = new QTreeWidgetItem(QStringList() << "Monochrome");
    QCheckBox *monochromeCheckbox = new QCheckBox();
    monochromeCheckbox->setTristate(false);

    monochromeCheckbox->setCheckState(selectedCameraControls.getMonochrome()?Qt::CheckState(2):Qt::CheckState(0));
    colorAppearance->addChild(monochrome);
    connect(monochromeCheckbox, &QCheckBox::clicked, [=]{
        cameraControlsMap[cameraInSelection].setMonochrome(monochromeCheckbox->isChecked());
    });
    ui->cameraControlsTreeWidget->setItemWidget(monochrome, 1, monochromeCheckbox);

    rgb = new QTreeWidgetItem(QStringList() << "RGB");
    QCheckBox *rgbCheckbox = new QCheckBox();
    rgbCheckbox->setTristate(false);
    rgbCheckbox->setCheckState(selectedCameraControls.getRgb()?Qt::CheckState(2):Qt::CheckState(0));
    colorAppearance->addChild(rgb);
    connect(rgbCheckbox, &QCheckBox::clicked, [=]{
        cameraControlsMap[cameraInSelection].setRgb(rgbCheckbox->isChecked());
    });
    ui->cameraControlsTreeWidget->setItemWidget(rgb, 1, rgbCheckbox);

    ui->cameraControlsTreeWidget->addTopLevelItems(topLevelItems);
    ui->cameraControlsTreeWidget->setLayout(mainLayout);
    exposureControls->setExpanded(true);
    colorAppearance->setExpanded(true);

}

void Homescreen::updateMapValue(int value)
{
    qDebug() << "THe value : " << value;
    cameraControlsMap[cameraInSelection].setHue(value);

}

void Homescreen::setupDevicesUI()
{

    QList<QTreeWidgetItem *> topLevelItems;
    QListIterator<QString> devicesIterator(devices);


    // USB
    // parent
    auto usb3 = new QTreeWidgetItem();
    usb3->setText(0, "USB3");
    topLevelItems.append(usb3);

    while(devicesIterator.hasNext())
    {

        // child
        auto * usbCamera = new QTreeWidgetItem(QStringList() << devicesIterator.next());

        usb3->addChild(usbCamera);
    }
    ui->devicesTreeWidget->addTopLevelItems(topLevelItems);
    ui->devicesTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->devicesTreeWidget,&QTreeWidget::customContextMenuRequested,this, &Homescreen::prepareMenu);
}

void Homescreen::prepareMenu(const QPoint &pos){
    QTreeWidget *tree = ui->devicesTreeWidget;

    QTreeWidgetItem *nd = tree->itemAt( pos );

    qDebug()<<"Items : "<<pos<<nd->text(0);
    QString rightClickedOn = nd->text(0);
    if(devices.contains(rightClickedOn))
    {
        QAction *newAct = new QAction(QIcon(""), tr("&Connect"), this);
        connect(newAct, &QAction::triggered, this, [=](){connectToCamera(rightClickedOn);});   // SLOT(&connectToCamera(rightClickedOn)));
        QMenu menu(this);
        menu.addAction(newAct);
        menu.exec(tree->mapToGlobal(pos));
    }


}

void Homescreen::detectAttachedDevices()
{

    using namespace HalconCpp;
    qDebug() << "Detecting attached devices";
    HalconCpp::HTuple *information = new HalconCpp::HTuple;
    HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
    try {
        HalconCpp::InfoFramegrabber("usb3vision","device",information, valueList);
        qDebug() << "Devices: "<<valueList->Length();
        auto deviceList = valueList->ToSArr();
        for(int i=0;i<valueList->Length();i++)
        {
            // Eg: Value list  (" ", " device:2676016419A3_Basler_acA2040120um ",
            // " unique_name:2676016419A3_Basler_acA2040120um ",
            // " interface:Usan_VirtualIF ", " producer:Usan")
            auto device = QString(deviceList[i].Text()).split("|")[1].split(":")[1].trimmed();
            ui->messagesListWidget->addItem("New camera detected : "+ device);
//            emit pushToMessageBoxSignal("New camera detected : "+ device);
//            connect(this, SIGNAL(pushToMessageBoxSignal(QString)),this,SLOT(pushToMessageBoxSlot(QString)));

            devices.append(device);
        }

    }  catch (HException &except)
    {
        qDebug() << "Device unavailable";
        qDebug() << except.ErrorCode();
        qDebug() << except.ErrorMessage().Text();

    }

}


Homescreen::~Homescreen()
{
    delete ui;
}



void Homescreen::onApplicationStartup()
{
    detectAttachedDevices();
    setupDevicesUI();
    setupCameraControls();
    showCameraControlsUI("");


}

void Homescreen::setupCameraControls()
{
    int numberOfCameras = devices.count();
    for(int i=0;i<numberOfCameras;i++){
        CameraControlDefaults cameraControlsDefaults;
        CameraControls cameraControlSettings(cameraControlsDefaults);
        cameraControlsMap[devices.at(i)] = cameraControlSettings;
    }
}


void Homescreen::on_devicesRefresh_clicked()
{

//    HalconCpp::HFramegrabber acq("USB3Vision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "267601642BB5_Basler_acA2040120um", 0, -1);
//    qDebug() <<"Params: ";
//    HalconCpp::HTuple h = acq.GetFramegrabberParam("Gain");

//    qDebug() << h.ToString().Text() ;
//    acq.SetFramegrabberParam("Gain",10);


//    using namespace HalconCpp;
//    HalconCpp::HTuple *information = new HalconCpp::HTuple;
//    HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
//    QList<QString> list = {"bits_per_channel", "camera_type", "color_space","external_trigger","generic","info_boards","line_in","parameters","parameters_readonly","parameters_writeonly","port","revision"};
//    for (auto param : list) {
//        qDebug()<< "************" << param << "*************";
//        HalconCpp::InfoFramegrabber("usb3vision",HTuple(HString::FromUtf8(param.toUtf8())),information, valueList);
//        qDebug () << "Information: "<< information->S().Text();
//        qDebug() << "Valuelist length: "<<valueList->Length();
//        auto deviceList = valueList->ToSArr();
//        for(int i=0;i<valueList->Length();i++)
//        {
//            qDebug() << QString(deviceList[i].Text());
//        }

//    }







    //    HalconCpp::HFramegrabber acq("USB3Vision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "267601642BB5_Basler_acA2040120um", 0, -1);
//    HalconCpp::HImage image = acq.GrabImage();
//    Hlong  width,height;
//      image.GetImageSize(&width,&height);

//      qDebug() << "Image size "<< width << height;
//      HalconCpp::WriteImage(image,"tiff",0,"C:/Users/daruizcadalso/Documents/QTApplications/USoundApp/sample.jpg");
//      HalconCpp::CloseFramegrabber(acq);



}

void Homescreen::on_devicesTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    cameraInSelection = item->text(0);
    if(devices.contains(cameraInSelection))
    {
        showCameraControlsUI(cameraInSelection);
        qDebug() << "Item clicked " << cameraInSelection << (new QSpinBox(ui->cameraControlsTreeWidget->itemWidget(hue,column)))->value();

    }

}



void Homescreen::connectToCamera(QString deviceName)
{
    try {
        ImageAcquisition *imgAcq = new ImageAcquisition(this);
        /*HalconCpp::HFramegrabber acq = */HalconCpp::HFramegrabber acq("USB3Vision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", HalconCpp::HString(deviceName.toLocal8Bit().constData()).Text(), 0, -1);
        imgAcq->setAcq(acq);
        imgAcq->setDeviceName(deviceName);
        windowWidget = new ImageStreamWindow();
        windowWidget->imageAcquisitionThread = imgAcq;
        windowWidget->resize(600,600);
        QVBoxLayout * layout2 = new QVBoxLayout;
        windowWidget->setLayout(layout2);
        imageLabel = new QLabel(windowWidget);
        imageLabel->setContentsMargins(0,0,0,0);

        QFont font = imageLabel->font();
        font.setPointSize(72);
        font.setBold(true);
        imageLabel->setFont(font);

        newCameraConnectionLabels.append(imageLabel);
        newCameraConnectionWindows.append(windowWidget);
        imgAcq->setWindowIndex(newCameraConnectionWindows.length()-1);
        connect(imgAcq,SIGNAL(renderImageSignal(QImage, int)),this,SLOT(renderImage(QImage, int)));

        imgAcq->start();


    } catch (QException &e) {
        qDebug () <<"Exception";
        qDebug() <<e.what();
    }

}


void Homescreen::on_devicesTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString doubleClickedOn = item->text(0);
    if(devices.contains(doubleClickedOn))
    {
        emit connectToCamera(doubleClickedOn);
    }

}

void Homescreen::renderImage(QImage qImage, int widgetIndex)
{

    windowWidget = newCameraConnectionWindows.at(widgetIndex);
    if(!windowWidget->imageAcquisitionThread->stop)
    {
        imageLabel = newCameraConnectionLabels.at(widgetIndex);
        int height=windowWidget->height(); int width = windowWidget->width();
        imageLabel->setPixmap(QPixmap::fromImage(qImage));
        imageLabel->setScaledContents(true);
        imageLabel->resize(width, height);
        windowWidget->show();
    }


//    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(qImage));
//    QPixmap pixmap(qImage);
//    scene.addItem(item);
}

void Homescreen::onCameraWindowClose()
{
//    windowWidget->close();
    qDebug() <<"In close";
}

void Homescreen::pushToMessageBoxSlot(QString message)
{
    qDebug() << "Illige barthane ila bidu";
    ui->messagesListWidget->addItem(message);
}


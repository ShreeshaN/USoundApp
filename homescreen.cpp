#include "homescreen.h"
#include "ui_homescreen.h"
#include "cameracontrols.h"
#include "imageacquisition.h"

#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "Halcon.h"
#  include "HFramegrabber.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif

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
//#include "qteditorfactory.h"
//#include "qttreepropertybrowser.h"
//#include "qtpropertymanager.h"
//#include "qtvariantproperty.h"

Homescreen::Homescreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Homescreen)

{
    ui->setupUi(this);
    onApplicationStartup();

}


void Homescreen::setupDevicesUI()
{

    QList<QTreeWidgetItem *> topLevelItems;
    QListIterator<QString> devicesIterator(devices);
    QVBoxLayout* mainLayout = new QVBoxLayout();


    // USB
    // parent
    auto usb3 = new QTreeWidgetItem();
    usb3->setText(0, "USB3");
    topLevelItems.append(usb3);

    while(devicesIterator.hasNext())
    {

        // child
        auto * usbCamera = new QTreeWidgetItem(QStringList() << devicesIterator.next());
        usbCamera->setToolTip(0,usbCamera->text(0));
        usb3->addChild(usbCamera);
    }
    ui->devicesTreeWidget->addTopLevelItems(topLevelItems);
    ui->devicesTreeWidget->setLayout(mainLayout);
    ui->devicesTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->devicesTreeWidget,&QTreeWidget::customContextMenuRequested,[=](const QPoint &pos){
        // prepare menu
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

    });
}

void Homescreen::detectAttachedDevices()
{

    using namespace HalconCpp;
    qDebug() << "Detecting attached devices";
    HalconCpp::HTuple *information = new HalconCpp::HTuple;
    HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
    try {
        HalconCpp::InfoFramegrabber("USB3Vision","info_boards",information, valueList);
        qDebug() << "Devices: "<<valueList->Length();
        qDebug() << "Info: "<<information;
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
}



void Homescreen::on_devicesRefresh_clicked()
{

    HalconCpp::HFramegrabber acq("USB3Vision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "2676016419A3_Basler_acA2040120um", 0, -1);
//    acq.SetFramegrabberParam("ExposureAuto","Off");

    qDebug() <<"Params: ";
    HalconCpp::HTuple h;
////    qDebug() <<"********************available_param_names******************************";
////    try {
////         h = acq.GetFramegrabberParam("available_param_names");
////    } catch (HalconCpp::HException &e) {
////        qDebug() << "Exception "<< e.ErrorMessage().Text();
////    }
////    qDebug() << h.ToString().Text() ;

//    try {
//         h = acq.GetFramegrabberParam("AcquisitionFrameRateEnable");
//    } catch (HalconCpp::HException &e) {
//        qDebug() << "Exception "<< e.ErrorMessage().Text();
//    }
//    qDebug() << "Here ";
//    qDebug() << "Reading"<<h.ToString().Text();
//    qDebug() << h.DArr()[0];

//    try {
//         h = acq.GetFramegrabberParam("ExposureAuto");
//    } catch (HalconCpp::HException &e) {
//        qDebug() << "Exception "<< e.ErrorMessage().Text();
//    }
//    qDebug() << h.ToString().Text();
//    qDebug() << h.DArr()[0];



    using namespace HalconCpp;
    HalconCpp::HTuple *information = new HalconCpp::HTuple;
    HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
    QList<QString> list = { "bits_per_channel", "camera_type", "color_space", "defaults", "device", "external_trigger", "field", "general", "generic", "horizontal_resolution", "image_height", "image_width", "info_boards", "parameters", "parameters_readonly", "parameters_writeonly", "port", "revision", "start_column", "start_row", "vertical_resolution"};
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
//    HalconCpp::InfoFramegrabber("usb3vision","parameters",information, valueList);
//    qDebug () << "Information: "<< information->S().Text();
//    qDebug() << "Valuelist length: "<<valueList->Length();
//    auto deviceList = valueList->ToSArr();
//    for(int i=0;i<valueList->Length();i++)
//    {
//        qDebug() << QString(deviceList[i].Text());
//    }



//    qDebug() << cameraControlsMap["267601642BB5_Basler_acA2040120um"].getGamma();
//    qDebug() << cameraControlsMap["2676016419A3_Basler_acA2040120um"].getGamma();




    //    HalconCpp::HFramegrabber acq("USB3Vision", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "267601642BB5_Basler_acA2040120um", 0, -1);
//    HalconCpp::HImage image = acq.GrabImage();
//    Hlong  width,height;
//      image.GetImageSize(&width,&height);

//      qDebug() << "Image size "<< width << height;
//      HalconCpp::WriteImage(image,"tiff",0,"C:/Users/daruizcadalso/Documents/QTApplications/USoundApp/sample.jpg");
//      HalconCpp::CloseFramegrabber(acq);



}


void Homescreen::connectToCamera(QString deviceName)
{
    try {
        // Create a new thread for image acquisition
        imageAcquisitionThread = new ImageAcquisition(deviceName, this);
        imageAcquisitionThread->setup();

        // Create a new window for streaming acquired images
        windowWidget = new ImageStreamWindow(this);
        windowWidget->setWindowTitle(deviceName);
        windowWidget->setImageAcquisitionThread(imageAcquisitionThread);
        windowWidget->setupCameraWindow();
        imageAcquisitionThread->start();

        // all signal/slots conections
        connect(imageAcquisitionThread,SIGNAL(renderImageSignal(QImage)),windowWidget,SLOT(renderImage(QImage)));


    } catch (QException &e) {
        qDebug () <<"Exception while connecting to camera";
        qDebug() <<e.what();
        throw e;
    }

}


void Homescreen::on_devicesTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString doubleClickedOn = item->text(0);
    if(devices.contains(doubleClickedOn))
    {
        connectToCamera(doubleClickedOn);
    }
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


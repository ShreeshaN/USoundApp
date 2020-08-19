#include "homescreen.h"
#include "ui_homescreen.h"
#include "imageacquisition.h"
#include "defaults.h"
#include <QMessageBox>

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
#include <usoundutils.h>
//#include "qteditorfactory.h"
//#include "qttreepropertybrowser.h"
//#include "qtpropertymanager.h"
//#include "qtvariantproperty.h"
#include <QComboBox>
#include <ui_about.h>
#include <ui_about.h>

// Initialize Global Message Box
QPlainTextEdit * Homescreen::globalMessageBox = 0;

Homescreen::Homescreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Homescreen)
{
    ui->setupUi(this);
    ui->logLevelSelector->setCurrentIndex(LOGGING_CONFIGURATION::LOG_LEVEL_INDEX);
    logLevel = LOGGING_CONFIGURATION::LOG_LEVEL;
    // Assign UI plainTextEdit to global Message Box
    globalMessageBox =ui->plainTextEdit;

    // Connect message log box to pushToMessageBoxSlot
    connect(this,SIGNAL(pushToMessageBoxSignal(QString)),this,SLOT(pushToMessageBoxSlot(QString)));
    // Connect slot to update log level on change in UI
    connect(ui->logLevelSelector,SIGNAL(activated(QString)),this,SLOT(updateLogLevel(QString)));
    // Connect slot to clear logs
    connect(ui->clearLogs,SIGNAL(clicked()),this,SLOT(clearLogs()));

    onApplicationStartup();
}

void Homescreen::setupDevicesUI()
{

    QList<QTreeWidgetItem *> topLevelItems;
    QList<QString> deviceTypes = devices.keys();
    QVBoxLayout* mainLayout = new QVBoxLayout();


    // USB
    // parent
    for (QString deviceType : deviceTypes) {
        auto deviceTypeWidget = new QTreeWidgetItem();
        deviceTypeWidget->setText(0, deviceType);
        topLevelItems.append(deviceTypeWidget);

        // child
        for (QString deviceName: devices.value(deviceType)){
            auto * camera = new QTreeWidgetItem(QStringList() << deviceName);
            camera->setToolTip(0,camera->text(0));
            deviceTypeWidget->addChild(camera);
        }
    }


    ui->devicesTreeWidget->addTopLevelItems(topLevelItems);
    ui->devicesTreeWidget->setLayout(mainLayout);
    ui->devicesTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    for(int i=0;i<topLevelItems.size();i++)
        topLevelItems[i]->setExpanded(true);

    connect(ui->devicesTreeWidget,&QTreeWidget::customContextMenuRequested,[=](const QPoint &pos){
        // prepare menu
        QTreeWidget *tree = ui->devicesTreeWidget;

        QTreeWidgetItem *nd = tree->itemAt( pos );

        qDebug()<<"Items : "<<pos<<nd->text(0);
        QString rightClickedOn = nd->text(0);
        if(!devices.keys().contains(rightClickedOn))
        {
            QAction *newAct = new QAction(QIcon(""), tr("&Connect"), this);
            connect(newAct, &QAction::triggered, this, [=](){
                QString deviceType = "";
                for(QString key: devices.keys())
                {
                    if(devices.value(key).contains(rightClickedOn)){
                        deviceType = key;
                        break;
                    }
                }
                connectToCamera(deviceType, deviceMakeMapping.value(rightClickedOn), rightClickedOn);});
            QMenu menu(this);
            menu.addAction(newAct);
            menu.exec(tree->mapToGlobal(pos));
        }

    });

}

void Homescreen::detectAttachedDevices()
{

    using namespace HalconCpp;
    try {
        qDebug() << "Detecting attached devices . . .";
        HalconCpp::HTuple *information = new HalconCpp::HTuple;
        HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
        QList<std::string> deviceTypes = {"USB3Vision","GigEVision2"};
        for (std::string deviceType  : deviceTypes) {
            QList<QString> devicesForDeviceType;
            HalconCpp::InfoFramegrabber(deviceType.c_str(),"info_boards",information, valueList);
            qDebug() << "Devices: "<<valueList->Length();
            qDebug() << "Info: "<<information;
            auto deviceList = valueList->ToSArr();
            for(int i=0;i<valueList->Length();i++)
            {
                QString deviceName = QString(deviceList[i].Text()).split("|")[1].split(":")[1].trimmed();
                qInfo() << "New camera detected "+deviceName;
                initializeDirectoriesForDevice(deviceName);
                devicesForDeviceType.append(deviceName);

                // Identify vendor
                // Sample output from Halcon
                /* " | device:000f314e2145_AlliedVisionTechnologies_MantaG1236CE0022719 |
                 unique_name:000f314e2145_AlliedVisionTechnologies_MantaG1236CE0022719 | interface:Esen_ITF_98e743951d79a9fec620ffff0000 |
                 producer:Esen | vendor:Allied Vision Technologies | model:Manta G-1236C (E0022719) | tl_type:GEV |
                 device_ip:169.254.162.10/16 | interface_ip:169.254.198.32/16 | status:available"
                */
                // Fetch vendor from the above string
                QString vendor = QString(deviceList[i].Text()).split("|")[5].split(":")[1].trimmed();
                deviceMakeMapping.insert(deviceName,vendor);
            }
            devices.insert(QString(deviceType.c_str()), devicesForDeviceType);
        }
    }
    catch (HException &except)
    {
        qDebug() << "Device unavailable";
        qDebug() << except.ErrorCode();
        qDebug() << except.ErrorMessage().Text();
        if (except.ErrorCode()==2042){
            QMessageBox Msgbox;
            Msgbox.critical(0,"Error", "Halcon license expired!");
            abort();
        }

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


//2676016419A3_Basler_acA2040120um
void Homescreen::on_devicesRefresh_clicked()
{


//    HalconCpp::HFramegrabber acq("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default", -1, "false", "default", "000f314e2145_AlliedVisionTechnologies_MantaG1236CE0022719", 0, -1);

//    HalconCpp::HTuple h;
//    qDebug() <<"Params: ";
//    qDebug() <<"********************available_param_names******************************";
//    try {
//        h = acq.GetFramegrabberParam("available_param_names");
//    } catch (HalconCpp::HException &e) {
//        qDebug() << "Exception "<< e.ErrorMessage().Text();
//    }
//    qDebug() << h.ToString().Text() ;

    //    try {
    //         HalconCpp::HTuple h;
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("PixelFormat");
    //         qDebug() <<"Pixel format" <<h.S().Text();

    //    double d = 10000;
    //    imageAcquisitionThread->getImageAcquisitionHandle().SetFramegrabberParam("ExposureTime",d);
    //    try {
    //         HalconCpp::HTuple h;
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("ExposureTime");
    //         qDebug() <<"Exposture time double" <<h.D();

    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    imageAcquisitionThread->getImageAcquisitionHandle().SetFramegrabberParam("ExposureAuto","Once");
    //    Sleep(2000);

    //    try {
    //         imageAcquisitionThread->getImageAcquisitionHandle().SetFramegrabberParam("AcquisitionFrameRateEnable", true);
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("AcquisitionFrameRateEnable");

    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    qDebug() <<"Acquisition frame rate enable" <<h.D();
    //    qDebug() << "Type "<<h.Type();

    //    try {
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("AcquisitionFrameRateEnable");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    qDebug() <<"Acquisition frame rate enable" <<h.D();
    //    HalconCpp::HTuple h;
    //    try {
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("ResultingFrameRate");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Resulting frame rate" <<h.DArr()[0];
    //    try {
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerSource");
    //        qDebug() << "Source" <<h.S().Text();
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerMode");
    //        qDebug() << "Mode" <<h.S().Text();
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerDelay");
    //        qDebug() << "Delay" <<h.D();
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerSelector");
    //        qDebug() << "Select" <<h.S().Text();


    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }



    //    using namespace HalconCpp;
    //    HalconCpp::HTuple *information = new HalconCpp::HTuple;
    //    HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
    //    QList<QString> list = { "bits_per_channel", "camera_type", "color_space", "defaults", "device", "external_trigger", "field", "general", "generic", "horizontal_resolution", "image_height", "image_width", "info_boards", "parameters", "parameters_readonly", "parameters_writeonly", "port", "revision", "start_column", "start_row", "vertical_resolution"};
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
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    imageAcquisitionThread->getImageAcquisitionHandle().SetFramegrabberParam("ExposureAuto","Once");
    //    Sleep(2000);

    //    try {
    //         imageAcquisitionThread->getImageAcquisitionHandle().SetFramegrabberParam("AcquisitionFrameRateEnable", true);
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("AcquisitionFrameRateEnable");

    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    qDebug() <<"Acquisition frame rate enable" <<h.D();
    //    qDebug() << "Type "<<h.Type();

    //    try {
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("AcquisitionFrameRateEnable");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    qDebug() <<"Acquisition frame rate enable" <<h.D();
    //    HalconCpp::HTuple h;
    //    try {
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("ResultingFrameRate");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Resulting frame rate" <<h.DArr()[0];
    //    try {
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerSource");
    //        qDebug() << "Source" <<h.S().Text();
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerMode");
    //        qDebug() << "Mode" <<h.S().Text();
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerDelay");
    //        qDebug() << "Delay" <<h.D();
    //        h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("TriggerSelector");
    //        qDebug() << "Select" <<h.S().Text();


    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }



    //    using namespace HalconCpp;
    //    HalconCpp::HTuple *information = new HalconCpp::HTuple;
    //    HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
    //    QList<QString> list = { "bits_per_channel", "camera_type", "color_space", "defaults", "device", "external_trigger", "field", "general", "generic", "horizontal_resolution", "image_height", "image_width", "info_boards", "parameters", "parameters_readonly", "parameters_writeonly", "port", "revision", "start_column", "start_row", "vertical_resolution"};
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
    //    HalconCpp::InfoFramegrabber("GigEVision2","parameters",information, valueList);
    //    qDebug () << "Information: "<< information->S().Text();
    //    qDebug() << "Valuelist length: "<<valueList->Length();
    //    auto deviceList = valueList->ToSArr();
    //    for(int i=0;i<valueList->Length();i++)
    //    {
    //        qDebug() << QString(deviceList[i].Text());
    //    }



    //    qDebug() << cameraControlsMap["267601642BB5_Basler_acA2040120um"].getGamma();
    //    qDebug() << cameraControlsMap["2676016419A3_Basler_acA2040120um"].getGamma();
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();

    //    try {
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("AcquisitionFrameRate");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    qDebug() <<"Acquisition frame rate" <<h.D();

    //    try {
    //         h = imageAcquisitionThread->getImageAcquisitionHandle().GetFramegrabberParam("AcquisitionFrameRateEnable");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << "Here ";
    //    qDebug() << "Reading"<<h.ToString().Text();
    //    qDebug() <<"Acquisition frame rate enable" <<h.D();

    //    try {
    //         h = acq.GetFramegrabberParam("ExposureAuto");
    //    } catch (HalconCpp::HException &e) {
    //        qDebug() << "Exception "<< e.ErrorMessage().Text();
    //    }
    //    qDebug() << h.ToString().Text();
    //    qDebug() << h.DArr()[0];


        try {
            using namespace HalconCpp;
            HalconCpp::HTuple *information = new HalconCpp::HTuple;
            HalconCpp::HTuple * valueList = new HalconCpp::HTuple;
            QList<QString> list = { "bits_per_channel", "camera_type", "color_space", "defaults", "device", "external_trigger", "field", "general", "generic", "horizontal_resolution", "image_height", "image_width", "info_boards", "parameters", "parameters_readonly", "parameters_writeonly", "port", "revision", "start_column", "start_row", "vertical_resolution"};
            for (auto param : list) {
                qDebug()<< "************" << param << "*************";
                HalconCpp::InfoFramegrabber("GigEVision2",HTuple(HString::FromUtf8(param.toUtf8())),information, valueList);
                qDebug () << "Information: "<< information->S().Text();
                qDebug() << "Valuelist length: "<<valueList->Length();
                auto deviceList = valueList->ToSArr();
                for(int i=0;i<valueList->Length();i++)
                {
                    qDebug() << QString(deviceList[i].Text());
                }

            }
        } catch (HalconCpp::HException &e) {
            qDebug() << e.ErrorMessage().Text();
        }

    //        HalconCpp::InfoFramegrabber("GigEVision","parameters",information, valueList);
    //        qDebug () << "Information: "<< information->S().Text();
    //        qDebug() << "Valuelist length: "<<valueList->Length();
    //        auto deviceList = valueList->ToSArr();
    //        for(int i=0;i<valueList->Length();i++)
    //        {
    //            qDebug() << QString(deviceList[i].Text());
    //        }



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


void Homescreen::connectToCamera(QString deviceType, QString deviceMake, QString deviceName)
{
    try {
        // Create a new thread for image acquisition
        imageAcquisitionThread = new ImageAcquisition(deviceType, deviceMake, deviceName, this);

        // Create a new window for streaming acquired images
        windowWidget = new ImageStreamWindow(imageAcquisitionThread, this);
        windowWidget->setWindowTitle(deviceName);
        windowWidget->setupCameraWindow();
        imageAcquisitionThread->start();

        // all signal/slots conections
        connect(imageAcquisitionThread,SIGNAL(renderImageSignal(QImage)),windowWidget,SLOT(renderImage(QImage)));
        connect(imageAcquisitionThread,SIGNAL(updateStatusBarSignal(QString)),windowWidget,SLOT(updateStatusBar(QString)));
        connect(imageAcquisitionThread,SIGNAL(updateStatusBarSignal(QString)),windowWidget,SLOT(updateStatusBar(QString)));
    } catch (QException &e) {
        qDebug () <<"Exception while connecting to camera";
        qDebug() <<e.what();
        //        throw e;
    }

    catch (HalconCpp::HOperatorException &e) {
        qDebug() << e.ErrorMessage().Text();
    }
    catch (std::exception &e) {
        qDebug() << e.what();
    }

}


void Homescreen::on_devicesTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString doubleClickedOn = item->text(0);
    if(!devices.keys().contains(doubleClickedOn))
    {
        QString deviceType = "";
        for(QString key: devices.keys())
        {
            if(devices.value(key).contains(doubleClickedOn)){
                deviceType = key;
                break;
            }
        }
        connectToCamera(deviceType, deviceMakeMapping.value(doubleClickedOn),doubleClickedOn);
    }
}


void Homescreen::onCameraWindowClose()
{
    //    windowWidget->close();
    qDebug() <<"In close";
}

void Homescreen::pushToMessageBoxSlot(QString message)
{
    ui->plainTextEdit->appendPlainText(message);
}

void Homescreen::updateLogLevel(QString message)
{
    qDebug() << "Log Level changed to: "+ message;
    if(message=="DEBUG"){
        logLevel = QtDebugMsg;
    }
    else if(message == "INFO"){
        logLevel = QtInfoMsg;
    }
    else if(message == "CRITICAL"){
        logLevel = QtCriticalMsg;
    }
    else if(message == "WARN"){
        logLevel = QtWarningMsg;
    }
    else if(message == "FATAL"){
        logLevel = QtFatalMsg;
    }
}

void Homescreen::clearLogs()
{
    ui->plainTextEdit->clear();
}

void Homescreen::on_actionAbout_triggered()
{
    auto aboutDialog = new QDialog(0,0);
    about->setupUi(aboutDialog);
    aboutDialog->exec();
}

void Homescreen::on_actionExit_triggered()
{
    abort();
}

void Homescreen::on_actionSettings_triggered()
{
    settings->exec();
}



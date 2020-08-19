#include "defaultcameraparameternames.h"
#include<defaults.h>
#include<QDebug>
#include<usoundutils.h>


using namespace std;
DefaultCameraParameterNames* DefaultCameraParameterNames::cameraSettings=nullptr;
DefaultCameraParameterNames *DefaultCameraParameterNames::getInstance()
{
    if(cameraSettings==nullptr){
        cameraSettings = new DefaultCameraParameterNames();
        cameraSettings->loadSettings();
    }
    return cameraSettings;
}


void DefaultCameraParameterNames::loadSettings()
{
    try {
        settings = new QSettings("CameraParameters.ini", QSettings::IniFormat);
        QStringList cameraMakes = settings->childGroups();
        for(int i=0;i<cameraMakes.size();i++)
        {
            qDebug()<< "Registering camera parameters for make: "<< cameraMakes.at(i);
            CameraParameters cameraParameters;
            settings->beginGroup(cameraMakes.at(i));
            cameraParameters.setEXPOSURETIME(settings->value("EXPOSURETIME").toString().toUtf8().constData());
            cameraParameters.setAUTOEXPOSURE(settings->value("AUTOEXPOSURE").toString().toUtf8().constData());
            cameraParameters.setGAMMA(settings->value("GAMMA").toString().toUtf8().constData());
            cameraParameters.setACQUISITIONFRAMERATE(settings->value("ACQUISITIONFRAMERATE").toString().toUtf8().constData());
            cameraParameters.setACQUISITIONFRAMERATEENABLE(settings->value("ACQUISITIONFRAMERATEENABLE").toString().toUtf8().constData());
            cameraParameters.setRESULTINGFRAMERATE(settings->value("RESULTINGFRAMERATE").toString().toUtf8().constData());
            cameraParameters.setGAIN(settings->value("GAIN").toString().toUtf8().constData());
            cameraParameters.setAUTOGAIN(settings->value("AUTOGAIN").toString().toUtf8().constData());
            cameraParameters.setHUE(settings->value("HUE").toString().toUtf8().constData());
            cameraParameters.setSATURATION(settings->value("SATURATION").toString().toUtf8().constData());
            cameraParameters.setBRIGHTNESS(settings->value("BRIGHTNESS").toString().toUtf8().constData());
            cameraParameters.setCONTRAST(settings->value("CONTRAST").toString().toUtf8().constData());
            cameraParameters.setMONOCHROME(settings->value("MONOCHROME").toString().toUtf8().constData());
            cameraParameters.setRGB(settings->value("RGB").toString().toUtf8().constData());
            settings->endGroup();
            defaultCameraParameterNames.insert(cameraMakes.at(i), cameraParameters);
        }
    }  catch (std::exception &e) {
        qCritical() << e.what();
        qCritical() << "Failed to load configuration from INI file. Using defaults!";
        throw CameraParameterNamesExceptioDefaultn();
    }

}




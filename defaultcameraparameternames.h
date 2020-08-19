#ifndef DEFAULTCAMERAPARAMETERNAMES_H
#define DEFAULTCAMERAPARAMETERNAMES_H
#include <singleton.h>
#include <QSettings>
#include "defaults.h"
//#include <exception>
using namespace std;


class CameraParameterNamesExceptioDefaultn: public exception
{

public:
  virtual const char* what() const throw()
  {
    return "Could not load camera parameters file";
  }
};


class DefaultCameraParameterNames
{

private:
    DefaultCameraParameterNames(){}
    DefaultCameraParameterNames(DefaultCameraParameterNames const&);
    void operator=(DefaultCameraParameterNames const&);

    static DefaultCameraParameterNames* cameraSettings;
public:
    void loadSettings();
    QSettings *settings;
    QMap<QString, CameraParameters> defaultCameraParameterNames;
    static DefaultCameraParameterNames *getInstance();
};


#endif // CAMERAPARAMETERNAMES_H

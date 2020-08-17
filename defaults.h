#ifndef DEFAULTS_H
#define DEFAULTS_H
#include <QString>


struct CameraControlDefaults{
    std::string RESOLUTION="";

    // exposure controls
    long EXPOSURETIME = 0;
    float ANALOGGAIN = 0.0;
    bool AUTOGAIN = false;
    float EXPOSURETARGET = 0.0;  // not sure about this implementation
    bool AUTOEXPOSURE=false;

    // color appearance parameters
    float HUE = 0.0;
    float SATURATION = 0.0;
    float BRIGHTNESS = 0.0;
    float CONTRAST = 0.0;
    float GAMMA = 0.0;
    long ACQUISITIONFRAMERATE=0;
    bool ACQUISITIONFRAMERATEENABLE=false;
    long RESULTINGFRAMERATE=0;
    bool MONOCHROME=true; // not sure about this implementation
    bool RGB=false; // not sure about this implementation
};


class BaslerCameraParameterNames{
public:
    static const std::string EXPOSURETIME;
    static const std::string AUTOEXPOSURE;
    static const std::string GAMMA;
    static const std::string ACQUISITIONFRAMERATE;
    static const std::string ACQUISITIONFRAMERATEENABLE;
    static const std::string RESULTINGFRAMERATE;
    static const  std::string GAIN;
    static const  std::string AUTOGAIN;
    static const  std::string HUE;
    static const  std::string SATURATION;
    static const  std::string BRIGHTNESS;
    static const  std::string CONTRAST;
    static const  std::string MONOCHROME;
    static const  std::string RGB;


};

class AvailableImageFormats{
public:
    static const inline QString JPEG="jpeg";
    static const inline QString BMP="bmp";
    static const inline QString JP2="jp2";
    static const inline QString JPEGXR = "jpegxr";
    static const inline QString TIFF = "tiff";
    static const inline QString PNG="png";
    static const inline QString HOBJ = "hobj";
    static const inline QString IMA = "ima";
};


class DIRECTORIES{
public:
    static QString APPDIR;
    static QString DATADIR;
    static QString IMAGESAVEDIR;
    static QString VIDEOSAVEDIR;
};

class LOGGING_CONFIGURATION{
public :    
    static QString FILE_NAME;
    static QString FILE_FORMAT;
    static QString LOG_FILE_PATH;
    static QtMsgType LOG_LEVEL;
    static int LOG_LEVEL_INDEX;
};

class IMAGE_CONFIGURATION{
public:
    static QString IMAGEFORMAT;
    static int IMAGEFORMAT_INDEX;
};



#endif // DEFAULTS_H

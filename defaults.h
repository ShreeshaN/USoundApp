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


class CameraParameterNames{
public:
    CameraParameterNames();
    std::string EXPOSURETIME;
    std::string AUTOEXPOSURE;
    std::string GAMMA;
    std::string ACQUISITIONFRAMERATE;
    std::string ACQUISITIONFRAMERATEENABLE;
    std::string RESULTINGFRAMERATE;
    std::string GAIN;
    std::string AUTOGAIN;
    std::string HUE;
    std::string SATURATION;
    std::string BRIGHTNESS;
    std::string CONTRAST;
    std::string MONOCHROME;
    std::string RGB;
    virtual std::string getHUE() const;
    void setHUE(const std::string &value);
    std::string getEXPOSURETIME() const;
    void setEXPOSURETIME(const std::string &value);
    std::string getAUTOEXPOSURE() const;
    void setAUTOEXPOSURE(const std::string &value);
    std::string getGAMMA() const;
    void setGAMMA(const std::string &value);
    std::string getACQUISITIONFRAMERATE() const;
    void setACQUISITIONFRAMERATE(const std::string &value);
    std::string getACQUISITIONFRAMERATEENABLE() const;
    void setACQUISITIONFRAMERATEENABLE(const std::string &value);
    std::string getRESULTINGFRAMERATE() const;
    void setRESULTINGFRAMERATE(const std::string &value);
    std::string getGAIN() const;
    void setGAIN(const std::string &value);
    std::string getAUTOGAIN() const;
    void setAUTOGAIN(const std::string &value);
    std::string getSATURATION() const;
    void setSATURATION(const std::string &value);
    std::string getBRIGHTNESS() const;
    void setBRIGHTNESS(const std::string &value);
    std::string getCONTRAST() const;
    void setCONTRAST(const std::string &value);
    std::string getMONOCHROME() const;
    void setMONOCHROME(const std::string &value);
    std::string getRGB() const;
    void setRGB(const std::string &value);
};

class BaslerCameraParameterNames: public CameraParameterNames{

public:
    BaslerCameraParameterNames();
    std::string EXPOSURETIME="ExposureTime";
    std::string AUTOEXPOSURE="ExposureAuto";
    std::string GAMMA="Gamma";
    std::string ACQUISITIONFRAMERATE="AcquisitionFrameRate";
    std::string ACQUISITIONFRAMERATEENABLE="AcquisitionFrameRateEnable";
    std::string RESULTINGFRAMERATE="ResultingFrameRate";
    std::string GAIN="Gain";
    std::string AUTOGAIN="GainAuto";
    // Although, I did not find these controls in acquisition handle's SetFramegrabberParam method
    // Initializing just to maintain equality
    std::string HUE="BslHue";
    std::string SATURATION="BslSaturation";
    std::string BRIGHTNESS="brightness";
    std::string CONTRAST="constrast";
    std::string MONOCHROME="monochrome";
    std::string RGB="rgb";

    std::string getHUE() const;
    void setHUE(const std::string &value);
};

class AlliedVisionCameraParameterNames: public CameraParameterNames{
public:
    AlliedVisionCameraParameterNames();
    static const inline std::string EXPOSURETIME="ExposureTime";
    static const inline std::string AUTOEXPOSURE="ExposureAuto";
    static const inline std::string GAMMA="Gamma";
    static const inline std::string ACQUISITIONFRAMERATE="AcquisitionFrameRate";
    static const inline std::string ACQUISITIONFRAMERATEENABLE="AcquisitionFrameRateEnable";
    static const inline std::string RESULTINGFRAMERATE="ResultingFrameRate";
    static const inline std::string GAIN="Gain";
    static const inline std::string AUTOGAIN="GainAuto";
    // Although, I did not find these controls in acquisition handle's SetFramegrabberParam method
    // Initializing just to maintain equality
    static const inline std::string HUE="BslHue";
    static const inline std::string SATURATION="BslSaturation";
    static const inline std::string BRIGHTNESS="brightness";
    static const inline std::string CONTRAST="constrast";
    static const inline std::string MONOCHROME="monochrome";
    static const inline std::string RGB="rgb";
};

class CameraMakes{
public:
    static const inline QString BASLER="Basler";
    static const inline QString ALLIEDVISION="Allied Vision Technologies";
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

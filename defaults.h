#ifndef DEFAULTS_H
#define DEFAULTS_H
#include <QString>


class CameraParameters{
public:
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
    std::string getHUE() const;
    void setHUE(const std::string &value);
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

class META{
public:
    static QString APP_NAME;
    static QString APP_VERSION;
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
    static QString IMAGE_FORMAT;
    static int IMAGE_FORMAT_INDEX;
    static int IMAGE_RESOLUTION_WIDTH;
    static int IMAGE_RESOLUTION_HEIGHT;
    static int IMAGE_GRID_ROWS;
    static int IMAGE_GRID_COLUMNS;
};



#endif // DEFAULTS_H

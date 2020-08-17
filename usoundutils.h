#ifndef UTILS_H
#define UTILS_H
#include <QDateTime>
#include <QDir>
#endif // UTILS_H
#include<thread>
#include<defaults.h>
#include <QDebug>

static std::string generateTimeStamp(){
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz").toLatin1().data();
}

static void mssleep(int ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void createDirectories(){
    qDebug() << "Initializing Directories . . .";
    QDir qdir;
    // App Dir    
    if (!qdir.exists(DIRECTORIES::APPDIR))   {
        qDebug() << "Creating Application directory at "+DIRECTORIES::APPDIR;
        qdir.mkpath(DIRECTORIES::APPDIR);
    }

    if (!qdir.exists(DIRECTORIES::APPDIR+"/"+DIRECTORIES::DATADIR))   {
        qDebug() << "Creating Data directory at "+DIRECTORIES::APPDIR+"/"+DIRECTORIES::DATADIR;
        qdir.mkpath(DIRECTORIES::APPDIR+"/"+DIRECTORIES::DATADIR);
    }
}

static void initializeDirectoriesForDevice(QString deviceName){
    qDebug() << "Initializing Directories for "+deviceName;
    QDir qdir;
    // App Dir
    QString deviceDir = DIRECTORIES::APPDIR+"/"+DIRECTORIES::DATADIR+"/"+deviceName;
    if (!qdir.exists(deviceDir))   {
        qDebug() << "Creating device directory at "+deviceDir;
        qdir.mkpath(deviceDir);
    }

    // Images
    if (!qdir.exists(deviceDir+DIRECTORIES::IMAGESAVEDIR))   {
        qDebug() << "Creating Image directory at "+deviceDir+DIRECTORIES::IMAGESAVEDIR;
        qdir.mkpath(deviceDir+DIRECTORIES::IMAGESAVEDIR);
    }

    // Video
    if (!qdir.exists(deviceDir+DIRECTORIES::VIDEOSAVEDIR))   {
        qDebug() << "Creating Video directory at "+deviceDir+DIRECTORIES::VIDEOSAVEDIR;
        qdir.mkpath(deviceDir+DIRECTORIES::VIDEOSAVEDIR);
    }
}

static QString getImageSavePathForDevice(QString deviceName){
    return DIRECTORIES::APPDIR+ "/" + DIRECTORIES::DATADIR+"/"+deviceName+DIRECTORIES::IMAGESAVEDIR;
}

static QString getVideoSavePathForDevice(QString deviceName){
    return DIRECTORIES::APPDIR+ "/" + DIRECTORIES::DATADIR+"/"+deviceName+DIRECTORIES::VIDEOSAVEDIR;
}

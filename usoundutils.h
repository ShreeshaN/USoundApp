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
    return std::to_string(currentDateTime.toTime_t());
}

static void mssleep(int ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void createDirectories(){
    qDebug() << "Initializing Directories . . .";
    QDir qdir;
    // App Dir
    if (!qdir.exists(Directories::APPDIR))   {
        qDebug() << "Creating Application directory at "+Directories::APPDIR;
        qdir.mkpath(Directories::APPDIR);
    }

    if (!qdir.exists(Directories::DATADIR))   {
        qDebug() << "Creating Data directory at "+Directories::DATADIR;
        qdir.mkpath(Directories::DATADIR);
    }
}

static void initializeDirectoriesForDevice(QString deviceName){
    qDebug() << "Initializing Directories for "+deviceName;
    QDir qdir;
    // App Dir
    QString deviceDir = Directories::DATADIR+"/"+deviceName;
    if (!qdir.exists(deviceDir))   {
        qDebug() << "Creating device directory at "+deviceDir;
        qdir.mkpath(deviceDir);
    }

    // Images
    if (!qdir.exists(deviceDir+Directories::IMAGESAVEDIR))   {
        qDebug() << "Creating Image directory at "+deviceDir+Directories::IMAGESAVEDIR;
        qdir.mkpath(deviceDir+Directories::IMAGESAVEDIR);
    }

    // Video
    if (!qdir.exists(deviceDir+Directories::VIDEOSAVEDIR))   {
        qDebug() << "Creating Video directory at "+deviceDir+Directories::VIDEOSAVEDIR;
        qdir.mkpath(deviceDir+Directories::VIDEOSAVEDIR);
    }
}

static QString getImageSavePathForDevice(QString deviceName){
    return Directories::DATADIR+"/"+deviceName+Directories::IMAGESAVEDIR;
}

static QString getVideoSavePathForDevice(QString deviceName){
    return Directories::DATADIR+"/"+deviceName+Directories::VIDEOSAVEDIR;
}

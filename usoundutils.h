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
    if (!qdir.exists(Directories::APPDIR.c_str()))   {
        qDebug() << ("Creating Application directory at "+Directories::APPDIR).c_str();
        qdir.mkpath(Directories::APPDIR.c_str());
    }

    // Images
    if (!qdir.exists(Directories::IMAGESAVEDIR.c_str()))   {
        qDebug() << ("Creating Image directory at "+Directories::IMAGESAVEDIR).c_str();
        qdir.mkpath(Directories::IMAGESAVEDIR.c_str());
    }

    // Video
    if (!qdir.exists(Directories::VIDEOSAVEDIR.c_str()))   {
        qDebug() << ("Creating Video directory at "+Directories::VIDEOSAVEDIR).c_str();
        qdir.mkpath(Directories::VIDEOSAVEDIR.c_str());
    }
}

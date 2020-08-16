#include "homescreen.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <usoundutils.h>
#include <QDateTime>
#include <defaults.h>
#include <QSettings>


QString logFilePath = "";
QString tempString = "";
Homescreen *homeScreenPointer = 0;
QString m_sSettingsFile;


void customLoggingHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("%1 - DEBUG - %3:%4:%5 - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stdout, "%s\n", txt.toLatin1().data());
        fflush(stdout);
        break;
    case QtInfoMsg:
        txt = QString("%1 - INFO - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData());
        fprintf(stdout, "%s\n", txt.toLatin1().data());
        fflush(stdout);
        break;
    case QtWarningMsg:
        txt = QString("%1 - WARNING - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData());
        fprintf(stdout, "%s\n", txt.toLatin1().data());
        fflush(stdout);
        break;
    case QtCriticalMsg:
        txt = QString("%1 - CRITICAL - %3:%4:%5 - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "%s\n", txt.toLatin1().data());
        fflush(stderr);
        break;
    case QtFatalMsg:
        txt = QString("%1 - FATAL - %3:%4:%5 - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "%s\n", txt.toLatin1().data());
        fflush(stderr);
        break;
    }

    //todo: Prathyush SP -> Set settings for log file path
    QFile outFile(logFilePath);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    //     Push to Message Box
    if(homeScreenPointer!=0 && Homescreen::globalMessageBox != 0 && Homescreen::logLevel == type){
        emit homeScreenPointer->pushToMessageBoxSignal(tempString+txt);
        tempString="";
    }
    else if (Homescreen::logLevel == type){
        tempString+=txt+'\n';
    }
}



void loadSettings()
{
    try {
        QSettings settings(m_sSettingsFile, QSettings::IniFormat);

        // Set Directories
        DIRECTORIES::APPDIR = settings.value("DIRECTORIES/APPDIR", DIRECTORIES::APPDIR).toString();

        // Set Logging Configuration
        LOGGING_CONFIGURATION::FILE_NAME = settings.value("LOGGING_CONFIGURATION/FILE_NAME", LOGGING_CONFIGURATION::FILE_NAME).toString();
        LOGGING_CONFIGURATION::FILE_FORMAT = settings.value("LOGGING_CONFIGURATION/FILE_FORMAT", LOGGING_CONFIGURATION::FILE_FORMAT).toString();

        // Set logfilepath
        logFilePath=DIRECTORIES::APPDIR+"/"+LOGGING_CONFIGURATION::FILE_NAME+QString(generateTimeStamp().c_str())+"."+LOGGING_CONFIGURATION::FILE_FORMAT;

        // Set loglevel
        auto logLevel = settings.value("LOGGING_CONFIGURATION/LOG_LEVEL", LOGGING_CONFIGURATION::LOG_LEVEL).toString();
        if (logLevel=="DEBUG"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=0;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtDebugMsg;
        }
        else if(logLevel == "INFO"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=1;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtInfoMsg;
        }
        else if(logLevel == "WARN"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=2;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtWarningMsg;
        }
        else if(logLevel == "CRITICAL"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=3;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtCriticalMsg;
        }
        else if(logLevel == "FATAL"){
            LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=4;
            LOGGING_CONFIGURATION::LOG_LEVEL = QtFatalMsg;
        }
        qDebug() << "Successfully loaded settings";
    }  catch (std::exception &e) {
        qDebug() << e.what();
        logFilePath=DIRECTORIES::APPDIR+"/"+LOGGING_CONFIGURATION::FILE_NAME+QString(generateTimeStamp().c_str())+"."+LOGGING_CONFIGURATION::FILE_FORMAT;
        qCritical() << "Failed to load configuration from INI file. Using defaults!";
    }

}


int main(int argc, char *argv[])
{
    try {
        // Load Settings
        m_sSettingsFile = "USoundSettings.ini";
        loadSettings();

        qRegisterMetaType<QList<long> >("QList<long>");
        // Setup directories
        createDirectories();
        qInstallMessageHandler(customLoggingHandler); // Install the handler
        qInfo() << "Logging to file:" + logFilePath;
        QApplication a(argc, argv);
        // This is used as a workaround to display menubar in mac os - https://stackoverflow.com/questions/25261760/menubar-not-showing-for-simple-qmainwindow-code-qt-creator-mac-os
        QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
        Homescreen w;
        homeScreenPointer = &w;
        w.setWindowIcon(QIcon("://icons/wpi_logo.ico"));
        w.show();
        qDebug() << "Application initialized successfully";
        return a.exec();
    }catch (HalconCpp::HOperatorException &e) {
        qDebug() << e.ErrorMessage().Text();
    }
    catch (std::exception &e) {
        qDebug() << e.what();
    }

}

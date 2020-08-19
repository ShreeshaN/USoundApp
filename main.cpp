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
#include<settingsstore.h>
#include"defaultcameraparameternames.h"
#include<QMessageBox>


QString tempString = "";
Homescreen *homeScreenPointer = 0;

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
    QFile outFile(LOGGING_CONFIGURATION::LOG_FILE_PATH);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << Qt::endl;
    //     Push to Message Box
    if(homeScreenPointer!=0 && Homescreen::globalMessageBox != 0 && LOGGING_CONFIGURATION::LOG_LEVEL <= type){
        emit homeScreenPointer->pushToMessageBoxSignal(tempString+txt);
        tempString="";
    }
    else if (Homescreen::logLevel <= type){
        tempString+=txt+'\n';
    }
}


int main(int argc, char *argv[])
{
    try {
        // Load Settings
        SettingsStore::loadSettings();
        qRegisterMetaType<QList<QLineSeries*> >("QList<QLineSeries*>");
        // Setup directories
        createDirectories();
        qInstallMessageHandler(customLoggingHandler); // Install the handler
        qInfo() << "Logging to file:" + LOGGING_CONFIGURATION::LOG_FILE_PATH;
        QApplication a(argc, argv);
        // This is used as a workaround to display menubar in mac os - https://stackoverflow.com/questions/25261760/menubar-not-showing-for-simple-qmainwindow-code-qt-creator-mac-os
        QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
        Homescreen w;
        homeScreenPointer = &w;
         qDebug() << "Application initialized successfully";
        w.setWindowIcon(QIcon("://icons/wpi_logo.ico"));
        w.show();
        return a.exec();
    }
    catch (HalconCpp::HOperatorException &e) {
        qFatal(e.ErrorMessage().Text());
    }
    catch (SettingsStoreException &e) {
        qFatal(e.what());
        QMessageBox::critical(0, "Issue with Settings",e.what(),QMessageBox::Ok);
    }
    catch (std::exception &e) {
        qFatal(e.what());
    }

}

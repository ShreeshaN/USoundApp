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

const std::string logFilePath = Directories::LOGDIR+"/"+Directories::LOGFILENAME+generateTimeStamp()+"."+Directories::LOGFILEFORMAT;
QString tempString = "";

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString txt;



    switch (type) {
    case QtDebugMsg:
        txt = QString("%1 - DEBUG - %3:%4:%5 - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stdout, "%s\n", txt.toLatin1().data());
        break;
    case QtInfoMsg:
        txt = QString("%1 - INFO - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData());
        fprintf(stdout, "%s\n", txt.toLatin1().data());
        break;
    case QtWarningMsg:
        txt = QString("%1 - WARNING - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData());
        fprintf(stdout, "%s\n", txt.toLatin1().data());
        break;
    case QtCriticalMsg:
        txt = QString("%1 - CRITICAL - %3:%4:%5 - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "%s\n", txt.toLatin1().data());
        break;
    case QtFatalMsg:
        txt = QString("%1 - FATAL - %3:%4:%5 - %2").arg(QDateTime::currentDateTime().toString()).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "%s\n", txt.toLatin1().data());
        break;
    }

    //todo: Prathyush SP -> Set settings for log file path
    QFile outFile(logFilePath.c_str());
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;

    // Push to Message Box
    if(Homescreen::globalMessageBox != 0){
        Homescreen::globalMessageBox->appendPlainText(tempString+txt);
        tempString="";
    }
    else{
        tempString+=txt+'\n';
    }


}

int main(int argc, char *argv[])
{
    try {
        fprintf(stdout, "Logging to file: %s", (logFilePath+"\n").c_str());
        qInstallMessageHandler(myMessageOutput); // Install the handler
        qInfo() << ("Logging to file:" + logFilePath).c_str();
        QApplication a(argc, argv);
        // This is used as a workaround to display menubar in mac os - https://stackoverflow.com/questions/25261760/menubar-not-showing-for-simple-qmainwindow-code-qt-creator-mac-os
        QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
        Homescreen w;
        w.setWindowIcon(QIcon("://icons/wpi_logo.ico"));
        w.show();
        return a.exec();
    } catch (std::exception &e) {
        qDebug() << e.what();
    }

}

#include "homescreen.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <usoundutils.h>
#include <logfilewatcherthread.h>
#include <QDateTime>

QString logFilePath = QDir::tempPath()+QString("/USoundApp-%1.log").arg(generateTimeStamp().c_str());
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
    QFile outFile(logFilePath);
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
        fprintf(stdout, "Logging to file: %s", (logFilePath.toStdString()+"\n").c_str());
        qInstallMessageHandler(myMessageOutput); // Install the handler
        qDebug() << QString("Logging to file: %1").arg(logFilePath);
        QApplication a(argc, argv);
        Homescreen w;
        w.setWindowIcon(QIcon("://icons/wpi_logo.ico"));
        w.show();
        return a.exec();
    } catch (std::exception &e) {
        qDebug() << e.what();
    }

}

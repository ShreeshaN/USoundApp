#include "settings.h"
#include "ui_settings.h"
#include<QFileDialog>
#include<QDialogButtonBox>
#include <QDebug>
#include<settingsstore.h>
#include <defaults.h>

using namespace std;

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->listWidget->setCurrentRow(0);

    //Set Directories
    loadDirectorySettings();

    // Logging Configuration
    loadLoggingSettings();

    // Image Configuration
    loadImageSettings();
}



Settings::~Settings()
{
    delete ui;
}

void Settings::loadImageSettings(){
    ui->imageSaveFormatToolbox->setCurrentIndex(IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX);
    ui->resolutionWidth->setText(QString::number(IMAGE_CONFIGURATION::IMAGE_RESOLUTION_WIDTH));
    ui->resolutionHeight->setText(QString::number(IMAGE_CONFIGURATION::IMAGE_RESOLUTION_HEIGHT));
}

void Settings::loadDirectorySettings(){
    ui->applicationDirectoryBrowserText->setText(DIRECTORIES::APPDIR);
    ui->dataDirectoryText->setText(DIRECTORIES::DATADIR);
    ui->imageDirectoryText->setText(DIRECTORIES::IMAGESAVEDIR);
    ui->videoDirectoryText->setText(DIRECTORIES::VIDEOSAVEDIR);
    ui->sampleImageSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::IMAGESAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
    ui->sampleVideoSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp of Record Start}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::VIDEOSAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
}


void Settings::loadLoggingSettings(){
    ui->logFileNameText->setText(LOGGING_CONFIGURATION::FILE_NAME);
    ui->logFileFormatText->setText(LOGGING_CONFIGURATION::FILE_FORMAT);
    ui->logLevelToolBox->setCurrentIndex(LOGGING_CONFIGURATION::LOG_LEVEL_INDEX);
}


void Settings::on_listWidget_itemSelectionChanged()
{
    ui->stackedWidget->setCurrentIndex(ui->listWidget->currentIndex().row());
}

void Settings::on_applicationDirectoryBrowser_clicked()
{
    // Set Application Directory
    QString selectedDir =
            QFileDialog::getExistingDirectory(this, "Select a directory", "directoryToOpen");
    DIRECTORIES::APPDIR = selectedDir;
    ui->applicationDirectoryBrowserText->setText(DIRECTORIES::APPDIR);
    SettingsStore::settings->setValue("DIRECTORIES/APPDIR", DIRECTORIES::APPDIR);
    ui->sampleImageSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::IMAGESAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
    ui->sampleVideoSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp of Record Start}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::VIDEOSAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
}

void Settings::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "Save"){
        qDebug() << "Saving settings to file  . . .";
        SettingsStore::saveSettings();
    }

    else if(button->text() == "Restore Defaults"){
        auto curIndex = ui->stackedWidget->currentIndex();

        if (curIndex==0){
            qDebug() << "Restoring default settings for Directories . . .";
            DIRECTORIES::reset();
            loadDirectorySettings();
        }
        else if (curIndex==1){
            qDebug() << "Restoring default settings for Logging . . .";
            LOGGING_CONFIGURATION::reset();
            loadLoggingSettings();
        }

        else if (curIndex==2){
            qDebug() << "Restoring default settings for Images . . .";
            IMAGE_CONFIGURATION::reset();
            loadImageSettings();
        }
    }

}

void Settings::on_dataDirectoryText_editingFinished()
{
    DIRECTORIES::DATADIR = ui->dataDirectoryText->text();
    SettingsStore::settings->setValue("DIRECTORIES/DATADIR", DIRECTORIES::DATADIR);
    ui->sampleImageSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::IMAGESAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
    ui->sampleVideoSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp of Record Start}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::VIDEOSAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
}

void Settings::on_imageDirectoryText_editingFinished()
{
    DIRECTORIES::IMAGESAVEDIR = ui->imageDirectoryText->text();
    SettingsStore::settings->setValue("DIRECTORIES/IMAGESAVEDIR", DIRECTORIES::IMAGESAVEDIR);
    ui->sampleImageSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::IMAGESAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
}

void Settings::on_videoDirectoryText_editingFinished()
{
    DIRECTORIES::VIDEOSAVEDIR = ui->videoDirectoryText->text();
    SettingsStore::settings->setValue("DIRECTORIES/VIDEOSAVEDIR", DIRECTORIES::VIDEOSAVEDIR);
    ui->sampleVideoSaveDirText->setText(QString("%1/%2/%3/{Device}/{Timestamp of Record Start}/{Timestamp}.%4").arg(DIRECTORIES::APPDIR).arg(DIRECTORIES::DATADIR).arg(DIRECTORIES::VIDEOSAVEDIR).arg(IMAGE_CONFIGURATION::IMAGE_FORMAT));
}

void Settings::on_logLevelToolBox_currentIndexChanged(int index)
{
    LOGGING_CONFIGURATION::LOG_LEVEL_INDEX=index;
    if (index==0){
        LOGGING_CONFIGURATION::LOG_LEVEL = QtDebugMsg;
        SettingsStore::settings->setValue("LOGGING_CONFIGURATION/LOG_LEVEL", "DEBUG");
    }
    else if(index == 1){
        LOGGING_CONFIGURATION::LOG_LEVEL = QtInfoMsg;
        SettingsStore::settings->setValue("LOGGING_CONFIGURATION/LOG_LEVEL", "INFO");
    }
    else if(index == 2){
        LOGGING_CONFIGURATION::LOG_LEVEL = QtWarningMsg;
        SettingsStore::settings->setValue("LOGGING_CONFIGURATION/LOG_LEVEL", "WARN");
    }
    else if(index == 3){
        LOGGING_CONFIGURATION::LOG_LEVEL = QtCriticalMsg;
        SettingsStore::settings->setValue("LOGGING_CONFIGURATION/LOG_LEVEL", "CRITICAL");
    }
    else if(index == 4){
        LOGGING_CONFIGURATION::LOG_LEVEL = QtFatalMsg;
        SettingsStore::settings->setValue("LOGGING_CONFIGURATION/LOG_LEVEL", "FATAL");
    }
}

void Settings::on_imageSaveFormatToolbox_currentIndexChanged(int index)
{
    IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX = index;
    if (index==0){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::TIFF;
    }
    else if(index == 1){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::JPEG;
    }
    else if(index == 2){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::BMP;
    }
    else if(index == 3){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::JP2;
    }
    else if(index == 4){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::JPEGXR;
    }
    else if(index == 5){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::PNG;
    }
    else if(index == 6){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::HOBJ;

    }
    else if(index == 7){
        IMAGE_CONFIGURATION::IMAGE_FORMAT=AvailableImageFormats::IMA;
    }
    IMAGE_CONFIGURATION::IMAGE_FORMAT_INDEX = index;
    SettingsStore::settings->setValue("IMAGE_CONFIGURATION/IMAGE_FORMAT", IMAGE_CONFIGURATION::IMAGE_FORMAT);
}

void Settings::on_resolutionWidth_editingFinished()
{
    IMAGE_CONFIGURATION::IMAGE_RESOLUTION_WIDTH = ui->resolutionWidth->text().toInt();
    SettingsStore::settings->setValue("IMAGE_CONFIGURATION/IMAGE_RESOLUTION_WIDTH", IMAGE_CONFIGURATION::IMAGE_RESOLUTION_WIDTH);
}

void Settings::on_resolutionHeight_editingFinished()
{
    IMAGE_CONFIGURATION::IMAGE_RESOLUTION_HEIGHT = ui->resolutionHeight->text().toInt();
    SettingsStore::settings->setValue("IMAGE_CONFIGURATION/IMAGE_RESOLUTION_HEIGHT", IMAGE_CONFIGURATION::IMAGE_RESOLUTION_HEIGHT);
}

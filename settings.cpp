#include "settings.h"
#include "ui_settings.h"
#include<QFileDialog>
#include<QDialogButtonBox>
#include <QDebug>
#include<settingsstore.h>
#include <defaults.h>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->listWidget->setCurrentRow(0);

    //Set Directories
    ui->applicationDirectoryBrowserText->setText(DIRECTORIES::APPDIR);

}

Settings::~Settings()
{
    delete ui;
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
    ui->applicationDirectoryBrowserText->setText(selectedDir);
    SettingsStore::settings->setValue("DIRECTORIES/APPDIR", selectedDir);
}

void Settings::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "Save"){
        SettingsStore::saveSettings();
    }

}

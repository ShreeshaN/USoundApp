#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <singleton.h>
#include <QAbstractButton>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    QSettings *settings = new QSettings();
    ~Settings();
    void loadImageSettings();
    void loadDirectorySettings();
    void loadLoggingSettings();

private slots:

    void on_listWidget_itemSelectionChanged();

    void on_applicationDirectoryBrowser_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_dataDirectoryText_editingFinished();

    void on_imageDirectoryText_editingFinished();

    void on_videoDirectoryText_editingFinished();

    void on_logLevelToolBox_currentIndexChanged(int index);

    void on_imageSaveFormatToolbox_currentIndexChanged(int index);

    void on_resolutionWidth_editingFinished();

    void on_resolutionHeight_editingFinished();

    void on_gridRows_editingFinished();

    void on_gridColumns_editingFinished();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H

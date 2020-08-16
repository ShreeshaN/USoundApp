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

private slots:

    void on_listWidget_itemSelectionChanged();

    void on_applicationDirectoryBrowser_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H

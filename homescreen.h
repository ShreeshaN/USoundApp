#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>
#include <ui_about.h>
#include <settings.h>
#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "Halcon.h"
#  include "HFramegrabber.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif
#include <QMap>
#include <map>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSpinBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include<QPixmap>
#include <QGraphicsView>
#include <imagestreamwindow.h>
#include<QtWidgets/QPlainTextEdit>
#include<QtWidgets/QListWidgetItem>
#include <QDir>


QT_BEGIN_NAMESPACE
namespace Ui { class Homescreen; }
QT_END_NAMESPACE

class Homescreen : public QMainWindow
{
    Q_OBJECT
public:
    Homescreen(QWidget *parent = nullptr);
    ~Homescreen();

    // variables
    QMap<QString, QList<QString>> devices;
    QMap<QString, QString> deviceMakeMapping;
    ImageStreamWindow *windowWidget;
    ImageAcquisition *imageAcquisitionThread;
    QLabel *imageLabel;
    static inline QtMsgType logLevel = QtInfoMsg;
    Ui::About *about = new Ui::About;
    Settings *settings = new Settings();
//    Ui_tabbedSettings *settings = new Ui_tabbedSettings;

    static QPlainTextEdit *globalMessageBox;

    QDir qDir;


    // functions
    void onApplicationStartup();
    void setupDevicesUI();
    void detectAttachedDevices();

public slots:
    void on_devicesRefresh_clicked();
    void connectToCamera(QString deviceType, QString deviceMake, QString deviceName);
    void on_devicesTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    //    void renderImage(QImage qImage, int widgetIndex);
    void onCameraWindowClose();
    void pushToMessageBoxSlot(QString message);
    void updateLogLevel(QString level);
    void clearLogs();

signals:
    void pushToMessageBoxSignal(QString message);

private slots:
    void on_actionAbout_triggered();
    void on_actionExit_triggered();
    void on_actionSettings_triggered();


private:
    Ui::Homescreen *ui;

};



#endif // HOMESCREEN_H

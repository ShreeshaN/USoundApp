#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QMainWindow>
#include <QLabel>
#include <HalconCpp.h>
#include "HFramegrabber.h"
#include <HFramegrabber.h>
#include <QMap>
#include <map>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSpinBox>
#include "cameracontrols.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include<QPixmap>
#include <QGraphicsView>
#include <imagestreamwindow.h>


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
    QMap<QString, CameraControls> cameraControlsMap;
    QList<QString> devices;

    // widgets
    QTreeWidgetItem *exposureControls;
    QTreeWidgetItem *exposureTime;
    QTreeWidgetItem *analogGain;
    QTreeWidgetItem *colorAppearance;
    QTreeWidgetItem *hue;
    QTreeWidgetItem *saturation;
    QTreeWidgetItem *brightness;
    QTreeWidgetItem *contrast;
    QTreeWidgetItem *gamma;
    QTreeWidgetItem *framerate;
    QTreeWidgetItem *monochrome;
    QTreeWidgetItem *rgb;

    ImageStreamWindow *windowWidget;
    QLabel *imageLabel;

    QList<ImageStreamWindow *> newCameraConnectionWindows;
    QList<QLabel *> newCameraConnectionLabels;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *graphicsPixmapItem;

    QList<QGraphicsPixmapItem *> graphicsPixmapItemList;





    // functions
    void onApplicationStartup();
    void setupDevicesUI();
    void detectAttachedDevices();
    QString cameraInSelection;



public slots:
    void on_devicesRefresh_clicked();
    void prepareMenu(const QPoint &pos);
    void on_devicesTreeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void connectToCamera(QString deviceName);
    void on_devicesTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void renderImage(QImage qImage, int widgetIndex);
    void onCameraWindowClose();
    void pushToMessageBoxSlot(QString message);

signals:
    void pushToMessageBoxSignal(QString message);

private:
    Ui::Homescreen *ui;

};
#endif // HOMESCREEN_H

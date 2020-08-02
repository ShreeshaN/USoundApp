#ifndef IMAGESTREAMWINDOW_H
#define IMAGESTREAMWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTreeWidgetItem>
#include "imageacquisition.h"
#include <QMap>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include<QPixmap>
#include <QGraphicsView>
#include <QSpinBox>
#include <QCheckBox>


class ImageStreamWindow : public QMainWindow
{
    Q_OBJECT

private:
    ImageAcquisition *imageAcquisitionThread;

public:
    explicit ImageStreamWindow(QWidget *parent = nullptr);


    // widgets
    QTreeWidgetItem *exposureControls;
    QTreeWidgetItem *exposureTime;
    QSpinBox *exposureTimeSpinBox;
    QTreeWidgetItem *analogGain;
    QDoubleSpinBox *analogGainSpinBox;
    QTreeWidgetItem *autoExposure;
    QCheckBox *autoExposureCheckbox;
    QTreeWidgetItem *colorAppearance;

    QTreeWidgetItem *hue;
    QSpinBox *hueSpinBox;
    QTreeWidgetItem *saturation;
    QSpinBox *saturationSpinBox;
    QTreeWidgetItem *brightness;
    QSpinBox *brightnessSpinBox;
    QTreeWidgetItem *contrast;
    QSpinBox *contrastSpinBox;
    QTreeWidgetItem *gamma;
    QDoubleSpinBox *gammaSpinBox;
    QTreeWidgetItem *acquisitionFrameRate;
    QSpinBox *acquisitionFramerateSpinBox;
    QTreeWidgetItem *acquisitionFrameRateEnable;
    QCheckBox *acquisitionFrameRateEnableCheckbox;
    QTreeWidgetItem *monochrome;
    QCheckBox *monochromeCheckbox;
    QTreeWidgetItem *rgb;
    QCheckBox *rgbCheckbox;

    QLabel *imageLabel;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *graphicsPixmapItem;

    QList<QGraphicsPixmapItem *> graphicsPixmapItemList;



    // Actions
    QAction *imageSaveButton, *recordButton, *recordPauseButton, *recordStopButton;



    // member functions
    void setupCameraWindow();
    void updateCameraParametersAndDisplay();
    void displayCameraParameters();


    // setters and getters
    HalconCpp::HFramegrabber getAcquisitionHandle() const;
    void setAcquisitionHandle(const HalconCpp::HFramegrabber &value);
    ImageAcquisition *getImageAcquisitionThread() const;
    void setImageAcquisitionThread(ImageAcquisition *value);




signals:


public slots:
    void renderImage(QImage qImage);
    void saveImage();
    void startVideoRecord();
    void pauseVideoRecord();
    void stopVideoRecord();


protected:
    void closeEvent(QCloseEvent *event);
};

#endif // IMAGESTREAMWINDOW_H

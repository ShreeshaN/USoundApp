#ifndef SPINBOXCONTAINER_H
#define SPINBOXCONTAINER_H
#include <QSpinBox>
#include <QTreeWidgetItem>
#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "Halcon.h"
#  include "HFramegrabber.h"
#include "imageacquisition.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif

class SpinboxContainer : public QSpinBox
{
    Q_OBJECT
public:
    SpinboxContainer(int, QString, int, int, ImageAcquisition*, QWidget *parent = nullptr);
    
    // should contain update and display functions. shoudl contain ranges for spinbox
    // should contain qtreewidgetitem and qspinbox
    QString getCameraParameterName() const;
    void setCameraParameterName(const QString &value);

    ImageAcquisition *getImageAcquisitionThread() const;
    void setImageAcquisitionThread(ImageAcquisition *value);

    QString getUiDisplayName() const;
    void setUiDisplayName(const QString &value);

    QSpinBox *getUiElement() const;
    void setUiElement(QSpinBox *value);

    QTreeWidgetItem *getQTreeWidgetItem() const;
    void setQTreeWidgetItem(QTreeWidgetItem *value);

    int getMinVal() const;
    void setMinVal(int value);

    int getMaxVal() const;
    void setMaxVal(int value);

    void update();
    void display();
    void fetch();
    void customConnect();
    void setValueInHardware(int);


    int getDefaultParameterValue() const;
    void setDefaultParameterValue(int value);

    typedef void (*SetFunction)();

private:
    int defaultParameterValue;
    QString cameraParameterName, uiDisplayName;
    ImageAcquisition *imageAcquisitionThread;
    QSpinBox *uiElement;
    QTreeWidgetItem *qTreeWidgetItem;
    int maxVal, minVal;
    QOverload<int> qOverloadInt;
    HalconCpp::HTuple val;

};

#endif // SPINBOXCONTAINER_H

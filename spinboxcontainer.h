#ifndef SPINBOXCONTAINER_H
#define SPINBOXCONTAINER_H
#include <QSpinBox>
#include <QTreeWidgetItem>
#include "parametercontainer.h"
#include "imageacquisition.h"
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


class SpinboxContainer: public QSpinBox, public ParameterContainer
{
    Q_OBJECT
public:
    SpinboxContainer(double defaultParameterValue, std::string cameraParameterName, std::string uiDisplayName, int minVal, int maxVal, int step, ImageAcquisition* imageAcquisitionThread, QWidget *parent = nullptr);

    ImageAcquisition *getImageAcquisitionThread() const;
    void setImageAcquisitionThread(ImageAcquisition *value);

    QSpinBox *getUiElement() const;
    void setUiElement(QSpinBox *value);

    QTreeWidgetItem *getQTreeWidgetItem() const;
    void setQTreeWidgetItem(QTreeWidgetItem *value);

    int getMinVal() const;
    void setMinVal(int value);

    int getMaxVal() const;
    void setMaxVal(int value);

    std::string getCameraParameterName() const;
    void setCameraParameterName(const std::string &value);

    std::string getUiDisplayName() const;
    void setUiDisplayName(const std::string &value);

    double getParamValue() const;
    void setParamValue(double value);

    void updateParamValue() override;
    void displayParamValue() override;
    void setValueInHardware(int) override;
    void setValueInHardware(double) override;
    void setValueInHardware(std::string) override;
    void setValueInHardware(bool) override;

private:
    double paramValue;
    std::string cameraParameterName, uiDisplayName;
    ImageAcquisition *imageAcquisitionThread;
    QSpinBox *uiElement;
    QTreeWidgetItem *qTreeWidgetItem;
    int maxVal, minVal;
    QOverload<int> qOverloadInt;
    HalconCpp::HTuple val;


};

#endif // SPINBOXCONTAINER_H

#ifndef DOUBLESPINBOXCONTAINER_H
#define DOUBLESPINBOXCONTAINER_H
#include <QDoubleSpinBox>
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
#include "doubleslider.h"

class DoubleSpinboxContainer : public QDoubleSpinBox, public ParameterContainer
{
    Q_OBJECT
public:
    DoubleSpinboxContainer(double defaultParameterValue, std::string cameraParameterName, std::string uiDisplayName, double minVal, double maxVal, double step, double multiplierForSlider, ImageAcquisition* imageAcquisitionThread, QWidget *parent = nullptr);

    void updateParamValue() override;
    void displayParamValue() override;
    void setValueInHardware(int) override;
    void setValueInHardware(double) override;
    void setValueInHardware(std::string) override;
    void setValueInHardware(bool) override;


    ImageAcquisition *getImageAcquisitionThread() const;
    void setImageAcquisitionThread(ImageAcquisition *value);

    QTreeWidgetItem *getQTreeWidgetItem() const;
    void setQTreeWidgetItem(QTreeWidgetItem *value);

    std::string getCameraParameterName() const;
    void setCameraParameterName(const std::string &value);

    std::string getUiDisplayName() const;
    void setUiDisplayName(const std::string &value);

    double getParamValue() const;
    void setParamValue(double value);

    QDoubleSpinBox *getUiElement() const;
    void setUiElement(QDoubleSpinBox *value);

    double getMinVal() const;
    void setMinVal(double value);

    double getMaxVal() const;
    void setMaxVal(double value);

    double getStep() const;
    void setStep(double value);

    DoubleSlider *getDoubleSlider() const;
    void setDoubleSlider(DoubleSlider *value);

    double getMultiplierForSlider() const;
    void setMultiplierForSlider(double value);

private:
    double paramValue;
    std::string cameraParameterName, uiDisplayName;
    ImageAcquisition *imageAcquisitionThread;
    QDoubleSpinBox *uiElement;
    QTreeWidgetItem *qTreeWidgetItem;
    double maxVal, minVal, step;
    QOverload<double> qOverloadDouble;
    HalconCpp::HTuple val;
    DoubleSlider *doubleSlider;
    double multiplierForSlider; // This is required because QT does not support setting double values for sliders, so a multiplying factor is taken into account

};

#endif // DOUBLESPINBOXCONTAINER_H

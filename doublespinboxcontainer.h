#ifndef DOUBLESPINBOXCONTAINER_H
#define DOUBLESPINBOXCONTAINER_H
#include <QDoubleSpinBox>
#include <QTreeWidgetItem>
#include "parametercontainer.h"
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


class DoubleSpinboxContainer : public QDoubleSpinBox, public ParameterContainer
{
    Q_OBJECT
public:
    DoubleSpinboxContainer(double defaultParameterValue, std::string cameraParameterName, std::string uiDisplayName, double minVal, double maxVal, double step ,ImageAcquisition* imageAcquisitionThread, QWidget *parent = nullptr);
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
    QDoubleSpinBox *uiElement;
    QTreeWidgetItem *qTreeWidgetItem;
    double maxVal, minVal, step;
    QOverload<double> qOverloadDouble;
    HalconCpp::HTuple val;

};

#endif // DOUBLESPINBOXCONTAINER_H

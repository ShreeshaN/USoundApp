#ifndef CHECKBOXCONTAINER_H
#define CHECKBOXCONTAINER_H
#include <QCheckBox>
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

class CheckboxContainer: public QCheckBox, public ParameterContainer
{
    Q_OBJECT
public:
    CheckboxContainer(bool defaultParameterState, std::string cameraParameterName, std::string uiDisplayName,  std::string checkedValue,  std::string uncheckedValue, ImageAcquisition* imageAcquisitionThread, QWidget *parent = nullptr);

    void displayParamValue() override;
    void updateParamValue() override;
    void setValueInHardware(double) override;
    void setValueInHardware(int) override;
    void setValueInHardware(std::string) override;
    void setValueInHardware(bool) override;

    bool getParamState() const;
    void setParamState(bool value);

    std::string getCameraParameterName() const;
    void setCameraParameterName(const std::string &value);

    ImageAcquisition *getImageAcquisitionThread() const;
    void setImageAcquisitionThread(ImageAcquisition *value);

    std::string getUiDisplayName() const;
    void setUiDisplayName(const std::string &value);

    QCheckBox *getUiElement() const;
    void setUiElement(QCheckBox *value);

    QTreeWidgetItem *getQTreeWidgetItem() const;
    void setQTreeWidgetItem(QTreeWidgetItem *value);

    HalconCpp::HTuple getVal() const;
    void setVal(const HalconCpp::HTuple &value);

    std::string getCheckedValue() const;
    void setCheckedValue(const std::string &value);

    std::string getUncheckedValue() const;
    void setUncheckedValue(const std::string &value);


private:
    bool paramState;
    std::string cameraParameterName, uiDisplayName;
    ImageAcquisition *imageAcquisitionThread;
    QCheckBox *uiElement;
    QTreeWidgetItem *qTreeWidgetItem;
    HalconCpp::HTuple val;
    std::string checkedValue, uncheckedValue; // set this variable to ""(empty str) if the camera parameter takes bool values directly. Else set it to appropriate strings for checked and unchecked variables.

};

#endif // CHECKBOXCONTAINER_H

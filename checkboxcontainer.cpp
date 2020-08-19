#include "checkboxcontainer.h"
#include <string.h>
#include "usoundutils.h"

CheckboxContainer::CheckboxContainer(bool defaultParameterState, std::string cameraParameterName, std::string uiDisplayName, std::string checkedValue, std::string uncheckedValue, ImageAcquisition* imageAcquisitionThread, QWidget *parent): QCheckBox(parent)
{
    this->paramState = defaultParameterState;
    this->cameraParameterName = cameraParameterName;
    this->uiDisplayName = uiDisplayName;
    qTreeWidgetItem = new QTreeWidgetItem(QStringList() << uiDisplayName.c_str());
    uiElement = new QCheckBox();
    uiElement->setTristate(false);
    this->imageAcquisitionThread = imageAcquisitionThread;
    this->checkedValue = checkedValue;
    this->uncheckedValue = uncheckedValue;
    updateParamValue();
    displayParamValue();


    // connect
    QObject::connect(this->uiElement, &QCheckBox::clicked,[=]{
//        qDebug() << "recieved from UI" << uiElement->isChecked();
        if (checkedValue.compare("") == 0){
            // if the camera parameter takes bool directly instead of strings
            this->setValueInHardware(uiElement->isChecked());
        }
        else{
            // if the camera parameter takes string names instead of bool, then take the bool value from UI and set appropriate string for the camera
            this->setValueInHardware(uiElement->isChecked()?checkedValue:uncheckedValue);
        }
//        mssleep(250);
        this->updateParamValue();
        this->displayParamValue();
    });


}

void CheckboxContainer::displayParamValue()
{
    if(this->getParameterAvailable())
    {
        this->uiElement->blockSignals(true);
        uiElement->setCheckState(paramState?Qt::CheckState(2):Qt::CheckState(0));
        this->uiElement->blockSignals(false);
    //    qDebug() << "Displaying value of "<<cameraParameterName.c_str() << "param value"<< paramState;
    }

}

void CheckboxContainer::updateParamValue()
{
    try {
        if(this->getParameterAvailable())
        {
            val = this->imageAcquisitionThread->getValueForParam(cameraParameterName);
            if(checkedValue.compare("") == 0){
                // this condition is satisfied if the camera parameter returns 0 or 1 (mimicking a bool) instead of string values
                int(val.D()) == 0?setParamState(false):setParamState(true);
            }
            else{
                // this condition is satisfied if the camera parameter returns strings like "Off", "Once", "Continuous".
                // In that case the return strings should be equal to checkedValue or uncheckedValue variables of this class.
                QString::compare(val.S().Text(),uncheckedValue.c_str()) == 0?setParamState(false):setParamState(true);
    //            qDebug() << "recieved from hardware" << val.S().Text();
        }
    //        qDebug() << "Updating param"<<cameraParameterName.c_str()<< "New state "<< paramState;
        }

    } catch (HalconCpp::HException &e) {
        if (e.ErrorCode() == 5330)
        {
            this->uiElement->setDisabled(true);
            this->setParameterAvailable(false);
            qDebug() << "Either parameter name is incorrect or the camera make does not support it>>"<< cameraParameterName.c_str()<< "<<. Currently, a default value is set";
        }
        else{
            qDebug() << "Unknown error while setting value for "<<cameraParameterName.c_str();
        }
    }

}

void CheckboxContainer::setValueInHardware(double)
{

}

void CheckboxContainer::setValueInHardware(int)
{

}

void CheckboxContainer::setValueInHardware(std::string val)
{
    this->imageAcquisitionThread->setValueForParam(cameraParameterName, val);
}

void CheckboxContainer::setValueInHardware(bool)
{
    this->imageAcquisitionThread->setValueForParam(cameraParameterName, uiElement->isChecked());
}

bool CheckboxContainer::getParamState() const
{
    return paramState;
}

void CheckboxContainer::setParamState(bool value)
{
    paramState = value;
}

std::string CheckboxContainer::getCameraParameterName() const
{
    return cameraParameterName;
}

void CheckboxContainer::setCameraParameterName(const std::string &value)
{
    cameraParameterName = value;
}

ImageAcquisition *CheckboxContainer::getImageAcquisitionThread() const
{
    return imageAcquisitionThread;
}

void CheckboxContainer::setImageAcquisitionThread(ImageAcquisition *value)
{
    imageAcquisitionThread = value;
}

std::string CheckboxContainer::getUiDisplayName() const
{
    return uiDisplayName;
}

void CheckboxContainer::setUiDisplayName(const std::string &value)
{
    uiDisplayName = value;
}

QCheckBox *CheckboxContainer::getUiElement() const
{
    return uiElement;
}

void CheckboxContainer::setUiElement(QCheckBox *value)
{
    uiElement = value;
}

QTreeWidgetItem *CheckboxContainer::getQTreeWidgetItem() const
{
    return qTreeWidgetItem;
}

void CheckboxContainer::setQTreeWidgetItem(QTreeWidgetItem *value)
{
    qTreeWidgetItem = value;
}

HalconCpp::HTuple CheckboxContainer::getVal() const
{
    return val;
}

void CheckboxContainer::setVal(const HalconCpp::HTuple &value)
{
    val = value;
}

std::string CheckboxContainer::getCheckedValue() const
{
    return checkedValue;
}

void CheckboxContainer::setCheckedValue(const std::string &value)
{
    checkedValue = value;
}

std::string CheckboxContainer::getUncheckedValue() const
{
    return uncheckedValue;
}

void CheckboxContainer::setUncheckedValue(const std::string &value)
{
    uncheckedValue = value;
}

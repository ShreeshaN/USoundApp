#include "doublespinboxcontainer.h"

DoubleSpinboxContainer::DoubleSpinboxContainer(double defaultParameterValue, std::string cameraParameterName,std::string uiDisplayName, double minVal, double maxVal, double step, ImageAcquisition *imageAcquisitionThread, QWidget *parent): QDoubleSpinBox(parent)
{
    this->paramValue = defaultParameterValue;
    this->uiDisplayName = uiDisplayName;
    this->cameraParameterName = cameraParameterName;
    this->minVal = minVal;
    this->maxVal = maxVal;
    this->step = step;
    qTreeWidgetItem = new QTreeWidgetItem(QStringList() << uiDisplayName.c_str());
    uiElement = new QDoubleSpinBox;
    uiElement->setRange(minVal, maxVal);
    uiElement->setSingleStep(step);
    this->imageAcquisitionThread = imageAcquisitionThread;
    displayParamValue();

    // connect
    connect(this->uiElement, qOverloadDouble(&QDoubleSpinBox::valueChanged),[=]{
        this->setValueInHardware(uiElement->value());
        this->updateParamValue();
        this->displayParamValue();

    });
}

// Virtual method implementations
void DoubleSpinboxContainer::updateParamValue()
{
    val = this->imageAcquisitionThread->getValueForParam(cameraParameterName);
    this->paramValue = val.D();
}

void DoubleSpinboxContainer::displayParamValue()
{
    this->uiElement->setValue(paramValue);
}

void DoubleSpinboxContainer::setValueInHardware(double paramValue)
{
    this->imageAcquisitionThread->setValueForParam(cameraParameterName,paramValue);
}

void DoubleSpinboxContainer::setValueInHardware(bool)
{

}

void DoubleSpinboxContainer::emitUiElementChangedSignal()
{

}

//void DoubleSpinboxContainer::updateAllParametersSignal()
//{
//    emit this->valueChanged(0);
//}
void DoubleSpinboxContainer::setValueInHardware(std::string)
{

}

void DoubleSpinboxContainer::setValueInHardware(int)
{

}


// Setters and getters
QDoubleSpinBox *DoubleSpinboxContainer::getUiElement() const
{
    return uiElement;
}

void DoubleSpinboxContainer::setUiElement(QDoubleSpinBox *value)
{
    uiElement = value;
}

QTreeWidgetItem *DoubleSpinboxContainer::getQTreeWidgetItem() const
{
    return qTreeWidgetItem;
}

void DoubleSpinboxContainer::setQTreeWidgetItem(QTreeWidgetItem *value)
{
    qTreeWidgetItem = value;
}

std::string DoubleSpinboxContainer::getCameraParameterName() const
{
    return cameraParameterName;
}

void DoubleSpinboxContainer::setCameraParameterName(const std::string &value)
{
    cameraParameterName = value;
}

std::string DoubleSpinboxContainer::getUiDisplayName() const
{
    return uiDisplayName;
}

void DoubleSpinboxContainer::setUiDisplayName(const std::string &value)
{
    uiDisplayName = value;
}

ImageAcquisition *DoubleSpinboxContainer::getImageAcquisitionThread() const
{
    return imageAcquisitionThread;
}

void DoubleSpinboxContainer::setImageAcquisitionThread(ImageAcquisition *value)
{
    imageAcquisitionThread = value;
}

double DoubleSpinboxContainer::getMinVal() const
{
    return minVal;
}

void DoubleSpinboxContainer::setMinVal(double value)
{
    minVal = value;
}

double DoubleSpinboxContainer::getMaxVal() const
{
    return maxVal;
}

void DoubleSpinboxContainer::setMaxVal(double value)
{
    maxVal = value;
}

double DoubleSpinboxContainer::getParamValue() const
{
    return paramValue;
}

void DoubleSpinboxContainer::setParamValue(double value)
{
    paramValue = value;
}


double DoubleSpinboxContainer::getStep() const
{
    return step;
}

void DoubleSpinboxContainer::setStep(double value)
{
    step = value;
}

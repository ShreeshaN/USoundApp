#include "spinboxcontainer.h"
#include <QDebug>
#include "usoundutils.h"

SpinboxContainer::SpinboxContainer(double defaultParameterValue, std::string cameraParameterName, std::string uiDisplayName, int minVal, int maxVal, int step, ImageAcquisition *imageAcquisitionThread, QWidget *parent): QSpinBox(parent)
{
 this->paramValue = defaultParameterValue;
 this->uiDisplayName = uiDisplayName;
 this->cameraParameterName = cameraParameterName;
 this->minVal = minVal;
 this->maxVal = maxVal;
 qTreeWidgetItem = new QTreeWidgetItem(QStringList() << uiDisplayName.c_str());
 uiElement = new QSpinBox;
 uiElement->setRange(minVal, maxVal);
 uiElement->setSingleStep(step);
 this->imageAcquisitionThread = imageAcquisitionThread;
 displayParamValue();

 // connect
 connect(this->uiElement, qOverloadInt(&QSpinBox::valueChanged),[=]{
     this->setValueInHardware((double)uiElement->value());
     mssleep(150);
     this->updateParamValue();
     this->displayParamValue();
//     emit updateAllParametersSignal();
});
}

//void SpinboxContainer::setme(ImageStreamWindow *ww)
//{
//   this->w = ww;
//}

// Virtual method implementations

void SpinboxContainer::updateParamValue()
{
    val = this->imageAcquisitionThread->getValueForParam(cameraParameterName);
    this->paramValue = val.D();
}

void SpinboxContainer::displayParamValue()
{
    this->uiElement->setValue(paramValue);
}

void SpinboxContainer::setValueInHardware(int paramValue)
{
//    this->imageAcquisitionThread->setValueForParam(cameraParameterName,paramValue);
}

void SpinboxContainer::setValueInHardware(double paramValue)
{
    this->imageAcquisitionThread->setValueForParam(cameraParameterName,paramValue);
}

void SpinboxContainer::setValueInHardware(bool)
{

}

void SpinboxContainer::emitUiElementChangedSignal()
{
    emit this->valueChanged(0);
}

//void SpinboxContainer::updateAllParametersSignal()
//{
//    emit this->valueChanged(0);
//}

void SpinboxContainer::setValueInHardware(std::string)
{

}

// Setters and Getters
QSpinBox *SpinboxContainer::getUiElement() const
{
    return uiElement;
}

void SpinboxContainer::setUiElement(QSpinBox *value)
{
    uiElement = value;
}

QTreeWidgetItem *SpinboxContainer::getQTreeWidgetItem() const
{
    return qTreeWidgetItem;
}

void SpinboxContainer::setQTreeWidgetItem(QTreeWidgetItem *value)
{
    qTreeWidgetItem = value;
}

int SpinboxContainer::getMinVal() const
{
    return minVal;
}

void SpinboxContainer::setMinVal(int value)
{
    minVal = value;
}

int SpinboxContainer::getMaxVal() const
{
    return maxVal;
}

void SpinboxContainer::setMaxVal(int value)
{
    maxVal = value;
}


std::string SpinboxContainer::getCameraParameterName() const
{
    return cameraParameterName;
}

void SpinboxContainer::setCameraParameterName(const std::string &value)
{
    cameraParameterName = value;
}

std::string SpinboxContainer::getUiDisplayName() const
{
    return uiDisplayName;
}

void SpinboxContainer::setUiDisplayName(const std::string &value)
{
    uiDisplayName = value;
}

ImageAcquisition *SpinboxContainer::getImageAcquisitionThread() const
{
    return imageAcquisitionThread;
}

void SpinboxContainer::setImageAcquisitionThread(ImageAcquisition *value)
{
    imageAcquisitionThread = value;
}

double SpinboxContainer::getParamValue() const
{
    return paramValue;
}

void SpinboxContainer::setParamValue(double value)
{
    paramValue = value;
}

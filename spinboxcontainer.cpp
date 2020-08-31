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

    // Slider
    intSlider = new IntSlider(this->getParamValue(),minVal, maxVal, step, Qt::Horizontal);
    connect(intSlider, &IntSlider::valueChanged,uiElement, &QSpinBox::setValue);
    connect(uiElement, qOverloadInt(&QSpinBox::valueChanged),intSlider,&IntSlider::setValue);

    updateParamValue();
    displayParamValue();

    // connect
    QObject::connect(this->uiElement, qOverloadInt(&QSpinBox::valueChanged),[=]{
        this->setValueInHardware((double)uiElement->value());
        this->updateParamValue();
        this->displayParamValue();
    });
}

// Virtual method implementations

void SpinboxContainer::updateParamValue()
{
    try {
        if(this->getParameterAvailable())
        {
            val = this->imageAcquisitionThread->getValueForParam(cameraParameterName);
            this->paramValue = val.D();
            //        qDebug() << "Updating param"<<cameraParameterName.c_str() << "New value "<< paramValue;

        }

    } catch (HalconCpp::HException &e) {
        if (e.ErrorCode() == 5330)
        {
            this->uiElement->setDisabled(true);
            this->intSlider->setDisabled(true);
            this->setParameterAvailable(false);
            qDebug() << "Either parameter name is incorrect or the camera make does not support it>>"<< cameraParameterName.c_str()<< "<<. Currently, a default value is set";
        }
        else{
            qDebug() << "Unknown error while setting value for "<<cameraParameterName.c_str();
        }
    }
}

void SpinboxContainer::displayParamValue()
{
    if(this->getParameterAvailable())
    {
        // UI element
        this->uiElement->blockSignals(true);
        this->uiElement->setValue(paramValue);
        this->uiElement->blockSignals(false);


        // SLider
        this->intSlider->blockSignals(true);
        this->intSlider->setSliderPosition(paramValue);
        this->intSlider->blockSignals(false);
    }
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

IntSlider *SpinboxContainer::getIntSlider() const
{
    return intSlider;
}

void SpinboxContainer::setIntSlider(IntSlider *value)
{
    intSlider = value;
}

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

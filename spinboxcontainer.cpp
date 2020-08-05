#include "spinboxcontainer.h"

SpinboxContainer::SpinboxContainer(int defaultParameterValue, QString cameraParameterName, int minVal, int maxVal, ImageAcquisition *imageAcquisitionThread, QWidget *parent): QSpinBox(parent)
{
 this->defaultParameterValue = defaultParameterValue;
 this->uiDisplayName = cameraParameterName;
 this->cameraParameterName = cameraParameterName;
 this->minVal = minVal;
 this->maxVal = maxVal;
 qTreeWidgetItem = new QTreeWidgetItem(QStringList() << cameraParameterName);
 uiElement = new QSpinBox;
 uiElement->setRange(minVal, maxVal);
 this->imageAcquisitionThread = imageAcquisitionThread;
 display();
}

QString SpinboxContainer::getCameraParameterName() const
{
    return cameraParameterName;
}

void SpinboxContainer::setCameraParameterName(const QString &value)
{
    cameraParameterName = value;
}

QString SpinboxContainer::getUiDisplayName() const
{
    return uiDisplayName;
}

void SpinboxContainer::setUiDisplayName(const QString &value)
{
    uiDisplayName = value;
}

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


int SpinboxContainer::getDefaultParameterValue() const
{
    return defaultParameterValue;
}

void SpinboxContainer::setDefaultParameterValue(int value)
{
    defaultParameterValue = value;
}

ImageAcquisition *SpinboxContainer::getImageAcquisitionThread() const
{
    return imageAcquisitionThread;
}

void SpinboxContainer::setImageAcquisitionThread(ImageAcquisition *value)
{
    imageAcquisitionThread = value;
}

void SpinboxContainer::update()
{
    val = this->imageAcquisitionThread->getValueForParam(cameraParameterName.toUtf8().constData());
    this->imageAcquisitionThread->getCameraControls().setExposureTime(val.D());
}

void SpinboxContainer::display()
{
    this->uiElement->setValue(defaultParameterValue);
}

void SpinboxContainer::setValueInHardware(int paramValue)
{
    this->imageAcquisitionThread->setValueForParam(cameraParameterName.toUtf8().constData(),paramValue);
}

void SpinboxContainer::customConnect()
{
    connect(uiElement, qOverloadInt(&QSpinBox::valueChanged),[=]{
        this->imageAcquisitionThread->getCameraControls().setExposureTime(uiElement->value());
        setValueInHardware(uiElement->value());
        update();
        display();
    });
}

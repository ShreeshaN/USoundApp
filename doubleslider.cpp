#include "doubleslider.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

//void DoubleSlider::notifyValueChanged(int value) {
//    double doubleValue = value / multiplicationFactor;
//    emit doubleValueChanged(doubleValue);
//}


//void DoubleSlider::setDoubleValue(double value){
//    _sleep(100);
//    this->blockSignals(true);
//    this->setSliderPosition(value*multiplicationFactor);
//    this->blockSignals(false);
//}

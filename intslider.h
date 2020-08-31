#ifndef INTSLIDER_H
#define INTSLIDER_H

#endif // INTSLIDER_H

#include <QSlider>
class IntSlider : public QSlider {
    Q_OBJECT

public:
    IntSlider(int initialValue, int minVal, int maxVal, int step, Qt::Orientation orientation, QWidget *parent = 0) : QSlider(orientation, parent) {
        this->setFocusPolicy(Qt::StrongFocus);
        this->setTickPosition(QSlider::TicksAbove);
        this->setTickInterval(0);
        this->setSingleStep(step);
        this->setRange(minVal,maxVal);
        this->setSliderPosition(initialValue);
    }
};

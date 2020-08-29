#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H
#include <QSlider>
#include <QDebug>

#endif // DOUBLESLIDER_H

class DoubleSlider : public QSlider {
    Q_OBJECT

    double multiplicationFactor=10.0;

public:
    DoubleSlider(double initialValue, int minVal, int maxVal, int step, Qt::Orientation orientation, QWidget *parent = 0) : QSlider(orientation, parent) {
        this->setFocusPolicy(Qt::StrongFocus);
        this->setTickPosition(QSlider::TicksAbove);
        this->setTickInterval(0);
        this->setSingleStep(step);
        this->setRange(minVal,maxVal);
        this->setSliderPosition(initialValue);

        connect(this, SIGNAL(valueChanged(int)),
            this, SLOT(notifyValueChanged(int)));
        connect(this, SIGNAL(doubleValueChanged(double)),
                this, SLOT(setDoubleValue(double)));

    }

signals:
    void doubleValueChanged(double value);
    void setValue(double value);

public slots:
    void notifyValueChanged(int value) {
        double doubleValue = value / multiplicationFactor;
        emit doubleValueChanged(doubleValue);
    }


    void setDoubleValue(double value){
        this->blockSignals(true);
        this->setSliderPosition(value*multiplicationFactor);
        this->blockSignals(false);
    }
};

#include "parametercontainer.h"

ParameterContainer::ParameterContainer()
{

}

bool ParameterContainer::getParameterAvailable() const
{
    return parameterAvailable;
}

void ParameterContainer::setParameterAvailable(bool value)
{
    parameterAvailable = value;
}

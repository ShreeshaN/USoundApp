#ifndef DEFAULTS_H
#define DEFAULTS_H
#include <string>


struct CameraControlDefaults{
    std::string RESOLUTION="";

    // exposure controls
    float EXPOSURETIME = 0.0;
    float ANALOGGAIN = 0.0;
    float EXPOSURETARGET = 0.0;  // not sure about this implementation
    bool AUTOEXPOSURE=false;

    // color appearance parameters
    float HUE = 0.0;
    float SATURATION = 0.0;
    float BRIGHTNESS = 0.0;
    float CONTRAST = 0.0;
    float GAMMA = 0.0;
    int FRAMERATE=60;
    bool MONOCHROME=true; // not sure about this implementation
    bool RGB=false; // not sure about this implementation
};


#endif // DEFAULTS_H

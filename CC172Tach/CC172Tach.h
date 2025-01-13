#pragma once

#include "Arduino.h"
#include <TFT_eSPI.h>


class CC172Tach
{
public:
    CC172Tach();
    void begin();
    void attach();
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();

private:
    bool    _initialised;
    int _curRpm = 0;
    double _curHours = 0;
    bool updateRPM();
    bool updateEngineHours();
};
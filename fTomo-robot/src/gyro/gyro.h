#ifndef _H_GYRO_
#define _H_GYRO_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class Gyro {
public:
    Gyro();
    bool init();
    float getAngle();
private:
    Adafruit_BNO055 sensor;
};

#endif


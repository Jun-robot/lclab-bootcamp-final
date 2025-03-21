#include "gyro.h"

Gyro::Gyro() : sensor(55, 0x28) {
}

bool Gyro::init() {
  if (!sensor.begin()) {
    return false;
  }
  return true;
}

float Gyro::getAngle() {
  imu::Vector<3> euler = sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
  float angle = euler.x();
  if (angle >= 180) { 
    angle -= 360; 
  }
  return angle;
}

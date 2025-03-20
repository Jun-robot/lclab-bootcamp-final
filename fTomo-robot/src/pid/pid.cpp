#include "pid/pid.h"
#include <Arduino.h>

PID::PID(float target, float kp, float ki, float kd)
    : target(target), Kp(kp), Ki(ki), Kd(kd), integral(0), previousError(0)
{

}

float PID::update(float currentValue, float dt) {
  float error = currentValue - target;
  // エラーの符号が変わった場合は積分値をリセット
  if (error * previousError < 0) {
      integral = 0;
  }
  integral += error * dt;
  float derivative = (error - previousError) / dt;
  float output = Kp * error + Ki * integral + Kd * derivative;
  previousError = error;
  return output;
}

void PID::setTarget(float newTarget) {
    target = newTarget;
    integral = 0;        // 積分値リセット
    previousError = 0;   // 前回の誤差リセット
}

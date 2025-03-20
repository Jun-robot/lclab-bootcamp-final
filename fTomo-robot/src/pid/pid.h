#ifndef _PID_H_
#define _PID_H_

#include <math.h>

class PID {
  public:
    PID(float target, float kp, float ki, float kd);
    float update(float currentValue, float dt);
    void setTarget(float newTarget);  // target更新用関数

  private:
    float target;
    float Kp, Ki, Kd;
    float integral;
    float previousError;
};

#endif // PID_H

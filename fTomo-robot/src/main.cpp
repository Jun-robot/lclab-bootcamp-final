#include <Arduino.h>

// Gyro
#include <Wire.h>
#include <Adafruit_BNO055.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// Motor
#include "ctromni/ctromni.h"
#include "drvmtr/drvmtr.h"

DRVMTR motorA(10, 11);
DRVMTR motorB(12, 13);
DRVMTR motorC(18, 19);
DRVMTR motorD(16, 17);

CTROMNI omni(&motorA, &motorB, &motorC, &motorD);

// Switch
const int switchPin = 14;

// PID制御用グローバル変数
float targetAngle = 0.0;
float Kp = 3.0, Ki = 0.1, Kd = 0.05;
float integral = 0.0, previousError = 0.0;
unsigned long lastTime = 0;

// --------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Gyro
  while(!bno.begin()){
    Serial.println("Could not find a valid BNO055 sensor, check wiring, address, sensor ID");
    delay(500);
  }

  omni.init();
  omni.setInstallationAngles(45, 135, 225, 315);
  lastTime = millis(); // PID用の初期時間
}

void loop() {

  // Gyro
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  if(euler.x() >= 180) { euler.x() -= 360; }

  Serial.println(euler.x());

  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // dtを秒単位で計算
  lastTime = currentTime;

  // PID計算
  float error = euler.x() - targetAngle;
  if (abs(error) < 5.0) { error = 0; } // ±3度未満を0として処理
  integral += error * dt;
  float derivative = (error - previousError) / dt;
  float pidOutput;
  if(error == 0) {
    pidOutput = 0;
  } else {
    pidOutput = Kp * error + Ki * integral + Kd * derivative;
  }
  previousError = error;

  if(digitalRead(switchPin) == HIGH){
    omni.cal(100, 0 - euler.x(), pidOutput);
  } else {
    omni.cal(0, 0, pidOutput);
  }
}

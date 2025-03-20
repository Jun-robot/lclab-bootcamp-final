#include <Arduino.h>

// Gyro
#include "gyro/gyro.h"   // 旧ヘッダとオブジェクトの削除

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

// Gyroオブジェクト生成
Gyro gyro;

// --------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Gyro初期化処理
  while(!gyro.init()){
    Serial.println("Gyro initialization failed, check wiring, address, sensor ID");
    delay(500);
  }

  omni.init();
  omni.setInstallationAngles(45, 135, 225, 315);
  lastTime = millis(); // PID用の初期時間
}

void loop() {
  // Gyroから角度取得
  float currentAngle = gyro.getAngle();
  Serial.println(currentAngle);

  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // dtを秒単位で計算
  lastTime = currentTime;

  // PID計算
  float error = currentAngle - targetAngle;
  if (abs(error) < 5.0) { error = 0; } // ±3度未満は0とする
  integral += error * dt;
  float derivative = (error - previousError) / dt;
  float pidOutput = (error == 0) ? 0 : Kp * error + Ki * integral + Kd * derivative;
  previousError = error;

  if(digitalRead(switchPin) == HIGH){
    omni.cal(100, -currentAngle, pidOutput);
  } else {
    omni.cal(0, 0, pidOutput);
  }
}

#include <Arduino.h>

// ----------Motor----------
#include "ctromni/ctromni.h"
#include "drvmtr/drvmtr.h"
DRVMTR motorA(10, 11);
DRVMTR motorB(12, 13);
DRVMTR motorC(18, 19);
DRVMTR motorD(16, 17);

CTROMNI omni(&motorA, &motorB, &motorC, &motorD);

// ----------Switch----------
const int switchPin = 14;

// ----------PID----------
#include "pid/pid.h"
PID pid(0.0, 3.0, 0.1, 0.05);
// PID用の時間計測用変数
unsigned long lastTime = 0;

// ----------Gyro----------
#include "gyro/gyro.h"
Gyro gyro;

// ----------espUdp----------
#include "espUdp/espUdp.h"
espUdp udp;

int omni_speed = 0;
int omni_angle = 0;

// --------------------------------------------------------------
void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  
  Serial.begin(115200);
  Serial.println("Start");

  // espUdpの初期化
  udp.begin();

  // Gyro初期化処理
  int n=0;
  while(!gyro.init()){
    n++; Serial.print(n);
    Serial.println(" Gyro initialization failed, check wiring, address, sensor ID");
    delay(500);
  }

  omni.init();
  omni.setInstallationAngles(45, 135, 225, 315);
  lastTime = millis(); // PID用の初期時間

  digitalWrite(21, HIGH);
}

void loop() {
  // Gyroから角度取得
  float currentAngle = gyro.getAngle();
  Serial.println(currentAngle);

  // PIDのdtを計算
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // dtを秒単位で計算
  lastTime = currentTime;

  // 姿勢制御のPID計算
  float pidGyro = pid.update(currentAngle, dt);
  //±3°未満の場合はPID出力を0とする
  if (abs(currentAngle - 0.0) < 5.0) {
    pidGyro = 0;
  }

  // espUdpから受信データを利用してモータ制御
  ESPData udpData;
  if (udp.readData(udpData)) {
    // omni.cal(udpData.angleX, udpData.angleY, pidGyro);
    omni_speed = udpData.speed;
    omni_angle = udpData.angleX*1.4;
  }

  // スイッチが押されている場合はモータ停止
  if (digitalRead(switchPin) == HIGH) {
    omni.cal(omni_speed, omni_angle, pidGyro);
  } else {
    omni.cal(0, 0, pidGyro);
  }
}

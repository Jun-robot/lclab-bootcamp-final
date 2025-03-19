#include <Arduino.h>
#include "ctromni/ctromni.h"
#include "drvmtr/drvmtr.h"

DRVMTR motorA(10, 11);
DRVMTR motorB(12, 13);
DRVMTR motorC(18, 19);
DRVMTR motorD(16, 17);

CTROMNI omni(&motorA, &motorB, &motorC, &motorD);

const int switchPin = 14;

void setup() {
  omni.init();
  omni.setInstallationAngles(45, 135, 225, 315);
}

void loop() {

  if(digitalRead(switchPin) == HIGH){
    omni.cal(100, 90, 0);
  }else{
    omni.cal(0, 0, 0);
  }
}

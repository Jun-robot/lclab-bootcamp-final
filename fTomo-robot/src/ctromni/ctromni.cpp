#include "./ctromni.h"
#include <math.h>

CTROMNI::CTROMNI(DRVMTR *mtr1, DRVMTR *mtr2, DRVMTR *mtr3, DRVMTR *mtr4){
  MTRS[0] = mtr1;
  MTRS[1] = mtr2;
  MTRS[2] = mtr3;
  MTRS[3] = mtr4;
  for(int i = 0; i < 4; i++){
    angles[i] = 0;
  }
}

void CTROMNI::init(){
  for(int i = 0; i < 4; i++){
    MTRS[i]->init();
  }
}

void CTROMNI::cal(int speed, int angle, int rot){
  int mtrSp[4];
  for(int i = 0; i < 4; i++){
    mtrSp[i] = speed * cos((angle - angles[i]) * PI / 180) + rot;
  }
  drive(mtrSp[0], mtrSp[1], mtrSp[2], mtrSp[3]);
}

void CTROMNI::drive(int mtrSp1, int mtrSp2, int mtrSp3, int mtrSp4){
  MTRS[0]->drive(mtrSp1);
  MTRS[1]->drive(mtrSp2);
  MTRS[2]->drive(mtrSp3);
  MTRS[3]->drive(mtrSp4);
}

void CTROMNI::setInstallationAngles(int a0, int a1, int a2, int a3){
  angles[0] = a0-90;
  angles[1] = a1-90;
  angles[2] = a2-90;
  angles[3] = a3-90;
}
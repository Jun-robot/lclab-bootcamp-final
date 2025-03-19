#ifndef _H_CTROMNI_
#define _H_CTROMNI_

#include <Arduino.h>
#include "../drvmtr/drvmtr.h"

class CTROMNI{
  private:
    DRVMTR *MTRS[4];
    int angles[4];
    void drive(int mtrSp1, int mtrSp2, int mtrSp3, int mtrSp4);
  public:
    CTROMNI(DRVMTR *mtr1, DRVMTR *mtr2, DRVMTR *mtr3, DRVMTR *mtr4);
    void init();
    void cal(int speed, int angle, int rot);
    void setInstallationAngles(int a0, int a1, int a2, int a3);
};

#endif
#ifndef _H_DRVMTR_
#define _H_DRVMTR_

#include <Arduino.h>

class DRVMTR{
  private:
    int PINA;
    int PINB;
    int mtrSp;
  public:
    DRVMTR(int ptr_A, int ptr_B);
    void init();
    void drive(int mtrSp);
};

#endif
#include "./drvmtr.h"

DRVMTR::DRVMTR(int ptr_A, int ptr_B){
  PINA = ptr_A;
  PINB = ptr_B;
  mtrSp = 0;
} 

void DRVMTR::init(){
  pinMode(PINA, OUTPUT);
  pinMode(PINB, OUTPUT);
}

void DRVMTR::drive(int mtrSp){
  if(mtrSp > 0){
    analogWrite(PINA, mtrSp);
    analogWrite(PINB, 0);
  } else if(mtrSp < 0){
    analogWrite(PINA, 0);
    analogWrite(PINB, abs(mtrSp));
  } else {
    analogWrite(PINA, 0);
    analogWrite(PINB, 0);
  }
}
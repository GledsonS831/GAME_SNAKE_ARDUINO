#include "LedControl.h"
#include "games/anaconda.h"
#include "control/displayLed.h"

void setup() {

  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);

  // seta o brilho para um valor mediano:
  //-------------------------------------
  lc.setIntensity(0,0);
  lc.setIntensity(1,0);
  lc.setIntensity(2,0);
  lc.setIntensity(3,0);

  // limpa o display:
  //-----------------
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
  Serial.begin(9600);
  randomSeed(analogRead(A5));
}
void loop(){
  snake();
}

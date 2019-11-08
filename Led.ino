#include "LedControl.h"
#include "binary.h"
 
LedControl lc = LedControl(12,11,10,1);
 
// tempo de delay entre as faces:
//-------------------------------
unsigned long delaytime=1000;
 
// face feliz:
//------------
 
byte ff[8]= {B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100};
 
// face neutra:
//-------------
byte fn[8]={B00111100, B01000010,B10100101,B10000001,B10111101,B10000001,B01000010,B00111100};
 
// face triste:
//-------------
byte ft[8]= {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01000010,B00111100};
 
void setup() {
  lc.shutdown(0,false);
 
  // seta o brilho para um valor mediano:
  //-------------------------------------
  lc.setIntensity(0,8);
 
  // limpa o display:
  //-----------------
  lc.clearDisplay(0);  
}
 
void desenhaFaces(){
 
  // Mostra a face feliz:
  //---------------------
  lc.setRow(0,7,B00000010);
  lc.setRow(1,7, 
  
  
  delay(delaytime);
}
 
void loop(){
  desenhaFaces();
}

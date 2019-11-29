#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 4);
int rdz = 0;
bool trap[8][8];
void setup() {
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.shutdown(2, false);
  lc.shutdown(3, false);

  lc.setIntensity(0,0);
  lc.setIntensity(1,0);
  lc.setIntensity(2,0);
  lc.setIntensity(3,0);

  Serial.begin(9600);
}

void seleciona_pos(){
  rdz = random(0, 4);
  trap[0][rdz] = 1;
  rdz = random(4, 8);
  trap[7][rdz] = 1;


  rdz = random(1, 4);
  trap[rdz][0] = 1;
  rdz = random(4, 8);
  trap[rdz][7] = 1;
}


void acender_armadilha(){
  seleciona_pos();
  for(byte k = 0; k < 4; k++){
    
    for(byte i = 0; i < 8; i++){
      lc.setLed(1, 0, i, trap[0][i]);
      lc.setLed(1, i, 0, trap[i][0]);
      lc.setLed(1, 7, i, trap[7][i]);
    }
    delay(500);
    limpar_matriz_led();
    delay(500);
  }
}

void limpar_matriz_game(){
  for(byte i = 0; i < 8; i++){
      for(byte j = 0; j < 8; j++){
        trap[i][j] = 0;
      }
   }
   limpar_matriz_led();
}

void limpar_matriz_led(){
  
  for(byte i = 0; i < 8; i++){
      for(byte j = 0; j < 8; j++){
        lc.setLed(1, i, j, false);
      }
   }
 
}

void atirar(){
  for(byte i = 0; i < 8; i++){
      if(trap[0][i]){
        for(byte j = 0; j < 8; j++){
          lc.setLed(1, j, i, true);
        }
      }

      if(trap[i][0]){
        for(byte j = 0; j < 8; j++){
          lc.setLed(1, i, j, true);
        }
      }
      
      if(trap[7][i]){
        for(byte j = 0; j < 8; j++){
          lc.setLed(1, j, i, true);
        }
      }
   }
   delay(500);
}
void loop() {
 acender_armadilha();
 delay(600);
 atirar();
 delay(1000);
 limpar_matriz_game();
 delay(2000);
}

#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 4);

int time = 0;
struct dinossauro{
  int pos_atual[3];
  byte pos_obstaculos[8];
  byte val_obstaculos[8];
  byte quant_obstaculos;
  int pontos;
  int nivel;
};
int ler_analogico_x (){
  int x;
  x = analogRead(A1);
  return x;
}
int ler_analogico_y (){
  int y;
  y = analogRead(A0);
  return y;
}
void step_led (int* p1, int* p2, int backup_p1, int backup_p2, bool manter_anterior){
  int tempo = 100;

  if (*p1 > tempo){
    *p1 = 1;
    *p2 = 0;
  }
  else if (*p1 < -tempo){
    *p1 = -1;
    *p2 = 0;
  }
  else if (manter_anterior){
    *p1 = backup_p1;
    *p2 = backup_p2;
  }
  else {
    *p1 = 0;
    *p2 = 0;
  }
}
void ler_analogico (int* x, int* y, bool vertical, bool horizontal, bool manter_anterior){
  int backup_x = *x;
  int backup_y = *y;

    *x = ler_analogico_x();
    *x -= 512;
    *y = ler_analogico_y();
    *y -= 512;

  if (vertical && horizontal){

    if (abs(*y) >= abs(*x)){
      step_led (y, x, backup_y, backup_x, manter_anterior);
    }

    else {
      step_led (x, y, backup_x, backup_y, manter_anterior);
    }
  }
  else if (vertical){
    *x = 0;
    step_led (y, x, backup_y, backup_x, manter_anterior);
  }
  else {
    *y = 0;
    step_led (x, y, backup_x, backup_y, manter_anterior);
  }
}

void andar_dinossauro (int x, int y, int* pos_z, int* pos_x, int* pos_y){

  *pos_x += x;
  *pos_y += y;
  if (*pos_y > 7){
    *pos_y = 0;
    --*pos_z;
    if (*pos_z < 1) *pos_z = 3;
  }
  else if (*pos_y < 0){
    *pos_y = 7;
    ++*pos_z;
    if (*pos_z > 3) *pos_z = 1;
  }
  if (*pos_x > 7) *pos_x = 0;
  else if (*pos_x < 0) *pos_x = 7;
}

void clearDisplay(){
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
}

void generate_new_barreira (struct dinossauro* dino){
  time = random(4,8);

  byte x;
  dino->pos_obstaculos[dino->quant_obstaculos] = 0;

  if (random(2)){
    dino->val_obstaculos[dino->quant_obstaculos] = 0b00000010;
  }
  else{
    byte walls[] = {0b00000001, 0b00000011, 0b00000111};
    dino->val_obstaculos[dino->quant_obstaculos] = walls[random(3)];
  }
  ++dino -> quant_obstaculos;
}

void andar_barreiras (struct dinossauro* dino){
  for (int i = 0; i < dino->quant_obstaculos; i++){
    lc.setColumn(lc->pos_obstaculos[i]/8, lc->pos_obstaculos[i]%8, 0b00000000);
    ++lc->pos_obstaculos[i];
    lc.setColumn(lc->pos_obstaculos[i]/8, lc->pos_obstaculos[i]%8, lc->val_obstaculos[i]);
  }
}
void paranaue (struct dinossauro *dino){
  if (time == 0) generate_new_barreira (dino);
  andar_barreiras(dino);
  time--;
}

bool colision_dinossauro(int x, int y, struct dinossauro* dino){
  if (x == 0 && y == 1){
    dino->pos_atual[1] = -1;
    lc.setLed(2, 1, 4, false);
  }
  else if (x ==0 && y == -1){
      do{
        lc.setLed(2, dino->pos_atual[1], 4, false);
        ++dino->pos_atual[1];
        lc.setLed(2, dino->pos_atual[1]+1, 4, true);
        delay(150);
      }while(dino->pos_atual[1] < 4);

      while(dino->pos_atual[1] > 0){
        lc.setLed(2, dino->pos_atual[1]+1, 4, false);
        --dino->pos_atual[1];
        lc.setLed(2, dino->pos_atual[1], 4, true);
        delay(150);
      }
  }
  else{
    dino->pos_atual[1] = 0;
    lc.setLed(2, 1, 4, true);
    lc.setLed(2, 0, 4, true);
  }
  return true;
}

void next_level_dinossauro(struct dinossauro* dino){
    clearDisplay();
    dino->pos_atual[0] = 2;
    dino->pos_atual[1] = 0;
    dino->pos_atual[2] = 4;

    ++dino->nivel;
}
struct dinossauro* init_dinossauro(){
  struct dinossauro* dino = (struct dinossauro*)malloc(sizeof(struct dinossauro));
  dino->nivel = 0;
  dino->pontos = 0;
  dino->quant_obstaculos = 0;
}

void dinossauro(){
  struct dinossauro* dino = init_dinossauro();

  for (int i = 0; i < 3; i ++){
    int x = 0;
    int y = 0;

    do{
      ler_analogico(&x, &y, true, false, false);
    }while(colision_dinossauro(x, y, dino));
  }
}

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

  clearDisplay();

  Serial.begin(9600);
  randomSeed(analogRead(A3));
}
void loop(){
  //dinossauro();
  lc.setColumn(0,1,255);
}

#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 4);

struct anaconda{
  int pos_atual[3];
  int pos_final[3];
  int mapeamento[3][8][8];
  int tamanho;
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

bool andar_anaconda (int x, int y, struct anaconda* ana){
  lc.setLed(ana->pos_atual[0], ana->pos_atual[1], ana->pos_atual[2], false);
  ana->pos_atual[1] += x;
  ana->pos_atual[2] += y;
  if (ana->pos_atual[2] > 7){
    ana->pos_atual[2] = 0;
    ana->pos_atual[0]--;
    if (ana->pos_atual[0] < 1) ana->pos_atual[0] = 3;
  }
  else if (ana->pos_atual[2] < 0){
    ana->pos_atual[2] = 7;
    ana->pos_atual[0]++;
    if (ana->pos_atual[0] > 3) ana->pos_atual[0] = 1;
  }
  if (ana->pos_atual[1] > 7) ana->pos_atual[1] = 0;
  else if (ana->pos_atual[1] < 0) ana->pos_atual[1] = 7;

  lc.setLed(ana->pos_atual[0], ana->pos_atual[1], ana->pos_atual[2], true);
  delay(500);
  return true;
}

void zerarMatriz (int matriz [3][8][8]){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 8; j++){
      for (int k = 0; k < 8; k++){
        matriz[i][j][k] = 0;
      }
    }
  }
}
void next_level_anaconda(struct anaconda* ana){
  ana->pos_atual[0] = 3;
  ana->pos_atual[1] = 0;
  ana->pos_atual[2] = 0;

  ana->pos_final[0] = 3;
  ana->pos_final[1] = 0;
  ana->pos_final[2] = 0;

  ana->tamanho = 1;
  ana->nivel++;

  zerarMatriz(ana->mapeamento);
}
struct anaconda* init_anaconda(){
  struct anaconda* ana = (struct anaconda*)malloc(sizeof(struct anaconda));

  ana->nivel = 0;
  next_level_anaconda(ana);
  return ana;
}
void snake (){
  int x = 0;
  int y = 0;
  struct anaconda* ana = init_anaconda();

  do{
    if (ana->tamanho == 9){
      next_level_anaconda(ana);
      x = 0;
      y = 0;
    }
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);
    ler_analogico(&x, &y, true, true, true);
  }while(andar_anaconda (x, y, ana));
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

  // limpa o display:
  //-----------------
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
  Serial.begin(9600);
}
void loop(){
  snake();
}

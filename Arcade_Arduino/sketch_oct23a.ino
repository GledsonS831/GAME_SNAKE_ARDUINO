#include "LedControl.h"
#include "binary.h"

short tempo = 200;
LedControl lc = LedControl(12,11,10,4);
struct anaconda{
  short pos_atual[3];
  short pos_final[3];
  bool campo[3][8][8];
  short tamanho;
  short pontos;
  short nivel;
};

short ler_analogico_x (){
  short x = analogRead(A0);
  return x;
}
short ler_analogico_y (){
  short y = analogRead(A1);
  return y;
}
void ler_analogico (short *x, short *y, bool v, bool h, bool manter){
  short backup_x = *x;
  short backup_y = *y;
  if (v && h){
    *x = ler_analogico_x();
    *x -= 512;
    *y = ler_analogico_y();
    *y -= 512;

    if (*y > tempo && *x > tempo){ //cima * direita
      if (*x > *y){ //direita
        *x = 1;
        *y = 0;
      }
      else{        //cima
        *x = 0;
        *y = 1;
      }
    }
    else if (*y > tempo && *x < -tempo){ //cima * esquerda
      if (*x * -1 > *y){ //esquerda
        *x = -1;
        *y = 0;
      }
      else{        //cima
        *x = 0;
        *y = 1;
      }
    }
    else if (*y < -tempo && *x > tempo){ //baixo * direita
      if (*x > *y * -1){ //direita
        *x = 1;
        *y = 0;
      }
      else{        //baixo
        *x = 0;
        *y = -1;
      }
    }
    else if (*y < -tempo && *x < -tempo){ //baixo * esquerda
      if (*x * -1 > *y * -1){ //esquerda
        *x = -1;
        *y = 0;
      }
      else{        //baixo
        *x = 0;
        *y = -1;
      }
    }
    else if (!manter) {
      *x = 0;
      *y = 0;
    }
    else{
      *x = backup_x;
      *y = backup_y;
    }
  }
  else if (v){
    *x = 0;
    *y = ler_analogico_y();
    *y -= 512;
    if (*y > tempo) *y = 1;
    else if (*y < -tempo) *y = -1;
    else if (!manter) *y = 0;
    else *y = backup_y;
  }
  else {
    *x = ler_analogico_x();
    *x -= 512;
    *y = 0;
    if (*x > tempo) *x = 1;
    else if (*x < -tempo) *x = -1;
    else if (!manter) *x = 0;
    else *x = backup_x;
  }
}
void andar_anaconda(short x, short y, struct anaconda* ana){

  ana->pos_atual[1] += x;
  ana->pos_atual[2] += y;
  lc.setLed(ana->pos_atual[0], ana->pos_atual[1], ana->pos_atual[2], true);

    if (ana->pos_atual[2] > 7){
      ana->pos_atual[2] = 0;
      ana->pos_atual[0]--;
      if (ana->pos_atual[0] == 0) ana->pos_atual[0] = 3;
    }
    else if (ana->pos_atual[2] < 7){
      ana->pos_atual[2] = 7;
      ana->pos_atual[0]++;
      if (ana->pos_atual[0] == 4) ana->pos_atual[0] = 1;
    }
    if (ana->pos_atual[1] > 7) ana->pos_atual[1] = 0;
    else if (ana->pos_atual[1] < 0) ana->pos_atual[1] = 7;

  delay(1000);
}
void zerarMatriz (bool matriz [3][8][8]){
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 8; j++){
      for (int k = 0; k < 8; k++){
        matriz[i][j][k] = false;
      }
    }
  }
  matriz[0][0][0] = true;
}
struct anaconda* init_anaconda(){
  struct anaconda* ana;

  ana->pos_atual[0] = 3;
  ana->pos_atual[1] = 0;
  ana->pos_atual[2] = 0;

  ana->pos_final[0] = 3;
  ana->pos_final[1] = 0;
  ana->pos_final[2] = 0;

  ana->pontos = 0;
  ana->tamanho = 0;
  ana->nivel = 0;

  //zerarMatriz(ana->campo);

  return ana;
}
void snake (){
  short x = 0;
  short y = 0;
  struct anaconda* ana = init_anaconda();

// lc.setLed(3, 3, 3, true);
  while (true){
    ler_analogico(&x, &y, true, true, true);
    Serial.println(y);
    andar_anaconda (x, y, ana);
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

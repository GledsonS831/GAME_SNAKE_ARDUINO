#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 4);

struct Personagem{
  int pos_x;
  int pos_y;
  int vida;
};

struct Campo{
  int pos[8][8];
};

struct Personagem *soldado = new Personagem;
struct Campo *espaco = new Campo;
int campo_canhao[8][2] = {{6, 0}, {5, 1}, {7, 2},{4, 3}, {4, 4}, {7, 5}, {5, 6},{6, 7}};
int armazena_canhao[8] = {};

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

void desenhaNave(){
  for (int i = 0; i < 8; i++){
    lc.setRow(3, i, B11111111);
  }
  
  lc.setRow(2, 0, B11111100);
  lc.setRow(2, 1, B11111000);
  lc.setRow(2, 2, B11111110);
  lc.setRow(2, 3, B11110000);
  lc.setRow(2, 4, B11110000);
  lc.setRow(2, 5, B11111110);
  lc.setRow(2, 6, B11111000);
  lc.setRow(2, 7, B11111100);
}

void zerarMatriz(){
  for(byte i = 0; i < 8; i++){
    for(byte j = 0; j < 8; j++){
      espaco->pos[i][j] = 0;
    }
  }
}


void setup(){
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);
  
  lc.setIntensity(0,0);
  lc.setIntensity(1,0);
  lc.setIntensity(2,0);
  lc.setIntensity(3,0);

  desenhaNave();
  zerarMatriz();
  randomSeed(analogRead(3));
  
  for(byte i = 0; i < 2; i++){
      armazena_canhao[i] = seleciona_tiro_esquerda();
    }
    for(byte i = 0; i < 2; i++){
      armazena_canhao[i] = seleciona_tiro_direita();
    }

  Serial.begin(9600);

}

int seleciona_tiro_esquerda(){
  int x = random(0, 4);
  while(armazena_canhao[x] != 1){
    x = random(0, 4);
  }
  return x;
}

int seleciona_tiro_direita(){
  int x = random(4, 8);
  while(armazena_canhao[x] != 1){
    x = random(4, 8);
  }
  return x;
}

void preenche_cachao(int pos){
  armazena_canhao[pos] = 1;
}

void atirar(){
  
}
void loop(){
  atirar();
}



//#include <LiquidCrystal_I2C.h>
#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 4);
//LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

struct anaconda{
  int pos_atual[3];
  int pos_final[3];
  int mapeamento[4][8][8];
  int pos_comida[3];
  int tamanho;
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

void clearDisplay(){
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
}

void select_pos_food (struct anaconda* ana){
  int x, y, z;

  do{
    x = random(8);
    y = random(8);
    z = random(1, 4);
  }while(ana->mapeamento[z][x][y] != 0);

  ana->pos_comida[0] = z;
  ana->pos_comida[1] = x;
  ana->pos_comida[2] = y;

  ana->mapeamento[z][x][y] = 5;
}
void blink_food (struct anaconda* ana){
  bool estado = true;

  for (int i = 0; i < 2; i++){
    lc.setLed(ana->pos_comida[0], ana->pos_comida[1], ana->pos_comida[2], estado);
    estado = !estado;
    delay (10);
  }
}

void andar_anaconda (int x, int y, int* pos_z, int* pos_x, int* pos_y){

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
void andar_bunda_anaconda (struct anaconda* ana){

  lc.setLed(ana->pos_final[0], ana->pos_final[1], ana->pos_final[2], false);

  if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 1){
    ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] = 0;
    andar_anaconda (1, 0, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  }
  else if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 2){
    ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] = 0;
    andar_anaconda (-1, 0, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  }
  else if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 3){
    ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] = 0;
    andar_anaconda (0, 1, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  }
  else if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 4){
    ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] = 0;
    andar_anaconda (0, -1, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  }
}

void andar_cabeca_anaconda (int x, int y, struct anaconda* ana){

  if (x == 1 && y == 0) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 1;
  else if (x == -1 && y == 0) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 2;
  else if (x == 0 && y == 1) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 3;
  else if (x == 0 && y == -1) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 4;

  andar_anaconda(x, y, &ana->pos_atual[0], &ana->pos_atual[1], &ana->pos_atual[2]);

  lc.setLed(ana->pos_atual[0], ana->pos_atual[1], ana->pos_atual[2], true);

}

bool colision_anaconda(int x, int y, struct anaconda* ana){//----------------------------------
  andar_cabeca_anaconda(x, y, ana);
  if (ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] == 5){
    ++ana->tamanho;
    if (ana->pontos < 10000) ana->pontos += 100;
    select_pos_food(ana);
  }
  else{
    if (ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] > 0){
      delay(1000);
      return false;
    }
    andar_bunda_anaconda(ana);
  }

  blink_food(ana);
  if (ana->nivel * 50 == 150) delay(50);
  else delay(200 - (ana->nivel * 10));
  return true;
}
void zerarMatriz (int matriz [4][8][8]){
  for (int i = 1; i < 4; i++){
    for (int j = 0; j < 8; j++){
      for (int k = 0; k < 8; k++){
        matriz[i][j][k] = 0;
      }
    }
  }
  matriz[3][0][0] = -1;
}
void next_level_anaconda(struct anaconda* ana){
  zerarMatriz(ana->mapeamento);
  clearDisplay();
  ana->pos_atual[0] = 3;
  ana->pos_atual[1] = 0;
  ana->pos_atual[2] = 0;

  ana->pos_final[0] = 3;
  ana->pos_final[1] = 0;
  ana->pos_final[2] = 0;

  select_pos_food(ana);

  ana->tamanho = 1;
  ++ana->nivel;
}

struct anaconda* init_anaconda(){
  struct anaconda* ana = (struct anaconda*)malloc(sizeof(struct anaconda));

  ana->nivel = 0;
  ana->pontos = 0;
  next_level_anaconda(ana);
  return ana;
}
void snake (){
  int x = 0;
  int y = 0;
  struct anaconda* ana = init_anaconda();

  for (int i = 0; i < 3; i++){
    do{
      if (ana->tamanho == 8){
        next_level_anaconda(ana);
        x = 0;
        y = 0;
      }
      ler_analogico(&x, &y, true, true, true);
      Serial.println(ana->pontos);
    }while(colision_anaconda(x, y, ana));
    --ana->nivel;
    ana->tamanho = 8;
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

  //lcd.begin (16,2);
  //lcd.setBacklight(HIGH);

  // limpa o display:
  //-----------------
  //clearDisplay();

  Serial.begin(9600);
  randomSeed(analogRead(A3));
}
void loop(){
  snake();
  /*
  lcd.setCursor(0,0);
  lcd.print("--nome do jogo--");
  lcd.setCursor(0,1);
  lcd.print("R -----");
  lcd.setCursor(9, 1);
  lcd.print("P -----");
  */

}

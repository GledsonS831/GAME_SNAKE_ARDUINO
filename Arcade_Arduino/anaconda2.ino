#include <LiquidCrystal_I2C.h>
#include "LedControl.h"
#include <EEPROM.h>

LedControl lc = LedControl(12, 11, 10, 4);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

volatile bool pause = false;

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
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
}
void menu(){
  detachInterrupt(digitalPinToInterrupt(3));
  pause = !pause;
  delayMicroseconds(300000);
  attachInterrupt(digitalPinToInterrupt(3), menu, FALLING);
}

void mostar_vida(int vida){
  lc.clearDisplay(0);

  if (vida == 3){
    lc.setRow(0, 2, 0b11111110);
    lc.setColumn(0, 0, 0b00111100);
    lc.setColumn(0, 3, 0b00111100);
    lc.setColumn(0, 6, 0b00111100);
  }

  else if (vida == 2){
    lc.setRow(0, 2, 0b11110000);
    lc.setRow(0, 5, 0b00011110);
    lc.setColumn(0, 0, 0b00111100);
    lc.setColumn(0, 3, 0b00111100);
    lc.setColumn(0, 6, 0b00111100);
  }

  else if (vida == 1){
    lc.setRow(0, 5, 0b11111110);
  }
  else{
    lc.setColumn(0, 0, 0b00111100);
    lc.setRow(0, 2, 0b11111110);
    lc.setRow(0, 5, 0b11111110);
    lc.setColumn(0, 6, 0b00111100);
  }
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

void atualiza_pontuacao (int val){
  lcd.setCursor(11, 1);
  lcd.print(val);

  if (val > (EEPROM.read(1) << 8) + EEPROM.read(0)){
    lcd.setCursor(2, 1);
    lcd.print(val);
  }
}
void init_lcd (){
  lcd.setCursor(0,0);
  lcd.print("ANACONDA'S GAME!");

  lcd.setCursor(0,1);
  lcd.print("R ");
  lcd.print( (EEPROM.read(1) << 8) + EEPROM.read(0));

  lcd.setCursor(9, 1);
  lcd.print("P ");
  lcd.print(0);
}
void reset_recorde(){
  EEPROM.update(0, 0);
  EEPROM.update(1, 0);
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
    if (ana->pontos < 10000){
      ana->pontos += 100;
      atualiza_pontuacao(ana->pontos);
    }
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
  if (ana->nivel * 10 == 150) delay(50);
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

  for (int i = 3; i > 0; i--){
    mostar_vida(i);
    do{
      while(pause);
      if (ana->tamanho == 8){
        next_level_anaconda(ana);
        x = 0;
        y = 0;
        delay(500);
      }
      ler_analogico(&x, &y, true, true, true);

    }while(colision_anaconda(x, y, ana));
    --ana->nivel;
    ana->tamanho = 8;
  }
  mostar_vida(0);
  if (ana->pontos > (EEPROM.read(1) << 8) + EEPROM.read(0)){
    EEPROM.update(1, ana->pontos >> 8);
    EEPROM.update(0, ana->pontos);
  }
  free(ana);

  delay(2000);
}

void setup() {

  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);

  lc.setIntensity(0,0);
  lc.setIntensity(1,0);
  lc.setIntensity(2,0);
  lc.setIntensity(3,0);

  lcd.begin (16,2);
  lcd.setBacklight(HIGH);
  //reset_recorde();
  init_lcd();

  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(3), menu, RISING);

  Serial.begin(9600);
  randomSeed(analogRead(A3));
}

void loop(){
  snake();
  //Serial.println(digitalRead(3));
}

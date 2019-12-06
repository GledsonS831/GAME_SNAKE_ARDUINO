#include "food_anaconda.h"

void andar_anaconda (int x, int y, int* pos_z, int* pos_x, int* pos_y){
  *pos_x += x;
  *pos_y += y;
  if (*pos_y > 7){
    *pos_y = 0;
    *pos_z--;
    if (*pos_z < 1) *pos_z = 3;
  }
  else if (*pos_y < 0){
    *pos_y = 7;
    *pos_z++;
    if (*pos_z > 3) *pos_z = 1;
  }
  if (*pos_x > 7) *pos_x = 0;
  else if (*pos_x < 0) *pos_x = 7;
}
void andar_bunda_anaconda (struct anaconda* ana){
  LedControl lc = LedControl(12, 11, 10, 4);
  ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] = 0;
  lc.setLed(ana->pos_final[0], ana->pos_final[1], ana->pos_final[2], false);

  if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 1)
    andar_anaconda (1, 0, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  else if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 2)
    andar_anaconda (-1, 0, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  else if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 3)
    andar_anaconda (0, 1, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
  else if (ana->mapeamento[ana->pos_final[0]][ana->pos_final[1]][ana->pos_final[2]] == 4)
    andar_anaconda (0, -1, &ana->pos_final[0], &ana->pos_final[1], &ana->pos_final[2]);
}

void andar_cabeca_anaconda (int x, int y, struct anaconda* ana){
  LedControl lc = LedControl(12, 11, 10, 4);
  if (x == 1 && y == 0) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 1; // por condição de não ir ao lado contrario
  else if (x == -1 && y == 0) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 2;
  else if (x == 0 && y == 1) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 3;
  else if (x == 0 && y == -1) ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] = 4;
  andar_anaconda(x, y, &ana->pos_atual[0], &ana->pos_atual[1], &ana->pos_atual[2]);
  lc.setLed(ana->pos_atual[0], ana->pos_atual[1], ana->pos_atual[2], true);
}
bool colision_anaconda(int x, int y, struct anaconda* ana){//----------------------------------
  andar_cabeca_anaconda(x, y, ana);
  if (ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] == 5){ // não uda o cinco
    ana->tamanho++;
    select_pos_food(ana);
  }
  else{
    andar_bunda_anaconda(ana);
    if (ana->mapeamento[ana->pos_atual[0]][ana->pos_atual[1]][ana->pos_atual[2]] != 0) return false;
  }

  blink_food(ana);
  delay(300);
  return true;
}

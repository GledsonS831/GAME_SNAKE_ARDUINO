#include "../control/structs_anaconda.h"

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
  bool estado = false;
  LedControl lc = LedControl(12, 11, 10, 4);
  for (int i = 0; i < 6; i++){
    lc.setLed(ana->pos_comida[0], ana->pos_comida[1], ana->pos_comida[2], estado);
    estado = !estado;
    delay (50);
  }
}

#include "../anaconda/andar_anaconda.h"
#include "../anaconda/config_anaconda.h"
#include "../control/analogico.h"

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
      Serial.print("-------------");
      x = 0;
      y = 0;
    }
    ler_analogico(&x, &y, true, true, true);

  }while(colision_anaconda(x, y, ana));
}

#include "LedControl.h"

LedControl lc = LedControl(12, 11, 10, 4);

struct Node{
    int val;
    struct Node* p_node;
};
struct lista{
    struct Node* p_node;
};
struct lista* criar_lista(){
    struct lista* l = (struct lista*)malloc(sizeof(struct lista));
    l->p_node = NULL;
}
void adiciona_lista(struct lista* l, int val){
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->val = val;
    if (l->p_node == NULL) l->p_node = n;
    else{
        struct Node* n1 = l->p_node;
        while (n1->p_node != NULL){
            n1 = n1->p_node;
        }
        n1->p_node = n;
    }
}
void remove_lista (struct lista* l){
    struct Node* n = l->p_node;
    l->p_node = n->p_node;
    free(n);
}

int time = 0;

struct dinossauro{
  int pos_atual[3];
  struct lista* pos_obstaculos;
  struct lista* val_obstaculos;
  int quant_obstaculos;
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
  adiciona_lista(dino->pos_obstaculos, 0);

  if (random(2)){
    adiciona_lista(dino->val_obstaculos, 64); //0b01000000
  }
  else{
    byte walls[] = {128, 192, 224};  //0b10000000, 0b11000000, 0b11100000
    adiciona_lista(dino->val_obstaculos, walls[random(3)]);
  }
  ++dino -> quant_obstaculos;
}

int conversor (int v){
  if (v == 0) return 3;
  else if (v == 1) return 2;
  return 1;
}
void andar_barreiras (struct dinossauro* dino){
  struct Node* pos = dino->pos_obstaculos->p_node;
  struct Node* val = dino->val_obstaculos->p_node;

  for (int i = 0; i < dino->quant_obstaculos; i++){
    lc.setColumn(conversor(pos->val/8), pos->val%8, 0); //0b00000000
    ++pos->val;
    lc.setColumn(conversor(pos->val/8), pos->val%8, val->val);

    pos = pos->p_node;
    val = val->p_node;
  }
  if (dino->pos_obstaculos->p_node->val == 23){
    remove_lista(dino->pos_obstaculos);
    remove_lista(dino->val_obstaculos);
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
    //paranaue(dino);
    delay(150);
  }
  else if (x ==0 && y == -1){
      do{
        lc.setLed(2, dino->pos_atual[1], 4, false);
        ++dino->pos_atual[1];
        lc.setLed(2, dino->pos_atual[1]+1, 4, true);
        //paranaue(dino);
        delay(150);
      }while(dino->pos_atual[1] < 4);

      while(dino->pos_atual[1] > 0){
        lc.setLed(2, dino->pos_atual[1]+1, 4, false);
        --dino->pos_atual[1];
        lc.setLed(2, dino->pos_atual[1], 4, true);
        //paranaue(dino);
        lc.setColumn(0,0,255);
        delay(150);
      }
  }
  else{
    dino->pos_atual[1] = 0;
    lc.setLed(2, 1, 4, true);
    lc.setLed(2, 0, 4, true);
    //paranaue(dino);
    delay(150);
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
  dino->pos_obstaculos = (struct lista*) malloc (sizeof(struct lista));
  dino->val_obstaculos = (struct lista*) malloc (sizeof(struct lista));
  dino->nivel = 0;
  dino->pontos = 0;
  dino->quant_obstaculos = 0;
  next_level_dinossauro(dino);
}

void dinossauro(){
  struct dinossauro* dino = init_dinossauro();

  for (int i = 0; i < 1; i ++){
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
  dinossauro();
  lc.setColumn(0, 0, 0b10000000);
}

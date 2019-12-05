#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 4);

bool campo[8][8];

struct nave{
  byte velocidade_tiro;
  byte velocidade_reload;
};

struct nave* init_nave(){
  struct nave* nave = (struct nave*)malloc(sizeof(struct nave));
  nave->velocidade_tiro = 1000;
  nave->velocidade_reload = 1500;
  return nave;
}

struct Node{
  byte pos_x;
  byte pos_y;
  struct Node *node_anterior;
  struct Node *node_proximo;
};
struct linked_list{
  byte lenght;
  struct Node* node;
  struct Node* last_node;
};

struct linked_list* create_linked_list(){
  struct linked_list* l = (struct linked_list*)malloc (sizeof(struct linked_list));
  l->lenght = 0;
  l->node = NULL;
  l->last_node = NULL;
}

void list_put_element(struct linked_list* l, byte x, byte y){
  struct Node* node = (struct Node*) malloc (sizeof(struct Node));
  node->pos_x = x;
  node->pos_y = y;
  node-> node_proximo = NULL;
  node->node_anterior = NULL;

  if (l->node == NULL){
    l->node = node;
    l->last_node = node;
  }
  else{
    node->node_proximo = l->node;
    node->node_anterior = l->last_node;
    l->last_node->node_proximo = node;
  }
}

void list_remove_element (struct linked_list* l, struct Node* node){
    if (node->node_proximo == NULL){
      l->node = NULL;
      l->last_node = NULL;
    }
    else{
      node->node_anterior->node_proximo = node->node_proximo;
    }

    free(node);
}

void reset (){
  asm volatile (" jmp 0");
}

void limpar_campo(){
  for(byte i = 0; i < 8; i++){
    for(byte j = 0; j < 8; j++){
      campo[i][j] = 0;
    }
  }
}
void setup() {
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.shutdown(2, false);
  lc.shutdown(3, false);

  lc.setIntensity(0,0);
  lc.setIntensity(1,0);
  lc.setIntensity(2,0);
  lc.setIntensity(3,0);
  randomSeed(analogRead(5));
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

  Serial.begin(9600);
}

void atirar(byte pos){
  for(byte i = 0; i < 8; i++){
    lc.setLed(1, pos, i, true);
    lc.setLed(1, pos, i, false);
    delay(400);
  }
}

void loop() {

// asm volatile (" jmp 0");
}

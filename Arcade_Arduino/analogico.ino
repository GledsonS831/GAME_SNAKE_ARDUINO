int *ler_analogico_x (){
  int* x = (int*)malloc(sizeof(int));
  *x = analogRead(A0);
  return x;
}

int *ler_analogico_y (){
  int* y = (int*)malloc(sizeof(int));
  *y = analogRead(A1);
  return y;
}

void funcao (int* p1, int* p2, int backup_p1, int backup_p2, int tempo, bool manter_anterior){
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
  int tempo = 100;
  
  if (vertical && horizontal){
    x = ler_analogico_x();
    *x -= 512;
    y = ler_analogico_y();
    *y -= 512;

    if (abs(*y) >= abs(*x)){
      funcao (y, x, backup_y, backup_x, tempo, manter_anterior);
    }
    
    else {
      funcao (x, y, backup_x, backup_y, tempo, manter_anterior);
    }
  }
  else if (vertical){
    *x = 0;
    funcao (y, x, backup_y, backup_x, tempo, manter_anterior);
  }
  else {
    x = ler_analogico_x();
    funcao (x, y, backup_x, backup_y, tempo, manter_anterior);
  }
                       Serial.print(*x);
  Serial.print(" ");
  Serial.println(*y);

}

void setup() {
  Serial.begin(9600);
}
void loop(){
  int *x;
  int *y;
  ler_analogico(x, y, true, true, true);

  delay(2000);
}

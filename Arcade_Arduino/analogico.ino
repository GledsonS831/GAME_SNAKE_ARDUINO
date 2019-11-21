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

void funcao (int* p1, int* p2, int backup_p1, int backup_p2, bool manter_anterior){
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
      funcao (y, x, backup_y, backup_x, manter_anterior);
    }

    else {
      funcao (x, y, backup_x, backup_y, manter_anterior);
    }
  }
  else if (vertical){
    *x = 0;
    funcao (y, x, backup_y, backup_x, manter_anterior);
  }
  else {
    *y = 0;
    funcao (x, y, backup_x, backup_y, manter_anterior);
  }
                       Serial.print(*x);
  Serial.print(" ");
  Serial.println(*y);

}

void setup() {
  Serial.begin(9600);
}
void loop(){
  int *x = (int*)malloc(sizeof(int));
  *x = 0;
  int *y = (int*)malloc(sizeof(int));
  *y = 0;
  ler_analogico(x, y, true, true, false);
Serial.print(*x);
Serial.print(" ");
Serial.println(*y);
  delay(2000);
}

void zerarMatriz (int matriz [4][8][8]){
  for (int i = 1; i < 4; i++){
    for (int j = 0; j < 8; j++){
      for (int k = 0; k < 8; k++){
        matriz[i][j][k] = 0;
      }
    }
  }
  matriz[3][0][0] = 5;
}
void next_level_anaconda(struct anaconda* ana){
  ana->pos_atual[0] = 3;
  ana->pos_atual[1] = 0;
  ana->pos_atual[2] = 0;

  ana->pos_final[0] = 3;
  ana->pos_final[1] = 0;
  ana->pos_final[2] = 0;

  ana->pos_comida[0] = 3;
  ana->pos_comida[1] = 0;
  ana->pos_comida[2] = 0;

  ana->tamanho = 0;
  ana->nivel++;

  zerarMatriz(ana->mapeamento);
}

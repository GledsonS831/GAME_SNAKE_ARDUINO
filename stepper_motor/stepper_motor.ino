/* Controle de motor de passo com DVR8825
   Autor: Gedeane Kenshima
   Data: 14/08/2017
*/
#include <Arduino.h>
#include "DRV8834.h"
#include "A4988.h"
#include "DRV8825.h"

// Quantidade de passos por volta completa 
#define MOTOR_STEPS 200

// Definição de pinos DIR e STEP
#define DIR 8
#define STEP 9

// botões esquerdo e direito
int botao_e = 5;
int botao_d = 4;


// Pinos de controle do DRV8825
   #define MODE0 10
   #define MODE1 11
   #define MODE2 12

// Instância da biblioteca para controle do Driver
   DRV8825 stepper(MOTOR_STEPS, DIR, STEP, MODE0, MODE1, MODE2);

void setup() {
    // Inicio da instância
    stepper.begin(120);
    // Definindo pinos de entrada PULLUP
    pinMode(botao_e,INPUT_PULLUP); 
    pinMode(botao_d,INPUT_PULLUP);
}

void loop() {
    // Leitura dos botões
    int leiturad = digitalRead(botao_d);
    int leiturae = digitalRead(botao_e);
    // Definindo modo do micropasso (1,2,4,8,16 ou 32 micropassos)
    stepper.setMicrostep(1); // modo 1

    // Se botão direito for pressioando
    if (leiturad == LOW){
    // o motor dará 100 passos em modo 1 para 360º sentido horário
    stepper.move(100);
    stepper.rotate(360);
    delay(1000); // aguarda 1s
    }

    // Se botão esquerdo for pressioando
    if(leiturae == LOW){
    // o motor dará 100 passos em modo 1 para 360º sentido anti-horário
    stepper.move(-100);
    stepper.rotate(-360);
    delay(1000); // aguarda 1s
    }   
 }
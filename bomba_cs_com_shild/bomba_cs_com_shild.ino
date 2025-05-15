/*
TRABALHO COM UTILIZAÇÃO DO SHILD (MFS)
BOMBA DO CS

INTEGRANTES:
VITOR ALBUQUERQUE DINELLI
RAFAEL ASSIS CARVALHO LACERDA
GABRIEL ASSIS CARVALHO LACERDA
EDUARDO DE SOUZA SILVA
*/


#include <TimerOne.h>         // Bibliotec TimerOne
#include <Wire.h>             // Biblioteca Wire
#include <MultiFuncShield.h>  // Biblioteca Multifunction shield]



int senha;
bool iniciado = false;





void setup() {
  Serial.begin(9600);
  Timer1.initialize(1000);  // inicializa o Timer 1
  MFS.initialize(&Timer1);  // initializa a biblioteca Multi função





  Timer1.attachInterrupt(contador);

  Serial.println("\nCadastre a senha da bomba: ");
}

bool modoCastro = true;


void loop() {
  if (modoCastro) {
    if (Serial.available() > 0) {
      senha = Serial.parseInt();
      Serial.println("Senha castrada com sucesso");
      modoCastro = false;
    }
  }

  if (!modoCastro) {
    byte btn = MFS.getButton();
    if (btn) {
      byte buttonNumber = btn & B00111111;  // comparando os bits das portas analógicas
      byte buttonAction = btn & B11000000;
      if (buttonNumber == 1 && buttonAction == BUTTON_PRESSED_IND) {
        iniciado = false;
      }
    }
  }

  if (iniciado) {

  }
}

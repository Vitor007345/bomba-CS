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

volatile unsigned int clockMilliSeconds = 0;
volatile byte clockSeconds = 40; 




void setup() {
  Serial.begin(9600);
  Timer1.initialize(1000);  // inicializa o Timer 1
  MFS.initialize(&Timer1);  // initializa a biblioteca Multi função
  MFS.userInterrupt = clockCount;



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
        Serial.println("Bomba Plantada");
        Serial.println("Digite a senha para defusar:");
        iniciado = true;
      }
    }
  }

  if (iniciado) {
    if (Serial.available() > 0){
          int senhaDigitada = Serial.parseInt();
          if(senhaDigitada){
            if(senhaDigitada != senha){
              if(clockSeconds > 4)clockSeconds = 4;
            }else{
              iniciado = false;
              Serial.println("Parabéns vc defusou a bomba");
            }
          }
    }
  }
}

void clockCount(){
  if(iniciado){
    clockMilliSeconds++;
    MFS.write(clockSeconds);
    if(clockMilliSeconds >= 1000){
      clockMilliSeconds = 0;
      clockSeconds--;
      MFS.write(clockSeconds);
      if(clockSeconds > 20){
        if(clockSeconds % 2 == 0){
          MFS.beep();
        }
      }else if(clockSeconds > 10){
        MFS.beep();
      }else if(clockSeconds > 5){
        MFS.beep(10, 40, 2, 1, 0);
      }else if(clockSeconds > 2){
        MFS.beep(5, 20, 4, 1, 0);
      }
      else if(clockSeconds == 2){
        MFS.beep(200);
      }else if(clockSeconds == 0){
        iniciado = false;
        //MFS.write(0);
      }
      
    }
  }
}

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
volatile byte clockSeconds = 40; //tempo de explosão da bomba




void setup() {
  Serial.begin(9600); //inicializa o Serial
  Timer1.initialize(1000);  // inicializa o Timer 1
  MFS.initialize(&Timer1);  // initializa a biblioteca Multi função
  MFS.userInterrupt = clockCount; //Inicializa o contador



  Serial.println("\nCadastre a senha da bomba: "); //informa ao usuário para cadastrar a bomba
}

bool modoCastro = true; //inicia com o modo de cadastro de senha


void loop() {
  if (modoCastro) {
    if (Serial.available() > 0) { //confere se o serial está disponível
      senha = Serial.parseInt(); //le um valor inteiro do input do serial, a senha colocamos um número inteiro pq a bomba dd cs tbm é só número a senha
      Serial.println("Senha castrada com sucesso"); //Informa que a senha foi cadastrada
      modoCastro = false; //desativa o modo de cadastro
    }
  }

  //já está fora do modo cadastro, mas ainda não iniciou
  if (!modoCastro) { 
    byte btn = MFS.getButton();
    if (btn) { //checa se um bos botões foi precionado
      byte buttonNumber = btn & B00111111;  // comparando os bits das portas analógicas
      byte buttonAction = btn & B11000000;
      if (buttonNumber == 1 && buttonAction == BUTTON_PRESSED_IND) { //checa se foi o botão 1 que foi pressionada
        Serial.println("Bomba Plantada"); //informa que a bomba foi plantada
        Serial.println("Digite a senha para defusar:"); //Informa que o usuário deve gigitar a senha para defusar
        iniciado = true; //inicia a bomba
      }
    }
  }
  //parte que executada depois que a bomba já foi iniciada
  if (iniciado) {
    if (Serial.available() > 0){ //checa se o serial está disponível
          int senhaDigitada = Serial.parseInt(); //le um número inteiro do input do serial
          if(senhaDigitada){ //checa se algo foi realmente lido, caso o usuário não tenha digitado nada, senhaDigitada ainda não existe então não entra nesse if
            if(senhaDigitada != senha){ 
              if(clockSeconds > 4)clockSeconds = 4; //caso a pessoa erre a senha o tempo diminui instantaneamente para 4s, e obviamente apenas se estiver com mais de 4 segundos faltando, pq se não a pessoa fica digitando varias vezes e voltando para 4s e nunca explode
            }else{
              iniciado = false; //se a senha for correta a bomda é defusada
              Serial.println("Parabéns vc defusou a bomba"); //mensagem informando que o úsuario defusou a bomba
            }
          }
    }
  }
}

void clockCount(){
  if(iniciado){ //checa se já foi iniciado, só realiza a contagem se a bomba já tiver sido iniciada
    clockMilliSeconds++; //soma o milisegundos, pois essa função repete a cada 1ms
    MFS.write(clockSeconds); //atualiza o display a todo segundo
    if(clockMilliSeconds >= 1000){ 
      clockMilliSeconds = 0; //se tiver passado 1000ms, 1s, ele reseta os ms e conta 1s
      clockSeconds--; //diminui os segundos pq em uma bomba a contagem é regressiva
      MFS.write(clockSeconds); //atualiza no display logo após a mudança
      if(clockSeconds > 20){ //De 40s até 20s faz um beep a cada 2s
        if(clockSeconds % 2 == 0){ //como essa parte do código executa a cada um segundo, verificando se o segundo é par, ele faz o beep só quando o segundo é par, sendo sempre a cada 2s
          MFS.beep();
        }
      }else if(clockSeconds > 10){ //Dos 20 até os 10, um beep por segundo
        MFS.beep();
      }else if(clockSeconds > 5){ // dos 10 até os 5, são 2 beeps por segundo
        MFS.beep(
          10, //100ms o beep
          40, //400ms de silencio    //100 + 400 = 500ms   500 x 2repetições    1000ms   1s
          2, //repete o beep 2 vezes
          1, //repete 1 vez o loop
          0 //tempo entre loops
        );
      }else if(clockSeconds > 2){
        MFS.beep(
          5, //50ms o beep
          20, //200ms de silencio    //50 + 200 = 250ms   250 x 4repetições   1000ms 1s
          4, //repete 4 vezes o beep
          1, //repete uma vez o loop
          0 //tempo entre loops
        );
      }
      else if(clockSeconds == 2){ //qnd da 2s executa um beep constante por 2s, ou sejá até o 0
        MFS.beep(200); //2000ms o beep
      }else if(clockSeconds == 0){ 
        iniciado = false; //qnd chega a 0 o processo termina e a variavel iniciado volta para false
      }
      
    }
  }
}

#include <VarSpeedServo.h>
#include <EEPROM.h>

#define pinPot1 A0
#define pinPot2 A1
#define pinPot3 A2
#define pinPot4 A3
#define pinBotao 11
#define pinApagar 12
#define botaoPosicao 6

const int pinServos[4] = {7, 8, 9, 10};
const int pinLeds[4] = {5, 4, 3, 2};

int posicao [7] [5];

int pot1 = 0;
int pot2 = 0;
int pot3 = 0;
int pot4 = 0;
int modoRobo = 0;
int posicaoLinha = 0;

bool apagar = false;
bool botaoModo;
bool posicaoBotao;

int teto = 0;

VarSpeedServo servo1;
VarSpeedServo servo2;
VarSpeedServo servo3;
VarSpeedServo servo4;

void setup() {

  Serial.begin(9600);

  for (int i; i <=3; i++){
    pinMode(pinServos[i], OUTPUT);
    pinMode(pinLeds[i], OUTPUT);
  }

  pinMode(pinBotao, INPUT_PULLUP);
  pinMode(botaoPosicao, INPUT_PULLUP);
  pinMode(pinApagar, INPUT_PULLUP);

  servo1.attach(pinServos[0]);
  servo2.attach(pinServos[1]);
  servo3.attach(pinServos[2]);
  servo4.attach(pinServos[3]);

}
void loop() {

  while (leModoRobo() == 0){
    movimentaRobo();
    leModoRobo();
    ligaLed(3);
  }
  while (leModoRobo() == 1){
    ligaLed(1);
    movimentaRobo();
    leModoRobo();
    apagar = !digitalRead(pinApagar);
    Serial.println("oi");
    if(apagar == 1){
      posicaoLinha = 0;
      for (int i =0; i <= 32; i++){
        EEPROM.update(i, 0);
      }
      while(apagar == 1){
        apagar = !digitalRead(pinApagar);
        ligaLed(4);
      }
    }
    posicaoBotao = !digitalRead(botaoPosicao);
    if(posicaoBotao == 1){
      EEPROM.update(posicaoLinha, servo1.read());
      posicaoLinha++;
      EEPROM.update(posicaoLinha, servo2.read());
      posicaoLinha++;
      EEPROM.update(posicaoLinha, servo3.read());
      posicaoLinha++;
      EEPROM.update(posicaoLinha, servo4.read());
      posicaoLinha++;
      if(posicaoLinha > 32){
        posicaoLinha = 0;
      }
      ligaLed(4);
      Serial.println(EEPROM.read(0));
      while(posicaoBotao == 1){
        posicaoBotao = !digitalRead(botaoPosicao);
      }
    }
  }
  while (leModoRobo() == 2){
    ligaLed(2);
    for(int i =0; i <= 32; i++){
      if(EEPROM.read(i) != 0){
        servo1.slowmove(EEPROM.read(i), 35);
        delay(1000);
        leModoRobo();
        i++;
        servo2.slowmove(EEPROM.read(i), 35);
        delay(1000);
        leModoRobo();
        i++;
        servo3.slowmove(EEPROM.read(i), 35);
        delay(1000);
        leModoRobo();
        i++;
        servo4.slowmove(EEPROM.read(i), 35);
        delay(1000);
        leModoRobo();
      }
      
    }
  }

}

void movimentaRobo(){

  pot1 = analogRead(pinPot1);
  pot1 = map(pot1, 0, 1023, 0, 180);
 
  pot2 = analogRead(pinPot2);
  pot2 = map(pot2, 0, 1023, 0, 180);
 
  pot3 = analogRead(pinPot3);
  pot3 = map(pot3, 0, 1023, 0, 180);
 
  pot4 = analogRead(pinPot4);
  pot4 = map(pot4, 0, 1023, 0, 180);  

  servo1.write(pot1);
  servo2.write(pot2);
  servo3.write(pot3);
  servo4.write(pot4);

}

int leModoRobo (){

  botaoModo = !digitalRead(pinBotao);

  if(botaoModo == 1){
    modoRobo++;
    if(modoRobo > 2){
      modoRobo = 0;
    }
    while( botaoModo == 1){
      botaoModo = !digitalRead(pinBotao);
    }
  }  
  return modoRobo;
}

void ligaLed(byte led){
  switch(led){   
    case 1: 
      digitalWrite(pinLeds[0], LOW);
      digitalWrite(pinLeds[1], HIGH);
      digitalWrite(pinLeds[2], LOW);
      digitalWrite(pinLeds[3], LOW);
      break;
    case 2:
      digitalWrite(pinLeds[0], LOW);
      digitalWrite(pinLeds[1], LOW);
      digitalWrite(pinLeds[2], HIGH);
      digitalWrite(pinLeds[3], LOW);
      break;
    case 3:
      digitalWrite(pinLeds[0], LOW);
      digitalWrite(pinLeds[1], LOW);
      digitalWrite(pinLeds[2], LOW);
      digitalWrite(pinLeds[3], HIGH);
      break;
    default:
      for(int i = 0; i <= 3; i++){
        digitalWrite(pinLeds[i], LOW);
      }
      break;
  }
}

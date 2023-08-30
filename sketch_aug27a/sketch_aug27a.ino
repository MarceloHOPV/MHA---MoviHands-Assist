#include <BleKeyboard.h>
BleKeyboard bleKeyboard;
 
 //Inicializando variáveis
const int sensorPin[5] = {33, 32, 35, 34, 39}; //definicao pinos sensores

const int ledPin = 2; //pino do led
const int btnPin = 26; //pino do botao

const int sensnum = 3;    // numero de sensores
const int maxref = 4095;  // 1023 para arduino 4095 para esp
const int speeds = 250000; //velociade do serial
const int filtro = 100; //numeros de amostras do filtro
const int caltime = 4000; //tempo de calibracao em ms
bool first; //usado para guardar primeiras execucoes

int sensorvalue[sensnum][3]; //armazena valores lidos e temporarios dos sensores
int sensorvalueF[5] = {0, 0, 0, 0, 0}; //armazena resultado da media movel
int reftime; //guardar tempo de referencia

void calibra(); //declaracao nada comum de void mas que funcina legal
void zeraref(); //zera mat de ref


//setup
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);

  bleKeyboard.begin();

  Serial.begin(speeds);

  //faz uma primeira calibracao por 4 segundos
  reftime = millis(); //t1
  calibra(reftime);

}

void calibra(int reft)
{
  digitalWrite(ledPin, HIGH); //acende o led durante calibracao
  zeraref();
  delay(400);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH); //acende o led durante calibracao
  while ((millis() - reft) < caltime) {
    for (int i = 0; i < sensnum; i++)
    {
      sensorvalue[i][0] = analogRead(sensorPin[i]);
      if (sensorvalue[i][0] > sensorvalue[i][2])
      {
        sensorvalue[i][2] = sensorvalue[i][0];
      }
      if (sensorvalue[i][0] < sensorvalue[i][1])
      {
        sensorvalue[i][1] = sensorvalue[i][0];
      }
    }
  }

  digitalWrite(ledPin, LOW);
}

void zeraref() {
  for (int i = 0; i < sensnum; i++)
  {
    sensorvalue[i][0] = 0;      // sensor value
    sensorvalue[i][1] = maxref; // min sensor value
    sensorvalue[i][2] = 0;      // max sensor value
  }
}

void loop()
{
  if (bleKeyboard.isConnected()) {
    //faz leitura, mapeamento e filtro
    for (int i = 0 ; i < filtro ; i++)
    {
      for (int j = 0 ; j < sensnum ; j++)
      {
        sensorvalue[j][0] = analogRead(sensorPin[j]);
        sensorvalue[j][0] = constrain(sensorvalue[j][0], sensorvalue[j][1], sensorvalue[j][2]);//rever esse sensor value j1 e j2
        sensorvalue[j][0] = map(sensorvalue[j][0], sensorvalue[j][1], sensorvalue[j][2], 0, 500);
        sensorvalueF[j] += sensorvalue[j][0];
      }
    }

    for (int j = 0 ; j < sensnum ; j++)
    {
      sensorvalueF[j] = sensorvalueF[j] / filtro;
      Serial.print(sensorvalueF[j]);
      Serial.print(' ');

      if (j == 0 && sensorvalueF[j] < 200 && sensorvalueF[j] > 0) {
        bleKeyboard.write(KEY_UP_ARROW);
      }
      else if (j == 1 && sensorvalueF[j] < 200 && sensorvalueF[j] > 0) {
        bleKeyboard.write(KEY_DOWN_ARROW);
      }
      else if (j == 2 && sensorvalueF[j] < 200 && sensorvalueF[j] > 0) {
        bleKeyboard.write(KEY_LEFT_ARROW);
      }
      else if (j == 3 && sensorvalueF[j] < 200 && sensorvalueF[j] > 0) {
        bleKeyboard.write(KEY_RIGHT_ARROW);
      }
      else if (j == 4 && sensorvalueF[j] < 200 && sensorvalueF[j] > 0) {
        bleKeyboard.print("J");
      }
    }
    Serial.println();
    while (!digitalRead(btnPin)) {//pq o "!"
      if (first) {
        reftime = millis(); //t1
        first = false;
      }
      calibra(reftime);
    }
    first = true;
  }
}
#define SENSOR A0 // Entrada da Eletro

long int sin_eletro = 0;
long int sim_media = 0;
int cont = 0;
unsigned long tempo_comp;
unsigned long tempo_ms;
unsigned long tempo_s;
long int sin_eletro_ant = 400;
int control = 400;
int pin = 7;
void setup()
{
  Serial.begin(20000000);
  pinMode(SENSOR,INPUT);
  pinMode(pin,OUTPUT);
}

void loop()
{
  tempo_comp = millis()/1000;
  for(int i = 0; i < 50; i++)
  {
    sin_eletro = analogRead(SENSOR);
    sin_media += sin_eletro;
  }
  sin_media /=50;

  if(tempo_coomp - tempo_s >=2)
  {
    if((sin_media > sin_eletro_ant * 1.15 || sin_media < sin eletro_ant* 0.05) && control = 400)
    {
      control = 600;
      digitalWrite(pin, HIGH);
    }
    else
    {
      control = 400;
      digitalWrite(pin, LOW);
    }
    tempo_s = tempo_ms / 1000;
  }

  Serial.print(sin_media);
  Serial.print(",");
  Serial.print(control);
  Serial.print(",");
  Serial.print(sin_eletro_ant);
  cont++;
  if(cont == 10){
    sin_eletro_ant = sin_media;
    cont = 0;
  }

  sin_media = 0;

}

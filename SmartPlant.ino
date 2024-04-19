//Include de Bibliotecas ---------------
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "dht.h"

//Definições de pinos ------------------
#define DHTPIN A2 // pino que estamos conectado
#define sensor_nivel_agua A1
#define DHTTYPE DHT11 // DHT 11
#define azul 11
#define verde 9
#define vermelho 10
#define umidadeAnalogica A3 //Atribui o pino A0 a variável umidade - leitura analógica do sensor
#define umidadeDigital 7 //Atribui o pino 7 a variável umidadeDigital - leitura digital do sensor
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);

int valorumidade; //Declaração da variável que armazenará o valor da umidade lida - saída analogica
int valorumidadeDigital; //Declaração da variável que armazenara a saída digital do sensor de umidade do solo

void setup()
{
  pinMode(vermelho, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(umidadeAnalogica, INPUT); //Define umidadeAnalogica como entrada
  pinMode(umidadeDigital, INPUT);   //Define umidadeDigital como entrada
  lcd.init();   dht.begin();   lcd.backlight();
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Inicializando");
  lcd.setCursor(0,1);
  lcd.print("a planta");
  lcd.clear();
  led_iniciando();
}

void loop()
{
  valorumidade = analogRead(umidadeAnalogica); //Realiza a leitura analógica do sensor e armazena em valorumidade
  valorumidade = map(valorumidade, 1023, 315, 0, 100); //Transforma os valores analógicos em uma escala de 0 a 100
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Umidade encontrada: "); //Imprime mensagem
  Serial.print(valorumidade); //Imprime no monitor serial o valor de umidade em porcentagem
  Serial.println(" % " );

  nivel_agua ();
  
  if(valorumidade <= 10){
    lcd.clear();
    Serial.println(" Umidade BAIXA! " ); Serial.print(valorumidade);  Serial.println(" % " );
    lcd.setCursor(0,0); lcd.print("umidade baixa");
    lcd.setCursor(0,1); lcd.print("Solo: "); lcd.print(valorumidade); lcd.print("%");
    delay(5000);   
  }else if(valorumidade >= 11 && valorumidade <= 50){
    lcd.clear();
    Serial.println(" Umidade BAIXA! " ); Serial.print(valorumidade);  Serial.println(" % " );
    lcd.setCursor(0,0); lcd.print("umidade media");
    lcd.setCursor(0,1); lcd.print("Solo: "); lcd.print(valorumidade); lcd.print("%");
    delay(5000);
  } else {
    lcd.clear();
    Serial.println(" Umidade alta! " ); Serial.print(valorumidade);  Serial.println(" % " );
    lcd.setCursor(0,0); lcd.print("umidade alta");
    lcd.setCursor(0,1); lcd.print("Solo: "); lcd.print(valorumidade); lcd.print("%");
    delay(5000);
  }

  
  if (!isnan(t) || !isnan(h))
    leitura_temperatura(t, h); //leitura temperatura e umidade do ambiente
  else
    erro_leitura_temperatura();

  if(!isnan(sensor_nivel_agua))
    nivel_agua(); // Leitura nível da água no reservatório
  else 
    erro_nivel_agua();
}

void leitura_temperatura(float temperatura, float umidade){
    lcd.setCursor(0,0); lcd.print("Umidade - Temp");
    lcd.setCursor(0,1); lcd.print(umidade); lcd.print("%    "); lcd.print(temperatura); lcd.print("C");
    nivel_agua(); // Leitura nível da água no reservatório ---------------------
    delay(2000);
    lcd.clear();  
}

void erro_leitura_temperatura(){
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("FALHA");
    lcd.setCursor(0,1); lcd.print("DHT-11");
}

void nivel_agua (){
  Serial.print("Valor Sensor do reservatorio: ");
  Serial.println(analogRead(sensor_nivel_agua));
  if((analogRead(sensor_nivel_agua)) >= 1 & (analogRead(sensor_nivel_agua)) <= 150){
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
    digitalWrite(vermelho, LOW);
  }
    
   else if((analogRead(sensor_nivel_agua)) > 151 & (analogRead(sensor_nivel_agua)) <= 290){
    digitalWrite(verde, HIGH);
    digitalWrite(azul, LOW);
    digitalWrite(vermelho, HIGH);
   }
    
   else{ 
    digitalWrite(verde, LOW);
    digitalWrite(azul, LOW);
    digitalWrite(vermelho, HIGH);
   }
}

void erro_nivel_agua(){
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("FALHA");
    lcd.setCursor(0,1); lcd.print("Sensor Reservatorio");
    delay(10000);
    lcd.clear();
}

void led_iniciando(){
  digitalWrite(vermelho, HIGH);
  digitalWrite(azul, HIGH);
  digitalWrite(verde, HIGH);
  delay(2000);
  digitalWrite(vermelho, LOW);
  digitalWrite(azul, LOW);
  digitalWrite(verde, LOW);
  delay(500);
  digitalWrite(verde, HIGH);
  delay(100);
  digitalWrite(verde, LOW);
  delay(100);
  digitalWrite(azul, HIGH);
  delay(100);
  digitalWrite(azul, LOW);
  delay(100);
  digitalWrite(vermelho, HIGH);
  delay(100);
  digitalWrite(vermelho, LOW);
  delay(500);
  }

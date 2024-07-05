#include "DHT.h"
#include <DHT_U.h> 

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int sensorPin = 2; // Pin al que está conectado el sensor digital
unsigned long previousMillis = 0; // Variable para almacenar el tiempo anterior
const long interval = 1000; // Intervalo de tiempo de 7 segundos (7000 ms)
unsigned long motionMillis = 0; // Tiempo en el que se detectó movimiento
const long motionInterval = 5000; // Tiempo durante el cual se mantiene el estado de movimiento (5 segundos)
int value_water;
float t;
float h;
int htierra;
int personal = 0; // Estado inicial de personal

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps
  pinMode(sensorPin, INPUT); // Configura el pin del sensor como entrada
  dht.begin();
}

void loop() {

  h = dht.readHumidity(); // Leemos la Humedad
  t = dht.readTemperature();
  value_water = analogRead(A0);
  htierra = analogRead(A1);

  unsigned long currentMillis = millis(); // Obtiene el tiempo actual

  // Verifica si hay movimiento
  if (digitalRead(sensorPin) == HIGH) {
    personal = 1; // Se detectó movimiento
    motionMillis = currentMillis; // Actualiza el tiempo en el que se detectó movimiento
  }

  // Verifica si ha pasado el tiempo de mantener el estado de movimiento
  if (currentMillis - motionMillis >= motionInterval) {
    personal = 0; // Resetea el estado de movimiento después de 5 segundos
  }

  // Verifica si ha pasado el intervalo de tiempo para la lectura de los sensores
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Actualiza el tiempo anterior

    // Imprime los valores en el serial
    Serial.print("P");
    Serial.print(personal);
    Serial.print("H");
    Serial.print(h);
    Serial.print("T");
    Serial.print(t);
    Serial.print("A");
    Serial.print(value_water);
    Serial.print("M");
    Serial.print(htierra);
    Serial.print("F");
    delay(100);
  }
}

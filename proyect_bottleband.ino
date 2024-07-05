#define SENSOR 4
#define ACTION 5 
#define TRIGGER_PIN 2 // pin para el pulso de disparo del sensor
#define ECHO_PIN 3 // pin para la medición del eco del sensor
#define RELAY_PIN 6 // pin para controlar el relé

int contadorPlastico = 0;
int contadorMetal = 0;

unsigned long int lastTimeL= 0;
unsigned long int lastTimeP= 0;
bool boolL = false;
bool boolP = false;

//MOTOR

int Encoder_C1 = 8; // Cable rojo pin 8 digital
unsigned long timeold = 0;
int paso = 0;
double RPM = 0;
int pulsesperturn = 11;
static volatile unsigned long debounce = 0;


void calculapulso(){
  if(  digitalRead (Encoder_C1) && (micros()-debounce > 500) && digitalRead (Encoder_C1) ) { 
    debounce = micros(); //Se asegura de no contar el rebote
    paso++;
  }
        else ;
} 


void setup() {

  Serial.begin(9600); 
  pinMode(SENSOR, INPUT_PULLUP); 
  pinMode(ACTION, OUTPUT); 
  pinMode(TRIGGER_PIN, OUTPUT); // Configura el pin de disparo como salida
  pinMode(ECHO_PIN, INPUT); // Configura el pin de eco como entrada
  pinMode(RELAY_PIN, OUTPUT); // Configura el pin del relé como salida

  attachInterrupt(0, calculapulso, RISING);
  pinMode(Encoder_C1, INPUT);
}

void loop() {

  long duration, distance;
  /////////////////////////////////ENCODER////////////////////////////////////
  if (millis() - timeold >= 500){
    noInterrupts();
    RPM = -(60 * 1000 / pulsesperturn ) / (millis() - timeold) * paso/4 - 4; 
    timeold = millis();
    paso = 0;
    interrupts();
  }

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

 ////////////////////////////////PISTONES////////////////////////////////////

  int L = digitalRead(SENSOR); 
  
  if (L == 1) {
    //No se detecta nada
    digitalWrite(ACTION, LOW);
  } else {
    //Se detecta el metal
    digitalWrite(ACTION, HIGH);
    if (boolL == false && (millis() - lastTimeL >= 1000)) {
      boolL = true;
    }
    lastTimeL = millis();
  }

  if (boolL == true) {
    //Suma de botellas de metal
    contadorMetal = contadorMetal + 1;
    boolL = false;
  }

  ////////////////////////////////ULTRASONIDO////////////////////////////////////

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < 8) {
    digitalWrite(RELAY_PIN, HIGH); 
    if (boolP == false && (millis() - lastTimeP >= 1000)) {
      boolP = true;
      lastTimeP= millis();
    }
  } else {
    digitalWrite(RELAY_PIN, LOW); 
  }

  if (boolP == true) {
    contadorPlastico = contadorPlastico+1;
    boolP = false;
  }



  Serial.print("L");
  Serial.print(contadorMetal);
  Serial.print("P");
  Serial.println(contadorPlastico);
  Serial.print("R");
  Serial.println(RPM);
  Serial.print("F");
  delay(100);
}

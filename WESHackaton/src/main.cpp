#include <Arduino.h>
#include "BobHandler.h"

#define DEBUG

// definicije pinova
#define MAP 9  // pin 9 -> MAP senzor
#define K_SEN 5  // pin 5 -> koljenasto vratilo
#define B_SEN 6  // pin 6 -> bregasto vratilo
#define B_OUT 11  // pin 11 -> izlazni pin za upravljanje bobinom

// definicije konstanti
#define LOW_PRESSURE 15
#define HIGH_PRESSURE 100
#define LOW_VOLTAGE 0.5
#define HIGH_VOLTAGE 4.5 
#define MAX_MIN_SUB 921.6

// definicije funkcija
int calculateMAPPressure(float x){
	return x / (MAX_MIN_SUB) * (HIGH_PRESSURE - LOW_PRESSURE) + 15;
}

// setup funkcija
void setup() {
  	pinMode(MAP, INPUT);
  	pinMode(K_SEN, INPUT);
  	pinMode(B_SEN, INPUT);
	pinMode(B_OUT, OUTPUT);
}

// loop funkcija
void loop() {
	float x = analogRead(MAP);
}
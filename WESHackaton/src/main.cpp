#include <Arduino.h>
#include "BobHandler.h"
#include "BregHandler.h"
#include "RPMHandler.h"

#define DEBUG

// definicije pinova
#define B_OUT 11  // pin 11 -> izlazni pin za upravljanje bobinom

// setup funkcija
void setup() {
	pinMode(B_OUT, OUTPUT);

	setupRPMTimer();
	setupBreg();
}


void calculateSpark(float angle){
	if(){

	}
	chargeDischargeSparkPlug();
}


void chargeDischargeSparkPlug(){
	digitalWrite(B_OUT, HIGH);
	delay(1.5);
	digitalWrite(B_OUT, LOW);
}


// loop funkcija
void loop() {
	int value = analogRead(pinMAP);
	float angle = getAngle(fmap((float)value, 102, 920, 15, 100), getRPM());

	calculateSpark(angle);
}
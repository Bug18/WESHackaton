#include <Arduino.h>
#include "BobHandler.h"
#include "BregHandler.h"
#include "RPMHandler.h"

#define DEBUG

// definicije pinova
#define B_OUT 11  // pin 11 -> izlazni pin za upravljanje bobinom


void chargeDischargeSparkPlug(){
	digitalWrite(B_OUT, HIGH);
	delay(1.5);
	digitalWrite(B_OUT, LOW);
	noInterrupts();
	armedForSpark = false;
	interrupts();
}


void calculateSpark(){
	noInterrupts();
	bool _armedForSpark = armedForSpark;  // lokalna varijabla
	int _zubac_cnt = zubac_cnt;
	uint32_t _past_delta = past_delta / 16.0;
	interrupts();
	float _delay;

	if(_armedForSpark){
		int value = analogRead(pinMAP);
		float angle = getAngle(fmap((float)value, 102, 920, 15, 100), getRPM());
		int current_position = _zubac_cnt * 15;
		
		_delay = (360 - angle - (1500 * 15 / _past_delta) - current_position) * _past_delta / 15;
	}else{
		return;
	}
	
	delay(_delay);
	chargeDischargeSparkPlug();
}


// setup funkcija
void setup() {
	pinMode(B_OUT, OUTPUT);

	setupRPMTimer();
	setupBreg();
}


// loop funkcija
void loop() {
	calculateSpark();
}
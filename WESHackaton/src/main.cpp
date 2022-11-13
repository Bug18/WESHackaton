#include <Arduino.h>
#include "BobHandler.h"
#include "BregHandler.h"
#include "RPMHandler.h"

void setup() {
	// Serial.begin(9600);
	pinMode(B_OUT, OUTPUT);
	setupRPMTimer();
	setupBreg();
}

void loop() {}
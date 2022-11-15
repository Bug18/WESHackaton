#include <Arduino.h>
#include "BobHandler.h"
#include "BregHandler.h"
#include "RPMHandler.h"

void setup() {
	pinMode(B_OUT, OUTPUT);
	setupRPMTimer();
	setupBreg();
}

void loop() {}
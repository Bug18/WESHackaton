#include <Arduino.h>

#include "BregHandler.h"
#include "RPMHandler.h"

void setup(){
    Serial.begin(9600);
    setupRPMTimer();
}

void loop(){       
    Serial.println(getRPM(), 2);
    //delay(1000);  // Slow down output
}
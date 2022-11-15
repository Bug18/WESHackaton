#include "BregHandler.h"
#include "RPMHandler.h"

#include <Arduino.h>

volatile bool bregChecked = false;
volatile bool bregExists  = false;

void setupBreg(){
    attachInterrupt(digitalPinToInterrupt(BREG_PIN), bregTriggered, RISING);
}

void bregTriggered(){
    bregChecked  = true;
    bregExists   = true;
    treashold    = 0;
    zubac_count  = -2;
}
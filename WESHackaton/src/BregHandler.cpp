#include "BregHandler.h"
#include "RPMHandler.h"

#include <Arduino.h>

bool bregChecked = false;
bool bregExists = false;

void setupBreg(){
    attachInterrupt(BREG_PIN, bregTriggered, HIGH);
}

void bregTriggered(){
    bregChecked = true;
    bregExists = true;
    zubac_cnt = -2;
}
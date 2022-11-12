#ifndef MEASURERPM_H
#define MEASURERPM_H

#include <Arduino.h>

#define RPM_PIN 8

uint32_t past_delta;
uint32_t delta;

uint32_t past_delta_sum;
uint32_t delta_sum;

bool firstRisingPassed;
uint32_t start_time;

ISR(TIMER1_CAPT_vect);

void initRPMTimer();
uint32_t getRPM();

#endif
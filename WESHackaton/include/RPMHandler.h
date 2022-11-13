#ifndef RPMHANDLER_H
#define RPMHANDLER_H

#include <Arduino.h>

extern uint32_t past_delta;
extern uint32_t delta;

extern uint32_t past_delta_sum;
extern uint32_t delta_sum;

extern int zubac_cnt;
extern bool toothless;
extern int rotationNumber;

extern volatile uint16_t Overflows;

ISR(TIMER1_OVF_vect);
ISR(TIMER1_CAPT_vect);
void setupRPMTimer();
float getRPM();

#endif

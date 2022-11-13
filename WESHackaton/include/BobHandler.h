#ifndef BOBHANDLER_H
#define BOBHANDLER_H

#include <Arduino.h>

#define pinMAP 9
// #define DEBUG
// #define TEST

#ifndef TEST
float fmap(float value, float inMin, float inMax, float outMin, float outMax);
template <typename T>
int getIndex(T array[], int size, T target);

#define PRESSURES_COUNT 10
#define RPMS_COUNT 11

extern int PRESSURES[PRESSURES_COUNT];
extern int RPMS[RPMS_COUNT];

extern int ANGLE_DATA[11][11];


// Definicije funkcija

int getAngleFromTable(int pressure, int rpm);

int customFloor(int DATA[], int size, float inputValue);

int customCeil(int DATA[], int size, float inputValue);

float getAngle(float pressure, float rpm);

/// Debug funkcije
#ifdef DEBUG
void setup();

void loop();
#endif


#endif
#endif
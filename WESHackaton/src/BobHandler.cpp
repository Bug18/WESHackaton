#include <Arduino.h>
#include "BobHandler.h"


int PRESSURES[PRESSURES_COUNT] = {15, 25, 30, 40, 50, 60, 70, 80, 90, 100};
int RPMS[RPMS_COUNT] = {1000, 1400, 1800, 2500, 3000, 4000, 6000, 8000, 10000, 12000, 14000};

int ANGLE_DATA[11][11]{
    {10, 13, 18, 20, 25, 25, 25, 25, 25, 25, 25},
    {10, 13, 18, 21, 28, 28, 28, 28, 28, 28, 28},
    {10, 13, 19, 22, 30, 30, 30, 30, 30, 30, 30},
    {10, 13, 19, 23, 32, 32, 32, 32, 32, 32, 32},
    {10, 13, 19, 24, 33, 33, 33, 33, 33, 33, 33},
    {10, 13, 20, 24, 35, 35, 35, 35, 35, 35, 35},
    {10, 14, 20, 25, 36, 36, 36, 36, 36, 36, 36},
    {10, 14, 21, 28, 38, 38, 38, 38, 38, 38, 38},
    {10, 15, 23, 30, 40, 40, 40, 40, 40, 40, 40},
    {10, 18, 25, 34, 38, 42, 42, 42, 42, 42, 42}};

int getAngleFromTable(int pressure, int rpm)
{

  // ovo je tak cudno zato jer moram invertat koji index dobijem.
  // to je tak da PRESSURES array bude uzlazno sortiran
  int row = PRESSURES_COUNT - 1 - getIndex(PRESSURES, 10, pressure);
  int column = getIndex(RPMS, 11, rpm);

  return ANGLE_DATA[row][column];
}

// računa ceiling rpma ili pressura prema tablici
// ako je rpm 3200 ceil je 4000
// ako je rom 4000 ceil je 4000
int customFloor(int DATA[], int size, float inputValue)
{
  int out = DATA[0];
  for (int i = size - 1; i >= 0; i -= 1)
  {
    if (inputValue >= DATA[i])
    {
      out = DATA[i];
      break;
    }
  }
  return out;
}

// računa ceiling rpma prema tablici
// ako je rpm 3200 ceil je 4000
// ako je rom 4000 ceil je 4000
int customCeil(int DATA[], int size, float inputValue)
{
  int out = DATA[size - 1];
  for (int i = 0; i < size; i++)
  {
    if (inputValue <= DATA[i])
    {
      out = DATA[i];
      break;
    }
  }
  return out;
}

float getAngle(float pressure, float rpm)
{
  int pressureFloor = customFloor(PRESSURES, PRESSURES_COUNT, pressure);
  int pressureCeil = customCeil(PRESSURES, PRESSURES_COUNT, pressure);

  int rpmFloor = customFloor(RPMS, RPMS_COUNT, rpm);
  int rpmCeil = customCeil(RPMS, RPMS_COUNT, rpm);

  int topLeft = getAngleFromTable(pressureCeil, rpmFloor);
  int topRight = getAngleFromTable(pressureCeil, rpmCeil);
  int bottomLeft = getAngleFromTable(pressureFloor, rpmFloor);
  int bottomRight = getAngleFromTable(pressureFloor, rpmCeil);

  float top = fmap(rpm, rpmFloor, rpmCeil, topLeft, topRight);
  float bot = fmap(rpm, rpmFloor, rpmCeil, bottomLeft, bottomRight);

  float mid = fmap(pressure, pressureFloor, pressureCeil, bot, top); // top je min kad je uvijek manji od bota

#ifdef DEBUG
  Serial.print("PRESSURE ");
  Serial.print(pressureFloor);
  Serial.print(" ");
  Serial.println(pressureCeil);

  Serial.print("RPM ");
  Serial.print(rpmFloor);
  Serial.print(" ");
  Serial.println(rpmCeil);

  Serial.println("VALUE GRID");
  Serial.print(topLeft);
  Serial.print(" ");
  Serial.println(topRight);

  Serial.print(bottomLeft);
  Serial.print(" ");
  Serial.println(bottomRight);

  Serial.print("TOP AND BOT: ");
  Serial.print(top);
  Serial.print(" ");
  Serial.println(bot);

  Serial.print("ANGLE:");
  Serial.println(mid);
#endif
  return mid;
}

/// CODEEE
#ifdef DEBUG
void setup()
{
  Serial.begin(9600);
  pinMode(pinMAP, INPUT);
}

void loop()
{
  int value = analogRead(pinMAP);
  float pressure = fmap((float)value, 102, 920, 15, 100); //
  Serial.println("---------------- PRES READING BELOW:");
  Serial.println(pressure);
  float angle = getAngle(pressure, 4000);
  Serial.println(angle);
  delay(10);
}
#endif

#ifdef TEST
// cout << getAngle(31, 2671) << endl;
#endif

/// library
template <typename T>
int getIndex(T array[], int size, T target)
{
  int index = -1;
  for (int i = 0; i < size; i++)
  {
    if (array[i] == target)
    {
      index = i;
      break;
    }
  }
  return index;
}

float fmap(float value, float inMin, float inMax, float outMin, float outMax)
{
  if (inMin == inMax && outMin == outMax)
  {
    return outMax;
  }
  else if (inMin == inMax && outMax != outMin)
  {
    Serial.println("BIG PROBLEM");
  }
  else
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

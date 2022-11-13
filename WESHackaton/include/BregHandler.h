#ifndef BREGHANDLER_H
#define BREGHANDLER_H

#define BREG_PIN 3

extern volatile bool bregChecked;
extern volatile bool bregExists;
extern int ZubacCntBreg;

void setupBreg();
void bregTriggered();

#endif

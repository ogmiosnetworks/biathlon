#include <Arduino.h>
#include <EEPROM.h>

#ifndef std_functions_h
#define std_functions_h

void serialSetUp();
void initOutputPin(int pinId);
void initOutputPin(int pinId, int initStatus);
void setupComplete();
void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize);
int readIntArrayFromEEPROM(int address, int numbers[], int arraySize);
void blink(const int pins[], int delayDuration, int blinkCount, int arraySize);
void blink(int pin, int delayDuration, int blinkCount);
void setAllTo(const int pins[],int status, int arraySize);

#endif

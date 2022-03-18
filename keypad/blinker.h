#include <Arduino.h>
#include "ledStatus.h"

#ifndef Blinker_h
#define Blinker_h

//
// handle the blinking of all of our LEDs
//
class Blinker {
public:
  Blinker(int greenPin, int redPin, int yellowPin, int secondYellowLed);
  void Update();
  void AddBlinkToGreen(int count);
  void AddBlinkToRed(int count);
  void AddBlinkToYellow(int count);
  void AddBlinkToAll(int count);
  bool LearningMode;
private:
  LedStatus *GreenLed;
  LedStatus *RedLed;
  LedStatus *YellowLed;

  bool NeedToBlink();
  void SetAllToStatus();
  unsigned long LastBlink=0;
  unsigned long BlinkRate=300;
  int Status;
  int SecondYellowLed;
};

#endif

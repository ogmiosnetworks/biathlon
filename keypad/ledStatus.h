#include <Arduino.h>
#ifndef LedStatus_h
#define LedStatus_h

//
// Track is the status of a single LED
//
class LedStatus {
public:
  LedStatus(int pin);
  LedStatus(int pin, int initialStatus);
  void SetCount(int count);
  void UpdateCount(int count);
  int Status;
  int Pin;
  int Count=0;  
};

#endif

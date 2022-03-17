#include <Arduino.h>
#ifndef LedStatus_h
#define LedStatus_h

//
// What is the status of a single LED
//
class LedStatus {
public:
  LedStatus(int pin);
  void Update(unsigned long newBlink);
  void SetCount(int count);
  void UpdateCount(int count);
  void ClearCount();
  void SetLastBlink(unsigned long newBlink);
  int Status;
  int Pin;
  
 private:
  int Count=0;  
  unsigned long LastBlink=0;
  unsigned long BlinkRate=300;
  
};

#endif

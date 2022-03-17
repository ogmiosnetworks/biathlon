#include <Arduino.h>
#include "ledStatus.h"

LedStatus::LedStatus(int pin){
    Status = LOW;
    Pin = pin;
    Count = 0;
    digitalWrite(Pin, LOW);  
  }

  void LedStatus::SetCount(int count){
    Count = count *2;
  }
  void LedStatus::UpdateCount(int count){
    Count += count *2;
  }

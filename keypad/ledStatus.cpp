#include <Arduino.h>
#include "ledStatus.h"

// setup an led - default it to off
LedStatus::LedStatus(int pin)
{
    Status = LOW;
    Pin = pin;
    Count = 0;
    digitalWrite(Pin, Status);  
}

// setup an led - default it to initialStatus
LedStatus::LedStatus(int pin, int initialStatus)
{
    Status = initialStatus;
    Pin = pin;
    Count = 0;
    digitalWrite(Pin, Status);  
}

// set the count - ignore the current value
void LedStatus::SetCount(int count)
{
  Count = count *2;
}

// add more to the count
void LedStatus::UpdateCount(int count)
{
  Count += count *2;
}

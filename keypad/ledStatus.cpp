#include <Arduino.h>
#include "ledStatus.h"

LedStatus::LedStatus(int pin){
    Status = LOW;
    Pin = pin;
    Count = 0;
    digitalWrite(Pin, LOW);  
  }

  void LedStatus::Update(unsigned long newBlink){    
    if(0<Count && (0 == LastBlink || BlinkRate+LastBlink <= newBlink)){      
      Status = (LOW == Status?HIGH:LOW);
          Serial.print("Setting pin ");
          Serial.print(Pin);
          Serial.print(" to ");
          Serial.print(Status);
          Serial.print(" count is ");
          Serial.print(Count);
          Serial.print(" last blink is ");
          Serial.print(LastBlink);
          Serial.print(" cur millis is ");
          Serial.print(newBlink);
          Serial.print(" cur blink val is ");
          Serial.println( BlinkRate+LastBlink);
          
      digitalWrite(Pin, Status);  
      Count--;
      LastBlink = newBlink;
    }
  }

  void LedStatus::ClearCount(){
    Count = 0;
    }
  void LedStatus::SetCount(int count){
    Count = count *2;

    Serial.print("SetCount: ");
    Serial.print(Count);
    Serial.print(" for pin ");
    Serial.println(Pin);
  }
  void LedStatus::UpdateCount(int count){
    Count += count *2;

    Serial.print("UpdateCount: ");
    Serial.print(Count);
    Serial.print(" for pin ");
    Serial.println(Pin);
  }

  void LedStatus::SetLastBlink(unsigned long newBlink)
  {
    LastBlink = newBlink;
  }

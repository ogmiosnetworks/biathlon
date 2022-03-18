#include <Arduino.h>
#include "blinker.h"
#include "ledStatus.h"

Blinker::Blinker(int greenPin, int redPin, int yellowPin, int secondYellowLed)
{
  GreenLed = new LedStatus(greenPin);
  RedLed = new LedStatus(redPin);
  YellowLed = new LedStatus(yellowPin);
  Status = LOW;
  SetAllToStatus();   
  LearningMode = false;
  SecondYellowLed = secondYellowLed;
}

// should be called at the bottom of the main loop
void  Blinker::Update()
{
  unsigned long syncTo = millis();

   if(NeedToBlink() && (0 == LastBlink || BlinkRate+LastBlink <= syncTo))
   {  
    Status = (LOW == Status?HIGH:LOW);
    
    if(0 < GreenLed->Count){
      GreenLed->Count--;
      digitalWrite(GreenLed->Pin, Status);  
    }
    
    if(0 < RedLed->Count){
      RedLed->Count--;
      digitalWrite(RedLed->Pin, Status);         
    }
    if(0 < YellowLed->Count){
      YellowLed->Count--;
      digitalWrite(YellowLed->Pin, Status);  
      digitalWrite(SecondYellowLed, Status);
    }
    
    LastBlink = syncTo;
   }
   else
   {
    // make sure the pins are set to low if we aren't blinking.
     if(0 == GreenLed->Count && HIGH == digitalRead(GreenLed->Pin)){        
      digitalWrite(GreenLed->Pin, LOW);  
    }
    
    if(0 == RedLed->Count && HIGH == digitalRead(RedLed->Pin)){        
      digitalWrite(RedLed->Pin, LOW);  
    }

    if(LearningMode && LOW == digitalRead(YellowLed->Pin))
    {
       digitalWrite(YellowLed->Pin, HIGH);  
       digitalWrite(SecondYellowLed, HIGH);
    }
    else if(!LearningMode && 0 == YellowLed->Count && HIGH == digitalRead(YellowLed->Pin))
    {        
      digitalWrite(YellowLed->Pin, LOW);  
      digitalWrite(SecondYellowLed, LOW);
    }
    
   }
}

// update the state of all LEDs we control
void  Blinker::SetAllToStatus()
{
      digitalWrite(GreenLed->Pin, Status);  
      digitalWrite(RedLed->Pin, Status);  
      digitalWrite(YellowLed->Pin, Status); 
      digitalWrite(SecondYellowLed, Status);  
}

// do we need to blink any of the LEDs
bool  Blinker::NeedToBlink(){
   bool n2b = (0 < GreenLed->Count || 0 < RedLed->Count || 0 < YellowLed->Count);
   return n2b;
}

// set the green blink count
void  Blinker::AddBlinkToGreen(int count){      
  Serial.println("Adding to green");
  GreenLed->SetCount(count);
}

// set the red blink count
void  Blinker::AddBlinkToRed(int count)
{
  Serial.println("Adding to red");
  RedLed->SetCount(count);
}

// set the yellow blink count
void  Blinker::AddBlinkToYellow(int count)
{
  Serial.println("Adding to yellow");
  YellowLed->SetCount(count);
}

// set the count on all of the LEDs
void  Blinker::AddBlinkToAll(int count)
{
  GreenLed->SetCount(count);
  RedLed->SetCount(count);
  YellowLed->SetCount(count);
  Status = LOW;
  SetAllToStatus();
}

#include <Arduino.h>
#include "blinker.h"
#include "ledStatus.h"

  Blinker::Blinker(int greenPin, int redPin, int yellowPin){
    GreenLed = new LedStatus(greenPin);
    RedLed = new LedStatus(redPin);
    YellowLed = new LedStatus(yellowPin);
  }
  void  Blinker::Update(){
    unsigned long syncTo = millis();
    GreenLed->Update(syncTo);
    RedLed->Update(syncTo);
    YellowLed->Update(syncTo);
  }
  void  Blinker::AddBlinkToGreen(int count){      
      GreenLed->SetCount(count);
  }
  void  Blinker::AddBlinkToRed(int count){
       RedLed->SetCount(count);
  }
  void  Blinker::AddBlinkToYellow(int count){
     YellowLed->SetCount(count);
  }
  void  Blinker::AddBlinkToAll(int count){
    GreenLed->SetCount(count);
    RedLed->SetCount(count);
    YellowLed->SetCount(count);
    
    unsigned long syncTo = millis();
    GreenLed->SetLastBlink(syncTo);
    RedLed->SetLastBlink(syncTo);
    YellowLed->SetLastBlink(syncTo);
  }

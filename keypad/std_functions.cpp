#include <Arduino.h>
#include <EEPROM.h>

// setup the serial output 
void serialSetUp(){
    Serial.begin(9600);
    delay(1000);
    while(!Serial) { } // wait for the serial to initialize
    Serial.println("");
}

// display the setup complete message
void setupComplete()
{
    Serial.println("");
    Serial.println("Setup is complete");
}

// configure an output pin
void initOutputPin(int pinId, int initStatus)
{
    pinMode(pinId, OUTPUT);
    digitalWrite(pinId, initStatus);
}

// configure an output pin
void initOutputPin(int pinId)
{
    initOutputPin(pinId, LOW);    
}

// set al set of pins to a particular state
void setAllTo(const int pins[],int status, int arraySize)
{
  for(int i=0;i<arraySize;i++)
  {
    digitalWrite(pins[i], status);  
  }   
}

// https://roboticsbackend.com/arduino-store-array-into-eeprom/
//
// write a whole array into the empom
//
void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++) 
  {
    Serial.print("writing [");
    Serial.print(numbers[i]);
    Serial.print("] to [");
    Serial.print(addressIndex);
    Serial.println("]");
    
    EEPROM.write(addressIndex, numbers[i] >> 8);
    EEPROM.commit();
    EEPROM.write(addressIndex + 1, numbers[i] & 0xFF);
    EEPROM.commit();
        
    addressIndex +=2;
  } 
}

//https://roboticsbackend.com/arduino-store-array-into-eeprom/
//
// return a sum so that we know if we have any usable values
//
int readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  int sum = 0;
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {    
   numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
    Serial.print("read [");
    Serial.print(numbers[i]);
    Serial.print("] from [");
    Serial.print(addressIndex);
    Serial.println("]");    
    addressIndex +=2;
    sum += numbers[i];
  }
  return sum;
}

// blink a single LED
void blink(int pin, int delayDuration, int blinkCount)
{
  for(int blink=0;blink<blinkCount;blink++)
  {
     digitalWrite(pin, HIGH);  
     delay(delayDuration);
     digitalWrite(pin, LOW);  
     delay(delayDuration);
  }
}

// blink several LEDs
void blink(const int pins[], int delayDuration, int blinkCount, int arraySize)
{     
  // force all of the LEDs to be synchronized in case one is already high or low  
  setAllTo(pins, LOW, arraySize);

  // now do the blinking 
  for(int blink=0;blink<blinkCount;blink++)
  {
    setAllTo(pins, HIGH, arraySize);
    delay(delayDuration);
    setAllTo(pins, LOW, arraySize);
    delay(delayDuration);
  }
}

#include "Keypad.h"
#include "std_functions.h"
#include "blinker.h"
#include <EEPROM.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

const int arraySize = 4;
const int startingEepromAddress = 0;
int passCode[arraySize];
int newPassCode[arraySize];
int newAttempt2PassCode[arraySize];
bool useStdPassCode = true; // need for learning mode
bool learningMode = false;

const int redLed = 17;
const int greenLed = 16;
const int yellowLed = 4;
const int secondYellowLed = 25;
const int allStatusPinsCount = 4;
const int allStatusPins[allStatusPinsCount] = {redLed, greenLed, yellowLed, secondYellowLed};

const int buttonPin = 34;
const int relay_1 = 32;
const int relay_2 = 33;
const char star = '*';
const char hash = '#';

long buttonStartPress = 0;
long lastKeyPress = 0;
int keyPosition = 0;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {star,'0',hash}
};

byte rowPins[ROWS] = {21, 19, 18, 5}; 
byte colPins[COLS] = {23, 22, 3};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Blinker blinker = Blinker(greenLed, redLed, yellowLed, secondYellowLed);

void setup() {
  serialSetUp();
  EEPROM.begin(arraySize * 4);

  // initialize our status pins
  initOutputPin(redLed);
  initOutputPin(greenLed);
  initOutputPin(yellowLed);
  initOutputPin(secondYellowLed);
  initOutputPin(relay_1, HIGH);
  initOutputPin(relay_2, HIGH);
    
  pinMode(buttonPin, INPUT);
  
  //newPassCode[0] = newPassCode[1] = newPassCode[2] = newPassCode[3] = 9;
  //setPassCodeInEeprom(newPassCode);

  Serial.println("Loading passcode from EEPROM:");
  getPassCodeFromEeprom();

  Serial.print("Started: ");
  Serial.println(millis());

  setupComplete();
}

void loop() {

  char key = keypad.getKey();

  // every 30 seconds just reset the whole data entry setup
  if(30000 <  (millis() - lastKeyPress))
  {
    resetDataEntry();
  }

  handleLearningButtonPress();

  if (key){
    if(learningMode)
    {
      learningModeHandleKeyPress(key);
    }
    else
    {
      handleKeyPress(key);
    }
  }

    blinker.Update();
}

// 
// User is pressing the button to get into learning mode
//
void handleLearningButtonPress()
{
  int buttonPressed = digitalRead (buttonPin);
  if (HIGH == buttonPressed)
  {
     //Serial.println("button pressed!!");
     if(0 == buttonStartPress)
     {
        buttonStartPress = millis();
     }
     if(learningMode)
     {
        cancelLearningMode();
     }
  }
  else if(LOW == buttonPressed && 0 < buttonStartPress)
  {   
    buttonStartPress = 0;
  }

  if(0 < buttonStartPress && 5000 <  millis() - buttonStartPress)
  {
    Serial.println("start learning mode!");
    cancelLearningMode(); // just to reset stuff
    learningMode = true;   
    blinker.LearningMode = true;
    blinker.AddBlinkToYellow(5);   
  }
}

//
// user is using the key pad while in learning mode
//
void learningModeHandleKeyPress(char key)
{
    lastKeyPress = millis();
    Serial.print("[Learning Mode]: Got the key:");
    Serial.println(key);

    int setPosition = keyPosition%arraySize;
    
    if(star == key || hash == key)
    {
      if(useStdPassCode)
      {
        if(0 == setPosition && 0 < keyPosition)
        {
           Serial.println("switching to secondary array");
           digitalWrite(greenLed, HIGH);
           useStdPassCode = false;
           keyPosition = 0;
        }
        else
        {
          // error - they didn't enter enough numbers
          Serial.println("not enough numbers. error.");
          Serial.println(setPosition);
          blinker.AddBlinkToAll(5);
          cancelLearningMode();
        }       
      }// use std
      else
      {
        // need to compare the 2 arrays
        Serial.println("compare both arrays");
        bool match = true;
        for(int i=0;i<arraySize; i++){
          match = match && (newPassCode[i] == newAttempt2PassCode[i]);
        }

        if(match)
        {
          Serial.println("compare is good");
          blinker.AddBlinkToGreen(5);
          //blink(greenLed, 300, 5);
          setPassCodeInEeprom(newAttempt2PassCode);
          getPassCodeFromEeprom();
        }
        else
        {
          Serial.println("compare is bad");
          blinker.AddBlinkToRed(5);
          //blink(redLed, 300, 5);
        }

        cancelLearningMode();
      }
    }// star or hash
    else
    {
      int keyAsInt = key - '0'; // convert to an int
      Serial.print("Setting ");
      Serial.print(key);
      Serial.print(" to position ");
      Serial.println(setPosition);

      if(useStdPassCode)
      {
        Serial.println("primary passcode array");
        newPassCode[setPosition] = keyAsInt;
      }
      else
      {
        Serial.println("confirmation passcode array");
        newAttempt2PassCode[setPosition] = keyAsInt;
      }
      
      keyPosition++;
    }
}

// cancel learning mode
void cancelLearningMode()
{
  Serial.println("cancelling learning mode");
 // reset all LEDS
  setAllTo(allStatusPins, LOW, allStatusPinsCount); 

  // reset all of the flags
  blinker.LearningMode = learningMode = false;
  keyPosition = 0;
  useStdPassCode = true;
  buttonStartPress = 0;

  // reset the input arrays
  newPassCode[0] = newPassCode[1] = newPassCode[2] = newPassCode[3] = 0;
  newAttempt2PassCode[0] = newAttempt2PassCode[1] = newAttempt2PassCode[2] = newAttempt2PassCode[3] = 0;
  
}

// handle standard data entry key press
void handleKeyPress(char key)
{
    lastKeyPress = millis();
    Serial.print("[Std Mode]: Got the key:");
    Serial.println(key);

    if(star == key || hash == key)
    {
      Serial.println("got a hash or star - checking the passcode");
      if(checkPassCode())
      {
        Serial.println("open a door");
        int relayPin = star == key?relay_1:relay_2;
        openDoor(relayPin);
      }      
    }
    else
    {
      int setPosition = keyPosition%arraySize;
      int keyAsInt = key - '0';
      Serial.print("Setting ");
      Serial.print(key);
      Serial.print(" to position ");
      Serial.println(setPosition);
      blinker.AddBlinkToRed(1);
      //blink(greenLed, 300, 1);
      newPassCode[setPosition] = keyAsInt;
      keyPosition++;
    }
}

// compare the passcode to the user's inputted code
bool checkPassCode()
{
  bool match = true;
  for(int i=0;i<arraySize; i++){
    match = match && (newPassCode[i] == passCode[i]);
  }

  if(true == match)
  {
     Serial.println("Passcode matches");
     resetDataEntry();
     blinker.AddBlinkToGreen(5);
     //blink(greenLed, 300, 5);
  }
  else
  {
    Serial.println("Passcode does not match");
    resetDataEntry();
    blinker.AddBlinkToRed(5);
    //blink(redLed, 300, 5);
  }

  return match;
}

// open a garage door
void openDoor(int relayPin)
{
  // keep it opwn for 20 seconds
  digitalWrite(relayPin, LOW);
  Serial.print("Opening door on relay: ");
  Serial.println(relayPin);

  // blink all of the lights while this is happening
  blinker.AddBlinkToAll(10);

  // not worth making this one non-blocking
  delay(5000);
  
  // close it
  digitalWrite(relayPin, HIGH);
  Serial.print("closing door on relay: ");
  Serial.println(relayPin);
}

// reset all data data entry stuff
void resetDataEntry()
{    
    newPassCode[0] = newPassCode[1] = newPassCode[2] = newPassCode[3] = 0; 
    lastKeyPress = millis();
    keyPosition = 0;
}

// overwrite the values in the eepmrom
void setPassCodeInEeprom(int newPassCode[arraySize])
{
  writeIntArrayIntoEEPROM(startingEepromAddress, newPassCode, arraySize);
  delay(100);
}

// pull in the passcode values from the eeprom
void getPassCodeFromEeprom()
{    
   int readTotal = readIntArrayFromEEPROM(startingEepromAddress, passCode, arraySize);
   if(0 == readTotal)
   {
      Serial.println("passCode is all zeros - setting it to default - 9999");
      passCode[0] =  passCode[1] =  passCode[2] =  passCode[3] = 9;      
      writeIntArrayIntoEEPROM(startingEepromAddress, passCode, arraySize);
      delay(100);
   }else{
    Serial.print("passCode set to: ");
    Serial.print(passCode[0]);
    Serial.print(" ");
    Serial.print(passCode[1]);
    Serial.print(" ");
    Serial.print(passCode[2]);
    Serial.print(" ");
    Serial.println(passCode[3]);
   }
}

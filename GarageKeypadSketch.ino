/*
  May control my garage door using a keypad
*/

#include <Keypad.h>

// I want to keep the secret code in another file so that I can
// make the rest of the source code public.
// Include the secret code with:
#include "secretcode.h"
//which should define the secret like this:
//
//const byte CODELENGTH = 5;
//const String correctCode = "12345";

char currentCode[sizeof(correctCode)];
const char RESET_KEY = '*';

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

byte index = 0;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int ledPin =  12;

void setup() {
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  keypad.getKey();
}

void keypadEvent(KeypadEvent key) {
  if (keypad.getState() == PRESSED) {
    Serial.print(key);
    Serial.print(' ');
    Serial.println("PRESSED");
    RecordKey(key);
  }
}

void RecordKey(char key) {
  if (RESET_KEY == key) {
    index = 0;
    return;
  }

  currentCode[index] = key;
  index++;

  if (index == CODELENGTH) {
    checkCode();
    index = 0;
  }
}

void checkCode() {
  Serial.print("checking code: ");
  Serial.println(currentCode);

  if (correctCode.equals(currentCode)) {
    Serial.println("code accepted");
    OpenTheDoor();
  }
  else {
    Serial.println("code rejected");
  }
}

void OpenTheDoor() {
  Serial.println("closing relay . . .");
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  Serial.println("opening relay");
}

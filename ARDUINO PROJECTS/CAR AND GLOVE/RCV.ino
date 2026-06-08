/*
  תוכנית זו מקבלת הודעות מהמשדר ומניעה את המכונית בהתאם.
  מותאם ל-Arduino Uno.
  
  חיווט מנועים:
  מנוע שמאל: קדימה -> 2, אחורה -> 3
  מנוע ימין: קדימה -> 4, אחורה -> 5
  
  חיווט NRF24L01:
  VCC->3.3V, CE->7, CSN->8, SCK->13, MOSI->11, MISO->12
*/

#include <SPI.h>
#include "NRFLite.h"

// הגדרת פינים למנועים
const int leftMotorForward  = 2;
const int leftMotorBackward = 3;
const int rightMotorForward = 4;
const int rightMotorBackward = 5;

// הגדרות רדיו
const static uint8_t RADIO_ID = 0;   
const static uint8_t PIN_RADIO_CE  = 7;
const static uint8_t PIN_RADIO_CSN = 8;

struct RadioPacket {
  uint8_t FromRadioId;
  uint32_t Data;
  uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

int directionOfMovement = 0;
int lastCommand = -1; // משתנה למניעת הצפת הדפסות בסיריאל

void setup() {
  Serial.begin(9600);

  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1); 
  }
  
  Serial.println("Receiver Ready! Waiting for command...");
}

void loop() {
  // משתמשים ב-if במקום ב-while כדי למנוע היקשרות בלולאה אינסופית במקרה של הצפה
  if (_radio.hasData()) {
    _radio.readData(&_radioData);
    
    directionOfMovement = _radioData.Data;

    // מדפיסים רק אם הפקודה השתנתה - זה חוסך המון זמן מעבד ומונע תקיעות
    if (directionOfMovement != lastCommand) {
      Serial.print("New Command Received: ");
      Serial.println(directionOfMovement);
      moveAccordingly();
      lastCommand = directionOfMovement;
    }
  }
  
  // השהיה קטנה מאוד לייצוב המערכת
  delay(5); 
}

void moveAccordingly() {
  switch (directionOfMovement) {
    case 1: front(); break;
    case 2: back();  break;
    case 3: left();  break;
    case 4: right(); break;
    case 0: none();  break;
  }
}

void front() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void back() {
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorBackward, HIGH);
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);
}

void left() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void right() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void none() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}
/*
  תוכנית זו מזהה את מצב ההטיה של MPU6050 ושולחת ערך למקלט דרך NRF24L01.
  עודכן עבור Arduino Uno: נוסף אבחון עמוק לתקשורת I2C וניהול שגיאות.
  
  חיווט חובה ל-Arduino Uno:
  -----------------------
  MPU6050: VCC->5V, GND->GND, SDA->A4, SCL->A5
  NRF24L01: VCC->3.3V (בלבד!), CE->7, CSN->8, SCK->13, MOSI->11, MISO->12
*/

#include <Wire.h>
#include "TinyMPU6050.h"
#include <SPI.h>
#include "NRFLite.h"

MPU6050 mpu(Wire);

const static uint8_t RADIO_ID = 1;             
const static uint8_t DESTINATION_RADIO_ID = 0; 
const static uint8_t PIN_RADIO_CE = 7;
const static uint8_t PIN_RADIO_CSN = 8;

struct RadioPacket {
  uint8_t FromRadioId;
  uint32_t Data;
  uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setup() {
  Serial.begin(9600);
  
  // אתחול I2C עם המתנה לייצוב
  Wire.begin();
  delay(1000);

  Serial.println("\n=== SYSTEM STARTUP ===");

  // בדיקה פיזית של קו ה-I2C (סורק כתובות)
  Serial.print("Scanning I2C bus... ");
  Wire.beginTransmission(0x68); // הכתובת הסטנדרטית של MPU6050
  byte error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("FOUND MPU6050 (0x68)!");
  } else {
    Serial.println("NOT FOUND!");
    Serial.println("CRITICAL ERROR: Check SDA(A4) and SCL(A5) wires.");
    // לא עוצרים כאן כדי לאפשר למשתמש לראות את ההודעה
  }

  Serial.println("Initializing Library...");
  mpu.Initialize();
  
  Serial.println("Calibrating... Keep sensor STILL.");
  mpu.Calibrate();
  Serial.println("Calibration Done!");

  // אתחול NRF24L01
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("NRF24 Error: Check wiring of pins 7, 8, 11, 12, 13");
    while (1); 
  }

  _radioData.FromRadioId = RADIO_ID;
  Serial.println("Ready to send data.\n");
}

void loop() {
  mpu.Execute();

  long rawX = mpu.GetRawAccX();
  long rawY = mpu.GetRawAccY();

  // בדיקת תקינות נתונים (אם הכל 0 או 1-, החיישן התנתק)
  if (rawX == -1 && rawY == -1) {
    Serial.println("ERROR: Sensor Disconnected!");
    delay(500);
    return;
  }

  // לוגיקת תנועה רציפה (Continuous)
  if (rawX <= -8000) {
    _radioData.Data = 1; // קדימה
    Serial.print("FRONT | X: "); Serial.println(rawX);
  }
  else if (rawX >= 8000) {
    _radioData.Data = 2; // אחורה
    Serial.print("BACK | X: "); Serial.println(rawX);
  }
  else if (rawY <= -8000) {
    _radioData.Data = 3; // שמאלה
    Serial.print("LEFT | Y: "); Serial.println(rawY);
  }
  else if (rawY >= 8000) {
    _radioData.Data = 4; // ימינה
    Serial.print("RIGHT | Y: "); Serial.println(rawY);
  }
  else {
    _radioData.Data = 0; // עצירה
    Serial.print("NONE | X: "); Serial.print(rawX);
    Serial.print(" Y: "); Serial.println(rawY);
  }

  sendData();
}

void sendData() {
  // שליחה רציפה
  if (!_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData))) {
    Serial.println(" [No ACK from Receiver]");
    _radioData.FailedTxCount++;
  }
  delay(100); 
}
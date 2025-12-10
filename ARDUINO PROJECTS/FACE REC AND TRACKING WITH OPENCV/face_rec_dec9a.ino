#include <Servo.h>

Servo x, y;
int width = 640, height = 480; 
int xpos = 90, ypos = 90;  // נקודת התחלה
const int angle = 2 ;       // גודל הצעד

void setup() {
  Serial.begin(9600);
  x.attach(9);
  y.attach(10);
  
  // איפוס
  x.write(xpos);
  y.write(ypos);
}

void loop() {
  if (Serial.available() > 0)
  {
    if (Serial.read() == 'X')
    {
      int x_mid = Serial.parseInt();
      if (Serial.read() == 'Y') {
        int y_mid = Serial.parseInt(); 
        
        // --- ציר X ---
        if (x_mid > width / 2 + 30)
          xpos += angle;
        if (x_mid < width / 2 - 30)
          xpos -= angle;

        // --- ציר Y (הכיוון המקורי) ---
        // אם הפנים למעלה (ערך Y נמוך) -> מקטינים זווית
        if (y_mid < height / 2 - 30)
          ypos -= angle; 
          
        // אם הפנים למטה (ערך Y גבוה) -> מגדילים זווית
        if (y_mid > height / 2 + 30)
          ypos += angle;

        // --- הגבלות ---
        if (xpos >= 180) xpos = 180;
        else if (xpos <= 0) xpos = 0;
        
        if (ypos >= 180) ypos = 180;
        else if (ypos <= 0) ypos = 0;

        // הזזת המנועים
        x.write(xpos);
        y.write(ypos);
      }
    }
  }
}
/*
 * ===============================================
 * BALL AND BEAM - SUPER SMOOTH VERSION
 * Target: 26.5 cm
 * Updates:
 * 1. Average of 10 readings (Filters noise)
 * 2. PID Update every 40ms (Slower reflexes)
 * 3. Deadband +/- 1.5cm (Stops micro-adjustments)
 * ===============================================
 */

#include <Servo.h>
#include <PID_v1.h>

// --- Pins ---
const int SERVO_PIN = 9;
const int TRIG_PIN = 10;
const int ECHO_PIN = 11;

// --- Mechanical Setup ---
const int SERVO_CENTER = 95;      
const int SERVO_LIMIT_LOW = 45;   
const int SERVO_LIMIT_HIGH = 160; 

// --- PID Settings (LAZY TUNING) ---
// Lower Kp means it won't react violently.
// Lower Kd means it won't twitch on sensor noise.
double Kp = 4.0;    // Soft Reaction
double Ki = 0.20;   // Slow correction
double Kd = 1.0;    // Minimal braking (less twitch)

// --- Smoothing Settings ---
const int NUM_READINGS = 10; 
double readings[NUM_READINGS]; // Buffer for smoothing
int readIndex = 0;             // Current index
double total = 0;              // Running total
double average = 0;            // The result

// --- Variables ---
double Setpoint, Input, Output;
int currentServoPosition = SERVO_CENTER; 

Servo myServo;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_CENTER);

  // Initialize Filter Buffer with Target Value (26.5)
  // This prevents startup jumps.
  Setpoint = 26.5; 
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 26.5;
    total += 26.5;
  }
  Input = 26.5; 

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-50, 50); 
  
  // *** SMOOTHNESS UPGRADE 1: SLOW DOWN ***
  // Only calculate new angles every 40ms.
  myPID.SetSampleTime(40); 
}

void loop() {
  // 1. Get Heavily Smoothed Distance
  double dist = getSmoothedDistance();
  
  int desiredAngle = SERVO_CENTER;
  int maxStep = 2; // Default to VERY SLOW movement

  // ==========================================================
  // 2. RESCUE MODE (Blind Zone < 5cm)
  // ==========================================================
  if (dist < 5.0) { 
    desiredAngle = 160; 
    maxStep = 100; // Move Fast only for emergency
    Input = 0; 
    Serial.print("RESCUE! "); 
  }
  // ==========================================================
  // 3. NORMAL MODE
  // ==========================================================
  else {
    Input = dist;
    double error = abs(Setpoint - Input);

    // *** SMOOTHNESS UPGRADE 2: WIDER DEAD ZONE ***
    // If ball is within 1.5cm of target (25.0 to 28.0), STOP.
    if (error < 1.0) {
      desiredAngle = SERVO_CENTER; 
      Output = 0; 
      maxStep = 1; // Move 1 degree at a time (Imperceptible)
    }
    else {
      myPID.Compute();
      desiredAngle = SERVO_CENTER + Output;
      
      // Dynamic Speed: Only go fast if we are REALLY far away
      if (error > 10.0) maxStep = 10; 
      else maxStep = 2; // Otherwise, crawl.
    }
  }

  desiredAngle = constrain(desiredAngle, SERVO_LIMIT_LOW, SERVO_LIMIT_HIGH);

  // ==========================================================
  // 4. MOVE SERVO (Gentle Steps)
  // ==========================================================
  int diff = desiredAngle - currentServoPosition;
  
  if (diff > maxStep) currentServoPosition += maxStep;
  else if (diff < -maxStep) currentServoPosition -= maxStep;
  else currentServoPosition = desiredAngle;

  myServo.write(currentServoPosition);

  // Debugging
  Serial.print(Setpoint);
  Serial.print(",");
  Serial.print(Input);
  Serial.print(",");
  Serial.println(currentServoPosition); 
  
  // *** SMOOTHNESS UPGRADE 3: RELAX LOOP ***
  delay(30); 
}

// --- Helper: Read Sensor ---
double readSensorRaw() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 6000); 
  if (duration == 0) return 0; 
  return (duration * 0.0343) / 2.0;
}

// --- Helper: Heavy Smoothing (Average of 10) ---
double getSmoothedDistance() {
  double current = readSensorRaw();
  
  // Rescue Bypass: If 0, return 0 immediately
  if (current == 0) return 0.0; 

  // Subtract the last reading
  total = total - readings[readIndex];
  
  // Add the new reading
  readings[readIndex] = current;
  
  // Add to total
  total = total + readings[readIndex];
  
  // Advance index
  readIndex = readIndex + 1;
  if (readIndex >= NUM_READINGS) {
    readIndex = 0;
  }

  // Calculate Average
  return total / NUM_READINGS;
}
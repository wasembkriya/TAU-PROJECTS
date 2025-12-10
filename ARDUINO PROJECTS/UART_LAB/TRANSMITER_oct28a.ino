int ledPin = 7;      // IR LED connected to pin 7
String message = ""; // message from serial monitor
int bitTime = 2500;  // duration of one bit in microseconds

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);   // Idle = Logic '1' = IR OFF
  Serial.begin(9600);
  Serial.println("Enter a message to send via IR UART:");
}

void loop() {
  if (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');
    message.trim();
    
    Serial.print("Transmitting: ");
    Serial.println(message);
    
    delayMicroseconds(100); // Give receiver time

    unsigned long startTime = micros(); // Start total timing

    for (int i = 0; i < message.length(); i++) {
      sendUARTChar(message[i]);
    }
    sendUARTChar('\n');  // End of message

    unsigned long endTime = micros(); // End total timing

    unsigned long totalTime = endTime - startTime; // μs
    int totalBits = (message.length() + 1) * 11; // 11 bits per char
    float bps = 1000000.0 / (totalTime / (float)totalBits);

    Serial.print("Total message time: "); Serial.print(totalTime); Serial.println(" us");
    Serial.print("Effective bps: "); Serial.println(bps);
  }
}

void sendUARTChar(char c) {
  // START BIT
  digitalWrite(ledPin, HIGH); // Logic '0' = IR ON
  delayMicroseconds(bitTime);

  // DATA BITS
  byte b = c;
  int parity = 0;
  for (int i = 0; i < 8; i++) {
    int bitVal = b & 0x01;
    digitalWrite(ledPin, bitVal ? LOW : HIGH); // 1 = OFF, 0 = ON
    if (bitVal) parity ^= 1;
    b >>= 1;
    delayMicroseconds(bitTime);
  }

  // PARITY BIT (odd)
  digitalWrite(ledPin, parity ? HIGH : LOW);
  delayMicroseconds(bitTime);

  // STOP BIT
  digitalWrite(ledPin, LOW);
  delayMicroseconds(bitTime);
}

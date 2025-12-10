int sensorPin = A1;
int bitTime = 2500;     // MUST match transmitter
String receivedMessage = "";
#define THRESHOLD 700  // Between your idle and signal levels

void setup() {
  Serial.begin(9600);
  Serial.println("IR UART Receiver Ready");
  Serial.println("Waiting...");
}

void loop() {
  unsigned long startTime = 0;
  bool messageStarted = false;

  while (true) {
    char c = receiveUARTChar();
    if (c != 0) {
      if (!messageStarted) {
        messageStarted = true;  // First character received
        startTime = micros();
      }

      if (c == '\n') {
        unsigned long endTime = micros();
        unsigned long totalTime = endTime - startTime;
        int totalBits = (receivedMessage.length() + 1) * 11;
        float bps = 1000000.0 / (totalTime / (float)totalBits);

        Serial.print("Received message: ");
        Serial.println(receivedMessage);
        Serial.print("Total receive time: "); Serial.print(totalTime); Serial.println(" us");
        Serial.print("Effective bps: "); Serial.println(bps);

        receivedMessage = "";
        messageStarted = false;
        break; // message complete
      } else {
        receivedMessage += c;
      }
    }
  }
}

char receiveUARTChar() {
  int sensorValue;

  // Wait for START BIT (IR ON)
  while (true) {
    sensorValue = analogRead(sensorPin);
    if (sensorValue > THRESHOLD) {
      break;
    }
  }

  // Wait 1.5 bit times to sample middle of first data bit
  delayMicroseconds(bitTime + bitTime/2);

  byte received = 0;
  int parity = 0;

  // Read 8 DATA BITS (LSB first)
  for (int i = 0; i < 8; i++) {
    sensorValue = analogRead(sensorPin);
    int bitVal = (sensorValue > THRESHOLD) ? 0 : 1;
    received |= (bitVal << i);
    if (bitVal) parity ^= 1;
    delayMicroseconds(bitTime);
  }

  // Read PARITY BIT
  sensorValue = analogRead(sensorPin);
  int parityBit = (sensorValue > THRESHOLD) ? 0 : 1;
  delayMicroseconds(bitTime);

  // Read STOP BIT
  sensorValue = analogRead(sensorPin);
  int stopBit = (sensorValue > THRESHOLD) ? 0 : 1;

  if (stopBit != 1) {
    Serial.println("[ERR: Stop bit]");
    return 0;
  }

  if ((parity ^ parityBit) != 1) {
    Serial.println("[ERR: Parity]");
    return 0;
  }

  return char(received);
}

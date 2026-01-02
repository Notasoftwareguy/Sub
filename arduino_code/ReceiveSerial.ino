
int parseBinaryData(byte* values) {
  if (Serial.available() >= 2) {
    if (Serial.peek() == START_MARKER) {
      Serial.read(); // Consume header
      byte numBytes = Serial.read();
      
      if (numBytes > MAX_VALUES) numBytes = MAX_VALUES;

      // Wait for data with a small local timeout to prevent hanging
      unsigned long startWait = millis();
      while (Serial.available() < numBytes && millis() - startWait < 50); 

      if (Serial.available() >= numBytes) {
        for (int i = 0; i < numBytes; i++) {
          values[i] = Serial.read();
        }
        return numBytes;
      }
    } else {
      Serial.read(); // Clear buffer until we find a header
    }
  }
  return 0;
}
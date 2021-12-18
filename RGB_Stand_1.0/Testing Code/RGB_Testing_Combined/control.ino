void serialControl() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();
    if (incomingByte != '\n') {
      mode = incomingByte - '0';
      Serial.print("Mode is now: ");
      Serial.println(mode);
      //Serial.println(incomingByte);
      EEPROM.write(addr, mode);
    }
  }
}

void buttonControl() {
  bool buttonState = digitalRead(buttonPin);
  static unsigned long prevTime = 0;
  //Serial.println(buttonState);
  
  if (buttonState == LOW && millis() - prevTime >= 200) {
    mode++;
    mode %= 6;

    prevTime = millis();
  }
}

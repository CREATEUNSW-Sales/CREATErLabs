// Use serial monitor to send mode commands
void serialControl() {
  // If there is data in the input buffer
  if (Serial.available() > 0) {
    // Read the next incoming byte:
    int incomingByte = Serial.read();
    if (incomingByte != '\n') {
      // Change current mode
      changeMode(incomingByte - '0');
      Serial.print("Mode is now: ");
      Serial.println(mode);
      //Serial.println(incomingByte);
    }
  }
}

// Use button to sequentially switch modes
void buttonControl() {
  // Read if button is pressed
  bool buttonState = digitalRead(BUTTON_PIN);
  static unsigned long prevTime = 0;
  //Serial.println(buttonState);

  // If button is pressed (LOW) and not waiting for switch bouncing (200ms since previous detected press)
  if (buttonState == LOW && millis() - prevTime >= 200) {
    changeMode(mode + 1);

    prevTime = millis();
  }
}

void changeMode(int newMode) {
  if (mode != newMode) {
    // Use direct port addressing to customise Timer1 and Timer2 for modes 4 and 5
    if ((mode == 4 || mode == 5) && !(newMode == 4 || newMode == 5)) {
      disableCustomTimers();
    } else if (newMode == 4 || newMode == 5) {
      setupCustomTimers();
    }
    //Serial.println(mode);
    mode = newMode;
    // Set mode back to 0 once we cycle through all modes
    mode %= NUM_MODES;
    // Write new mode to EEPROM (non-volatile memory)
    EEPROM.write(addr, mode);
  }
}

// Read potentiometer and update brightness
void readPot() {
  // Read analog resistance of potentiometer
  int sensorVal = analogRead(A1);
  // Map to brightness value from 0 - 1023 to 0.0 - 1.0
  brightness = map(sensorVal, 0, 1023, 0, 1000) / 1000.0;
}

// Print current RGB values for debugging
void printRGB() {
  char buff[20];
  sprintf(buff,"R: %d, G: %d, B: %d",rgb[0],rgb[1],rgb[2]);
  Serial.println(buff);
}

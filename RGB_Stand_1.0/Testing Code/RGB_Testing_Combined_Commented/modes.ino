// Set all LEDs to the same singular colour
void blueSolid() {
  // Convert HSV (226, 1.0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(226, 1, brightness, rgb);
  // Need to subtract from 255 as LEDs are reversed due to being common anode
  analogWrite(R_PIN, 255 - rgb[0]);
  analogWrite(G_PIN, 255 - rgb[1]);
  analogWrite(B_PIN, 255 - rgb[2]);

  // Cycle through LEDs
  switchLEDs();
}

void whiteSolid() {
  // Convert HSV (0, 0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(0, 0, brightness, rgb);
  // Need to subtract from 255 as LEDs are reversed due to being common anode
  analogWrite(R_PIN, 255 - rgb[0]);
  analogWrite(G_PIN, 255 - rgb[1]);
  analogWrite(B_PIN, 255 - rgb[2]);

  // Cycle through LEDs
  switchLEDs();
}

void orangeSolid() {
  // Convert HSV (24, 1.0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(24, 1, brightness, rgb);
  // Need to subtract from 255 as LEDs are reversed due to being common anode
  analogWrite(R_PIN, 255 - rgb[0]);
  analogWrite(G_PIN, 255 - rgb[1]);
  analogWrite(B_PIN, 255 - rgb[2]);

  // Cycle through LEDs
  switchLEDs();
}

// Takes input for which colour to be set
void solid(double hue, double saturation, double value) {
  // Convert HSV (226, 1.0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(hue, saturation, value, rgb);
  // Need to subtract from 255 as LEDs are reversed due to being common anode
  analogWrite(R_PIN, 255 - rgb[0]);
  analogWrite(G_PIN, 255 - rgb[1]);
  analogWrite(B_PIN, 255 - rgb[2]);

  // Cycle through LEDs
  switchLEDs();
}

// Fade all LEDs with the same colours
void fadeAll() {
  static int hue = 0;
  static unsigned long currTime;
  static unsigned long prevTimeFade = 0;

  currTime = millis();

  // Cycle through hue at rate defined by FADE_INTERVAL (ms)
  if (currTime - prevTimeFade >= FADE_INTERVAL) {
    prevTimeFade = currTime;
    
    if (hue >= 360) {
      hue = 0;
    }

    // Convert HSV (0, 1.0, 1.0) to RGB and write to corresponding pins
    HsvToRgb(hue, 1, brightness, rgb);
    // Need to subtract from 255 as LEDs are reversed due to being common anode
    analogWrite(R_PIN, 255 - rgb[0]);
    analogWrite(G_PIN, 255 - rgb[1]);
    analogWrite(B_PIN, 255 - rgb[2]);

    hue++;
  }

  // Cycle through LEDs
  switchLEDs();
}


// Set all LEDs to different colours
void multiSolid() {
  // Convert HSV (226, 1.0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(226, 1, brightness, rgb);
  // No need to subtract as timer already setup to account for common anode
  // Timer1 Overflow ISR handles LED switching for us using defined arrays of colours
  colours[0][0] = rgb[0];
  colours[0][1] = rgb[1];
  colours[0][2] = rgb[2];
  HsvToRgb(0, 0, brightness, rgb);
  colours[1][0] = rgb[0];
  colours[1][1] = rgb[1];
  colours[1][2] = rgb[2];
  HsvToRgb(24, 1, brightness, rgb);
  colours[2][0] = rgb[0];
  colours[2][1] = rgb[1];
  colours[2][2] = rgb[2];
}

// Set all LEDs to different colours with input
void solidMultiple(double colours1[3], double colours2[3], double colours3[3]) {
  // Convert HSV (226, 1.0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(colours1[0], colours1[1], colours1[2], rgb);
  // No need to subtract as timer already setup to account for common anode
  // Timer1 Overflow ISR handles LED switching for us using defined arrays of colours
  colours[0][0] = rgb[0];
  colours[0][1] = rgb[1];
  colours[0][2] = rgb[2];
  HsvToRgb(colours2[0], colours2[1], colours2[2], rgb);
  colours[1][0] = rgb[0];
  colours[1][1] = rgb[1];
  colours[1][2] = rgb[2];
  HsvToRgb(colours3[0], colours3[1], colours3[2], rgb);
  colours[2][0] = rgb[0];
  colours[2][1] = rgb[1];
  colours[2][2] = rgb[2];
}

// Fade all LEDs with different colours
void multiFade() {
  // Keep track of each hue for each LED
  static int hue0 = 0;
  static int hue1 = 120;
  static int hue2 = 240;
  
  if (hue0 >= 360) {
    hue0 = 0;
  }
  if (hue1 >= 360) {
    hue1 = 0;
  }
  if (hue2 >= 360) {
    hue2 = 0;
  }

  // Convert HSV (226, 1.0, 1.0) to RGB and write to corresponding pins
  HsvToRgb(hue0, 1, brightness, rgb);
  // No need to subtract as timer already setup to account for common anode
  // Timer1 Overflow ISR handles LED switching for us using defined arrays of colours
  colours[0][0] = rgb[0];
  colours[0][1] = rgb[1];
  colours[0][2] = rgb[2];
  HsvToRgb(hue1, 1, brightness, rgb);
  colours[1][0] = rgb[0];
  colours[1][1] = rgb[1];
  colours[1][2] = rgb[2];
  HsvToRgb(hue2, 1, brightness, rgb);
  colours[2][0] = rgb[0];
  colours[2][1] = rgb[1];
  colours[2][2] = rgb[2];

  hue0++;
  hue1++;
  hue2++;

  // Cycle through hue at rate defined by FADE_INTERVAL (ms)
  _delay_ms(FADE_INTERVAL);
}


// Turn all LEDs off
void off() {
  // Turn all LEDs off
  digitalWrite(ANODE_1, LOW);
  digitalWrite(ANODE_2, LOW);
  digitalWrite(ANODE_3, LOW);
}

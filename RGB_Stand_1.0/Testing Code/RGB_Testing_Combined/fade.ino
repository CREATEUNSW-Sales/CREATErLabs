unsigned long currTime;
unsigned long prevTimeFade = 0;

void fadeTest() {
  static int hue = 0;

  currTime = millis();
  
  if (currTime - prevTimeFade >= fadeInterval) {
    prevTimeFade = currTime;
    
    if (hue >= 360) {
      hue = 0;
    }

    HsvToRgb(hue, 1, brightness, rgb);
    analogWrite(R, 255 - rgb[0]);
    analogWrite(G, 255 - rgb[1]);
    analogWrite(B, 255 - rgb[2]);

    hue++;
  }

  switchLEDs();
}

void switchLEDs() {
  static int currAnode = 1;
    
  currAnode++;
  if (currAnode > numAnodes) {
    currAnode = 1;
  }

  digitalWrite(anode1, LOW);
  digitalWrite(anode2, LOW);
  digitalWrite(anode3, LOW);
  
  switch (currAnode) {
    case 1:
      digitalWrite(anode1, HIGH);
      break;
    case 2:
      digitalWrite(anode2, HIGH);
      break;
    case 3:
      digitalWrite(anode3, HIGH);
      break;
    default:
      digitalWrite(anode1, LOW);
      digitalWrite(anode2, LOW);
      digitalWrite(anode3, LOW);
      break;
  }
}

// Input:  0 <= H < 360, 0 <= S/V <= 1
// Output: 0 <= RGB <= 255
void HsvToRgb(double hue, double saturation, double value, int* rgb) {
  double r, g, b;

  constrain(hue, 0, 359);
  constrain(saturation, 0, 1);
  constrain(value, 0, 1);
  
  double c = value * saturation;
  double x = c * (1 - fabs(fmod((hue / 60), 2) - 1));
  double m = value - c;

  switch (int(hue / 60)) {
    case 0:
      r = c + m;
      g = x + m;
      b = m;
      break;
    case 1:
      r = x + m;
      g = c + m;
      b = m;
      break;
    case 2:
      r = m;
      g = c + m;
      b = x + m;
      break;
    case 3:
      r = m;
      g = x + m;
      b = c + m;
      break;
    case 4:
      r = x + m;
      g = m;
      b = c + m;
      break;
    case 5:
      r = c + m;
      g = m;
      b = x + m;
      break;
  }
  
  rgb[0] = int(r * 255);
  rgb[1] = int(g * 255);
  rgb[2] = int(b * 255);
}

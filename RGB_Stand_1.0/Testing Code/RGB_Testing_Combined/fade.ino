unsigned long currTime;
unsigned long prevTimeSwitch = 0;
unsigned long prevTimeFade = 0;

void fadeTest() {
  static int r = 255;
  static int g = 150;
  static int b = 50;
  static bool rUp = false;
  static bool gUp = false;
  static bool bUp = false;

  if (r >= 255) {
    rUp = false;
  } else if (r <= 0) {
    rUp = true;
  }
  if (g >= 255) {
    gUp = false;
  } else if (g <= 0) {
    gUp = true;
  }
  if (g >= 255) {
    gUp = false;
  } else if (r <= 0) {
    gUp = true;
  }

  if (currTime - prevTimeFade >= fadeInterval) {
    prevTimeFade = currTime;
    
    if (rUp) {
      r++;
    } else {
      r--;
    }
    if (gUp) {
      g++;
    } else {
      g--;
    }
    if (bUp) {
      b++;
    } else {
      b--;
    }
    //Serial.println(g);
    analogWrite(R, r);
    analogWrite(G, g);
    analogWrite(B, b);
  }

  switchLEDs();
}

void switchLEDs() {
  static int currAnode = 1;

  currTime = millis();
  
  if (currTime - prevTimeSwitch >= switchInterval) {
    prevTimeSwitch = currTime;
    
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
}

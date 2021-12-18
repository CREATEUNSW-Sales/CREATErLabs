#define R 3
#define G 10
#define B 9
#define anode1 13
#define anode2 12
#define anode3 11

unsigned long currTime;
unsigned long prevTimeSwitch;
unsigned long prevTimeFade;

#define switchInterval 1
#define fadeInterval 10
#define numAnodes 3

void setup() {
  // put your setup code here, to run once:
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  pinMode(anode1, OUTPUT);
  pinMode(anode2, OUTPUT);
  pinMode(anode3, OUTPUT);

  digitalWrite(R, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(B, HIGH);

  digitalWrite(anode1, LOW);
  digitalWrite(anode2, LOW);
  digitalWrite(anode3, LOW);

  Serial.begin(9600);

  prevTimeSwitch = 0;
  prevTimeFade = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  static int r = 255;
  static int g = 150;
  static int b = 50;
  static bool rUp = false;
  static bool gUp = false;
  static bool bUp = false;
  static int currAnode = 1;

  currTime = millis();

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

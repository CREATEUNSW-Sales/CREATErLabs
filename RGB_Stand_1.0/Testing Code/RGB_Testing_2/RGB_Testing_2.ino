#define R 5
#define G 6
#define B 9
#define anode1 13
#define anode2 12
#define anode3 11

#define r1 194
#define g1 119
#define b1 38
#define r2 0
#define g2 150
#define b2 150
#define r3 0
#define g3 230
#define b3 230

unsigned long currTime;
unsigned long prevTimeSwitch;

#define switchInterval 5
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
}

void loop() {
  // put your main code here, to run repeatedly:
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
        analogWrite(R, r1);
        analogWrite(G, g1);
        analogWrite(B, b1);
        digitalWrite(anode1, HIGH);
        break;
      case 2:
        analogWrite(R, r2);
        analogWrite(G, g2);
        analogWrite(B, b2);
        digitalWrite(anode2, HIGH);
        break;
      case 3:
        analogWrite(R, r3);
        analogWrite(G, g3);
        analogWrite(B, b3);
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

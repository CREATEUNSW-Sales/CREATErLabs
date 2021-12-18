#include <EEPROM.h>

#define R 3
#define G 10
#define B 9
#define anode1 13
#define anode2 12
#define anode3 11
#define buttonPin 2

#define fadeInterval 10
#define switchInterval 1

#define numAnodes 3

int mode;
int addr = 0;

void serialControl();
void buttonControl();

void switchLEDs();
void blueSolid();
void whiteSolid();
void orangeSolid();

void fadeTest();

void setupCustomTimers();
void disableCustomTimers();
void multiColorTest();
void multiFadeTest();

void setup() {
  // put your setup code here, to run once:
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  pinMode(anode1, OUTPUT);
  pinMode(anode2, OUTPUT);
  pinMode(anode3, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
  
  mode = EEPROM.read(addr);

  if (mode == 4 || mode == 5) {
    setupCustomTimers();
  }
}

void loop() {
  static int prevMode = -1;
  if (prevMode != mode) {
    if ((prevMode == 4 || prevMode == 5) && !(mode == 4 || mode == 5)) {
      disableCustomTimers();
    } else if (mode == 4 || mode == 5) {
      setupCustomTimers();
    }

    prevMode = mode;
  }
  
  switch (mode) {
    case 0:
      blueSolid();
      break;
    case 1:
      whiteSolid();
      break;
    case 2:
      orangeSolid();
      break;
    case 3:
      fadeTest();
      break;
    case 4:
      multiColorTest();
      break;
    case 5:
      multiFadeTest();
      break;
    default:
      mode = 0;
      break;
  }

  buttonControl();
}

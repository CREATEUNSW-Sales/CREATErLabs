/*
  CREATOR LABS

  DESCRIPTION AND FUNCTIONALITY HERE:

  WEBSITE LINKS AND STUFF:
  https://www.instructables.com/LED-Multiplexing-101-6-and-16-RGB-LEDs-With-Just-a/
  https://www.arduino.cc/en/Hacking/PinMapping168
  https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

#include <EEPROM.h>
#include <math.h>

// Define pins being used
#define R_PIN 3
#define G_PIN 10
#define B_PIN 9
#define ANODE_1 13
#define ANODE_2 12
#define ANODE_3 11
#define BUTTON_PIN 2
#define POT_PIN A1

// Define how fast the LEDs fade (ms)
#define FADE_INTERVAL 20

// Define number of LEDs used
#define NUM_ANODES 3
// Define total number of modes
#define NUM_MODES 6

// Set true if potentiometer is connected for use as brightness control
bool BRIGHTNESS_CONTROL = false;

// Stores which mode we are in
int mode;
// Address in EEPROM that we are writing to
int addr = 0;
// Default brightness set at max
double brightness = 1;
int rgb[3] = {0};
// Variables to be used in interrupt service routine in modes 4/5
volatile int colours[3][3] = {
  {255, 100, 0},
  {255, 100, 0},
  {255, 100, 0}
};
volatile int count = 0;

//// Control Functions ////

// Use serial monitor to send mode commands
void serialControl();
// Use button to sequentially switch modes
void buttonControl();
void changeMode(int newMode);
// Read potentiometer and update brightness
void readPot();
// Print current RGB values for debugging
void printRGB();

//// Modes ////

// Set all LEDs to the same singular colour
void blueSolid();
void whiteSolid();
void orangeSolid();
// Fade all LEDs with the same colours
void fadeAll();
// Set all LEDs to different colours
void multiSolid();
// Fade all LEDs with different colours
void multiFade();

//// Helper Functions ////
// Convert HSV to RGB
void HsvToRgb(double hue, double saturation, double value, int* rgb);
// Cycle through LEDs being turned on
void switchLEDs();
// Direct port manipulation to setup timers and revert
void setupCustomTimers();
void disableCustomTimers();
// Timer1 overflow interrupt service routine
ISR (TIMER1_OVF_vect);


void setup() {
  // Setup pins
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  pinMode(ANODE_1, OUTPUT);
  pinMode(ANODE_2, OUTPUT);
  pinMode(ANODE_3, OUTPUT);
  
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  // Read previously stored mode from EEPROM (non-volatile memory)
  mode = EEPROM.read(addr);

  // Use direct port addressing to customise Timer1 and Timer2 for modes 4 and 5
  if (mode == 4 || mode == 5) {
    setupCustomTimers();
  }
}

void loop() {
  // Keep track of changes to the current mode
  static int prevMode = -1;
  

  // Define which function to use according to the mode
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
      fadeAll();
      break;
    case 4:
      multiSolid();
      break;
    case 5:
      multiFade();
      break;
    default:
      mode = 0;
      break;
  }

  buttonControl();
  //serialControl();

  // Check potentiometer to change brightness if required
  if (BRIGHTNESS_CONTROL) readPot();
}

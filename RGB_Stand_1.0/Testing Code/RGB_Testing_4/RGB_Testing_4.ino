// USING ATMEGA328P

//https://www.instructables.com/LED-Multiplexing-101-6-and-16-RGB-LEDs-With-Just-a/
//https://www.arduino.cc/en/Hacking/PinMapping168
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

#define R 3
#define G 10
#define B 9
#define anode1 13
#define anode2 12
#define anode3 11
#define resetPin 4

#define fadeInterval 10

volatile int colors[3][3] = {
  {255, 100, 0},
  {255, 100, 0},
  {255, 100, 0}
};
volatile int count = 0;

#define switchInterval 5

void setR(int val) {
  OCR2B = val;
}
void setG(int val) {
  OCR1B = val;
}
void setB(int val) {
  OCR1A = val;
}

// Timer1 overflow interrupt function
ISR (TIMER1_OVF_vect) {
  // First, switch off LEDs, as they will display the value buffered on previous interrupt
  PORTB &= ~0b00111000; // set pins 11, 12, 13 LOW
  
  // Turn on the right LED (write HIGH)
  switch (count) {
    case 0:
      PORTB |= 0b00100000;
      break;
    case 1:
      PORTB |= 0b00010000;
      break;
    case 2:
      PORTB |= 0b00001000;
      break;
    default:
      break;
  }
  
  // Increment the counter
  count++;
  count %= 3;
  
  // Write new values to the buffers
  setR(colors[count][0]);
  setG(colors[count][1]);
  setB(colors[count][2]);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  
  pinMode(anode1, OUTPUT);
  pinMode(anode2, OUTPUT);
  pinMode(anode3, OUTPUT);

  pinMode(resetPin, OUTPUT);

  cli();
  
  // Stop and synchronise timers
  GTCCR |= _BV(TSM) | _BV(PSRASY) | _BV(PSRSYNC);
  
  // Setup fast PWM
  // Timer2 Set Pin 3 (Red) on Compare Match | Fast PWM (WGM0 = 11)
  TCCR2A = _BV(COM2B0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  
  // Timer1 Set Pin 10/9 (Green/Blue) on Compare Match | Fast PWM 8 bit (WGM1 = 0101)
  TCCR1A = _BV(COM1A0) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1) | _BV(WGM10);
  TCCR1B = _BV(WGM12); // fast PWM cntd
  
  // Timer0 Fast PWM (WGM0 = 11)
  //TCCR0A = _BV(WGM01) | _BV(WGM00);
  
  // Set prescalers to 64
  //TCCR0B = _BV(CS01) | _BV(CS00); // prescaler = 64 (CS0 = 011)
  //TCCR1B |= _BV(CS11) | _BV(CS10); // prescaler = 64 (CS1 = 011) (keeping WGM bit intact)
  //TCCR2B = _BV(CS22); // prescaler = 64 (CS2 = 100) 
  
  // Set prescalers to 256
  //TCCR0B = _BV(CS02) ; // prescaler = 256 (CS0 = 100)
  TCCR1B |= _BV(CS12); // prescaler = 256 (CS1 = 100) (keeping WGM bit intact)
  TCCR2B = _BV(CS22) | _BV(CS21) ; // prescaler = 256 (CS2 = 110)
  
  // Set all analog outputs to HIGH
  OCR2A = 255;
  OCR1B = 255;
  OCR1A = 255;
  //OCR1B = 255;
  //OCR2A = 255;
  //OCR2B = 255;
  
  // Reset all counters
  //TCNT0 = 0;
  TCNT1H = 0; // high byte
  TCNT1L = 0; // low byte
  TCNT2 = 0;
  
  TIMSK1 |= _BV(TOIE1); // enable overflow interrupt on timer 1

  sei();
  
  GTCCR = 0; //start timers
}

void loop() {
  // Some jank fading. To be edited when I find something better
  static bool r1Up = false;
  static bool g1Up = true;
  static bool b1Up = false;
  static bool r2Up = false;
  static bool g2Up = true;
  static bool b2Up = false;
  static bool r3Up = false;
  static bool g3Up = true;
  static bool b3Up = false;

  if (colors[0][0] >= 255) r1Up = false;
  else if (colors[0][0] <= 0) r1Up = true;
  if (colors[0][1] >= 255) g1Up = false;
  else if (colors[0][1] <= 0) g1Up = true;
  if (colors[0][2] >= 255) b1Up = false;
  else if (colors[0][2] <= 0) b1Up = true;

  if (colors[1][0] >= 255) r2Up = false;
  else if (colors[0][0] <= 0) r2Up = true;
  if (colors[1][1] >= 255) g2Up = false;
  else if (colors[1][1] <= 0) g2Up = true;
  if (colors[1][2] >= 255) b1Up = false;
  else if (colors[1][2] <= 0) b2Up = true;

  if (colors[2][0] >= 255) r3Up = false;
  else if (colors[2][0] <= 0) r3Up = true;
  if (colors[2][1] >= 255) g3Up = false;
  else if (colors[2][1] <= 0) g3Up = true;
  if (colors[2][2] >= 255) b3Up = false;
  else if (colors[2][2] <= 0) b3Up = true;

  if (r1Up) colors[0][0]++;
  else colors[0][0]--;
  if (g1Up) colors[0][1]++;
  else colors[0][1]--;
  if (b1Up) colors[0][2]++;
  else colors[0][2]--;

  if (r2Up) colors[1][0]++;
  else colors[1][0]--;
  if (g2Up) colors[1][1]++;
  else colors[1][1]--;
  if (b2Up) colors[1][2]++;
  else colors[1][2]--;

  if (r3Up) colors[2][0]++;
  else colors[2][0]--;
  if (g3Up) colors[2][1]++;
  else colors[2][1]--;
  if (b3Up) colors[2][2]++;
  else colors[2][2]--;

  _delay_ms(fadeInterval);
}

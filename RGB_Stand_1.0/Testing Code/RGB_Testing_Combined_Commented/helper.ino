// Convert HSV to RGB
// Input:  0 <= H < 360, 0 <= S/V <= 1
// Output: 0 <= RGB <= 255
// https://www.rapidtables.com/convert/color/hsv-to-rgb.html
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

// Cycle through LEDs being turned on
void switchLEDs() {
  static int currAnode = 1;

  // Cycle through LEDs
  currAnode++;
  if (currAnode > NUM_ANODES) {
    currAnode = 1;
  }

  // Turn all LEDs off
  digitalWrite(ANODE_1, LOW);
  digitalWrite(ANODE_2, LOW);
  digitalWrite(ANODE_3, LOW);
  
  switch (currAnode) {
    case 1:
      digitalWrite(ANODE_1, HIGH);
      break;
    case 2:
      digitalWrite(ANODE_2, HIGH);
      break;
    case 3:
      digitalWrite(ANODE_3, HIGH);
      break;
    default:
      digitalWrite(ANODE_1, LOW);
      digitalWrite(ANODE_2, LOW);
      digitalWrite(ANODE_3, LOW);
      break;
  }
}


/////////////////////////////////////////////////////////////////////////////////////
///////////////////// DIRECT PORT MANIPULATION HELPER FUNCTIONS /////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// Set value for output compare register for Timer2 - B (output compare 2B is pin 3 (R))
// val is value between 0 and 255; corresponds with values used in analogWrite
void setR(int val) {
  OCR2B = val;
}
// Set value for output compare register for Timer1 - B (output compare 1B is pin 10 (G))
// val is value between 0 and 255; corresponds with values used in analogWrite
void setG(int val) {
  OCR1B = val;
}
// Set value for output compare register for Timer1 - A (output compare 1A is pin 9 (B))
// val is value between 0 and 255; corresponds with values used in analogWrite
void setB(int val) {
  OCR1A = val;
}

// Direct port manipulation to setup timers
void setupCustomTimers() {
  // Disable interrupts
  cli();
  
  // Stop and synchronise timers
  GTCCR |= _BV(TSM) | _BV(PSRASY) | _BV(PSRSYNC);
  
  // Setup Timer2 in Fast PWM mode (WGM2 = 0b11) - counts to TOP and then restarts from BOTTOM. TOP is defined as 0xFF
  // Set OC2B (Pin 3 (Red)) on Compare Match | Clear at BOTTOM. This means higher value in OCR2B results in less time that OC2B is set
  TCCR2A = _BV(COM2B0) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  
  // Setup Timer1 in 8-bit Fast PWM mode (WGM1 = 0b0101) - counts to TOP and then restarts from BOTTOM. TOP is defined as 0xFF
  // Set OC1A (Pin 9 (Blue)) on Compare Match | Clear at BOTTOM. This means higher value in OCR1A results in less time that OC1A is set
  // Set OC1B (Pin 10 (Green)) on Compare Match | Clear at BOTTOM. This means higher value in OCR1B results in less time that OC1B is set
  TCCR1A = _BV(COM1A0) | _BV(COM1A1) | _BV(COM1B0) | _BV(COM1B1) | _BV(WGM10);
  TCCR1B = _BV(WGM12); // fast PWM cntd
  
  // Set prescalers to 64 - Timers increment every 64 clock cycles
  //TCCR1B |= _BV(CS11) | _BV(CS10); // prescaler = 64 (CS1 = 011) (keeping WGM bit intact)
  //TCCR2B = _BV(CS22); // prescaler = 64 (CS2 = 100) 
  
  // Set prescalers to 256 - Timers increment every 256 clock cycles
  TCCR1B |= _BV(CS12); // prescaler = 256 (CS1 = 100) (keeping WGM bit intact)
  TCCR2B = _BV(CS22) | _BV(CS21) ; // prescaler = 256 (CS2 = 110)
  
  // Set all analog outputs to HIGH
  OCR2B = 255;
  OCR1B = 255;
  OCR1A = 255;
  
  // Reset all timer counters
  TCNT1H = 0; // high byte
  TCNT1L = 0; // low byte
  TCNT2 = 0;

  // Enable overflow interrupt on Timer1
  TIMSK1 |= _BV(TOIE1);

  // Enable interrupts
  sei();

  // Start timers
  GTCCR = 0;
}

// Direct port manipulation to reset timers to default values used by Arduino
void disableCustomTimers() {
  // Disable interrupts
  cli();

  // Set Timer2 to Phase Correct PWM Mode
  // PWM disconnected until analogWrite called
  TCCR2A = 1;
  // Set Timer1 to Phase Correct PWM Mode
  // PWM disconnected until analogWrite called
  TCCR1A = 1;
  // Set Timer1 prescaler to 64
  TCCR1B = 3;
  // Set Timer2 prescaler to 64
  TCCR2B = 4;

  // Disable overflow interrupt on timer 1
  TIMSK1 &= ~_BV(TOIE1); 

  // Enable interrupts
  sei();
}

// Timer1 overflow interrupt service routine
// Handles switching LEDs and setting colours. Having these in the same ISR synchronises colour changing and LED switching
ISR (TIMER1_OVF_vect) {
  // Switch off LEDs, as they will display the value buffered on previous interrupt
  PORTB &= ~0b00111000; // Set pins 11, 12, 13 LOW
  
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
  count %= NUM_ANODES;
  
  // Write new values to the corresponding output compare registers
  setR(colours[count][0]);
  setG(colours[count][1]);
  setB(colours[count][2]);
}

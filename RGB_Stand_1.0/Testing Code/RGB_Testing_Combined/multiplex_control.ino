volatile int colors[3][3] = {
  {255, 100, 0},
  {255, 100, 0},
  {255, 100, 0}
};
volatile int count = 0;

void setR(int val) {
  OCR2B = val;
}
void setG(int val) {
  OCR1B = val;
}
void setB(int val) {
  OCR1A = val;
}

void setupCustomTimers() {
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

void disableCustomTimers() {
  cli();

  TCCR2A = 1;
  TCCR1A = 1;
  TCCR1B = 3;
  TCCR2B = 4;
  
  TIMSK1 &= ~_BV(TOIE1); // disable overflow interrupt on timer 1
  
  sei();
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
  count %= numAnodes;
  
  // Write new values to the buffers
  setR(colors[count][0]);
  setG(colors[count][1]);
  setB(colors[count][2]);
}

void multiColorTest() {
  HsvToRgb(226, 1, brightness, rgb);
  colors[0][0] = rgb[0];
  colors[0][1] = rgb[1];
  colors[0][2] = rgb[2];
  HsvToRgb(0, 0, brightness, rgb);
  colors[1][0] = rgb[0];
  colors[1][1] = rgb[1];
  colors[1][2] = rgb[2];
  HsvToRgb(24, 1, brightness, rgb);
  colors[2][0] = rgb[0];
  colors[2][1] = rgb[1];
  colors[2][2] = rgb[2];
}

void multiFadeTest() {
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

  HsvToRgb(hue0, 1, brightness, rgb);
  colors[0][0] = rgb[0];
  colors[0][1] = rgb[1];
  colors[0][2] = rgb[2];
  HsvToRgb(hue1, 1, brightness, rgb);
  colors[1][0] = rgb[0];
  colors[1][1] = rgb[1];
  colors[1][2] = rgb[2];
  HsvToRgb(hue2, 1, brightness, rgb);
  colors[2][0] = rgb[0];
  colors[2][1] = rgb[1];
  colors[2][2] = rgb[2];

  hue0++;
  hue1++;
  hue2++;
  
  _delay_ms(fadeInterval);
}

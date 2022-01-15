#include <Tone.h>

Tone buzzer1;
Tone buzzer2;
Tone buzzer3;

int NOTE[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5
};

void checkNote(int i, int b[8], int pb[8], int &numPlay, int &buzznote1, int &buzznote2, int &buzznote3);

void setup() {
  // put your setup code here, to run once:
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  
  buzzer1.begin(2);
  buzzer2.begin(3);
  buzzer3.begin(4);

  //buzzer1.play(NOTE_C4,3000);
  //buzzer2.play(NOTE_C4,4000);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Static variables to keep track of previous states
  static int pb[8] = {HIGH};
  static int numPlay = 0;
  static int buzznote1 = 0;
  static int buzznote2 = 0;
  static int buzznote3 = 0;

  int b[8];
  // Read current states (LOW is pressed)
  b[0] = digitalRead(5);
  b[1] = digitalRead(6);
  b[2] = digitalRead(7);
  b[3] = digitalRead(8);
  b[4] = digitalRead(9);
  b[5] = digitalRead(10);
  b[6] = digitalRead(11);
  b[7] = digitalRead(12);
  
  for (int i = 0; i < 8; i++) {
    checkNote(i, b, pb, numPlay, buzznote1, buzznote2, buzznote3);
  }

  //Serial.println(numPlay);
}

void checkNote(int i, int b[8], int pb[8], int &numPlay, int &buzznote1, int &buzznote2, int &buzznote3) {
  if (b[i] == LOW && b[i] != pb[i]) {  // b1 is pressed
    pb[i] = b[i];
    if (numPlay == 0) { // Play b1 note on buzzer1
      buzzer1.play(NOTE[i]);
      buzznote1 = NOTE[i];
      numPlay++;
    } else if (numPlay == 1) {  // Play b1 note on buzzer1/buzzer2
      if (buzznote1 == 0) {
        buzzer1.play(NOTE[i]);
        buzznote1 = NOTE[i];
      } else if (buzznote2 == 0) {
        buzzer2.play(NOTE[i]);
        buzznote2 = NOTE[i];
      } else if (buzznote3 == 0) {
        buzzer3.play(NOTE[i]);
        buzznote3 = NOTE[i];
      }
      numPlay++;
    } else if (numPlay == 2) {
      if (buzznote1 == 0) {
        buzzer1.play(NOTE[i]);
        buzznote1 = NOTE[i];
      } else if (buzznote2 == 0) {
        buzzer2.play(NOTE[i]);
        buzznote2 = NOTE[i];
      } else if (buzznote3 == 0) {
        buzzer3.play(NOTE[i]);
        buzznote3 = NOTE[i];
      }
      numPlay++;
    }
  } else if (b[i] == HIGH && b[i] != pb[i]) {
    pb[i] = b[i];
    if (buzznote1 == NOTE[i]) {
      buzzer1.stop();
      buzznote1 = 0;
      numPlay--;
    } else if (buzznote2 == NOTE[i]) {
      buzzer2.stop();
      buzznote2 = 0;
      numPlay--;
    } else if (buzznote3 == NOTE[i]) {
      buzzer3.stop();
      buzznote3 = 0;
      numPlay--;
    }
  }
}

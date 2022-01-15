#include <SoftwareSerial.h>

#define rx 10
#define tx 11

SoftwareSerial mySerial(rx, tx);

#define ledPin 13
char state = 0;

#define A_PWM 5
#define A_DIR 3
#define B_PWM 9
#define B_IN1 6
#define B_IN2 7

#define SPEED_ADJUSTMENT 0.6

void drive(char dir, int motor_speed);

void setup() {
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(A_PWM,OUTPUT);
  pinMode(A_DIR,OUTPUT);
  pinMode(B_PWM,OUTPUT);
  pinMode(B_IN1,OUTPUT);
  pinMode(B_IN2,OUTPUT);
  // set the data rate for the SoftwareSerial port

  mySerial.begin(9600);
  Serial.begin(9600);

  drive('S', 0);
}

void loop() {
  static char currDir = 'S';
  static char currSpeed = '0';
  static int Speed = 0;
  
  if(mySerial.available() > 0){ // Checks whether data is comming from the serial port
    state = mySerial.read(); // Reads the data from the serial port
  }

  // Check direction
  if (
    (state == 'F' || state == 'B' || state == 'L' || state == 'R' || 
    state == 'G' || state == 'I' || state == 'H' || state == 'J' || 
    state == 'D' || state == 'S') && state != currDir
  ) {
    currDir = state;
    drive(currDir, Speed);
  }

  // Check speed
  if (
    (state == '0' || state == '1' || state == '2' || state == '3' || 
    state == '4' || state == '5' || state == '6' || state == '7' || 
    state == '8' || state == '9' || state == 'q') && state != currSpeed
  ) {
    currSpeed = state;

    if (currSpeed == 'q') {
      Speed = 100;
    } else {
      Speed = 10 * (currSpeed - '0');
    }

    drive(currDir, Speed);
  }
  
  if (state == 'w') {
    digitalWrite(ledPin, LOW); // Turn LED OFF
  } else if (state == 'W') {
    digitalWrite(ledPin, HIGH);
  } 
}

void drive(char dir, int motor_speed) {
  motor_speed = map(motor_speed, 0, 100, 0, 255);
  
  if (dir == 'F') {
    digitalWrite(A_DIR,HIGH);
    analogWrite(A_PWM,255-motor_speed*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed);
    digitalWrite(B_IN1,HIGH);
    digitalWrite(B_IN2,LOW);
    Serial.println("Driving Forward");
  } else if (dir == 'B') {
    digitalWrite(A_DIR,LOW);
    analogWrite(A_PWM,motor_speed*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed);
    digitalWrite(B_IN1,LOW);
    digitalWrite(B_IN2,HIGH);
    Serial.println("Driving Backward");
  } else if (dir == 'L') {
    digitalWrite(A_DIR,HIGH);
    analogWrite(A_PWM,255-motor_speed*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed);
    digitalWrite(B_IN1,LOW);
    digitalWrite(B_IN2,HIGH);
    Serial.println("Driving Left");
  } else if (dir == 'R') {
    digitalWrite(A_DIR,LOW);
    analogWrite(A_PWM,motor_speed*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed);
    digitalWrite(B_IN1,HIGH);
    digitalWrite(B_IN2,LOW);
    Serial.println("Driving Right");
  } else if (dir == 'G') {
    digitalWrite(A_DIR,HIGH);
    analogWrite(A_PWM,255-motor_speed*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed*0.5);
    digitalWrite(B_IN1,HIGH);
    digitalWrite(B_IN2,LOW);
    Serial.println("Driving Forward Right");
  } else if (dir == 'I') {
    digitalWrite(A_DIR,HIGH);
    analogWrite(A_PWM,255-motor_speed*0.5*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed);
    digitalWrite(B_IN1,HIGH);
    digitalWrite(B_IN2,LOW);
    Serial.println("Driving Forward Left");
  } else if (dir == 'H') {
    digitalWrite(A_DIR,LOW);
    analogWrite(A_PWM,motor_speed*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed*0.5);
    digitalWrite(B_IN1,LOW);
    digitalWrite(B_IN2,HIGH);
    Serial.println("Driving Backward Right");
  } else if (dir == 'J') {
    digitalWrite(A_DIR,LOW);
    analogWrite(A_PWM,motor_speed*0.5*SPEED_ADJUSTMENT);
    analogWrite(B_PWM,motor_speed);
    digitalWrite(B_IN1,LOW);
    digitalWrite(B_IN2,HIGH);
    Serial.println("Driving Backward Left");
  }

  if (dir == 'S') {
    digitalWrite(A_DIR,HIGH);
    analogWrite(A_PWM,255-0);
    analogWrite(B_PWM,0);
    digitalWrite(B_IN1,HIGH);
    digitalWrite(B_IN2,LOW);
    Serial.println("Stopping");
  }
}

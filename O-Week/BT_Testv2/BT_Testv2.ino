#include <SoftwareSerial.h>

#define rx 10
#define tx 11

SoftwareSerial mySerial(rx, tx);

#define ledPin 13
char state = 0;

#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENB 3

void drive(char dir, int motor_speed);

void setup() {
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
  
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
    Serial.println(state);
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
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENA,motor_speed);
    analogWrite(ENB,motor_speed);
    Serial.println("Driving Forward");
  } else if (dir == 'B') {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENA,motor_speed);
    analogWrite(ENB,motor_speed);
    Serial.println("Driving Backward");
  } else if (dir == 'L') {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENA,motor_speed);
    analogWrite(ENB,motor_speed);
    Serial.println("Driving Left");
  } else if (dir == 'R') {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENA,motor_speed);
    analogWrite(ENB,motor_speed);
    Serial.println("Driving Right");
  } else if (dir == 'G') {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENA,motor_speed);
    analogWrite(ENB,motor_speed*0.5);
    Serial.println("Driving Forward Right");
  } else if (dir == 'I') {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    analogWrite(ENA,motor_speed*0.5);
    analogWrite(ENB,motor_speed);
    Serial.println("Driving Forward Left");
  } else if (dir == 'H') {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENA,motor_speed);
    analogWrite(ENB,motor_speed*0.5);
    Serial.println("Driving Backward Right");
  } else if (dir == 'J') {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(ENA,motor_speed*0.5);
    analogWrite(ENB,motor_speed);
    Serial.println("Driving Backward Left");
  }

  if (dir == 'S') {
    analogWrite(ENA,0);
    analogWrite(ENB,0);
    Serial.println("Stopping");
  }
}

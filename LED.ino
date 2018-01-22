#include <Servo.h>
#include <Ultrasonic.h>
Servo leftServo;
Servo rightServo;
Ultrasonic sonic(9,10);
bool backLightState = true;
int buttonPin = 4;
int backLightPin = 5;
int photoPin = A5;
int trigPin = 9;
int echoPin = 10;
long duration;

int counter, counterMax, distanceCm;
int rightAdd = 0;
int leftAdd = 0;

void setup() {
  rightServo.attach(A4);
  leftServo.attach(A3);
  rightServo.write(89);
  leftServo.write(89);
  Serial.begin(9600);
  counter = 0;
  counterMax = 100;
  pinMode(backLightPin, OUTPUT);
  pinMode(3, OUTPUT);
}

void beep(unsigned char delayms){
//  for (int i = 1; i <= 254; i++) {
//    analogWrite(3, i);
//    Serial.println(i);
//    delay(25);
//  }
  analogWrite(3, 50);
}  

void putColor(int w) {
  analogWrite(7, w);
}

void loop() {
  distanceCm = sonic.distanceRead();
  
  int joyX = analogRead(A1);
  int joyY = analogRead(A2);
  int buttonState = 1 - digitalRead(buttonPin);
  int lightLevel = analogRead(photoPin);
  int mapY = map(joyY, 0, 1023, 80, 100);
  int mapX = map(joyX, 0, 1023, 80, 100);

  leftAdd = mapX - 89;
  rightAdd = mapX - 89;

  int rightServoVal = mapY + rightAdd;
  int leftServoVal = 89 + 89 - mapY + leftAdd;
  if (distanceCm == 0 || distanceCm > 10) {
    rightServo.write(rightServoVal);
    leftServo.write(leftServoVal);
    if (abs(89-rightServoVal) <= 1) { backLightState = true;}
    else { backLightState = false; }
  }
  else {
    if (rightServoVal > 89 && leftServoVal < 89) {
      rightServo.write(rightServoVal);
      leftServo.write(leftServoVal);
      if (abs(89-rightServoVal) <= 1) { backLightState = true;}
      else { backLightState = false; }
    } else {
      rightServo.write(89);
      leftServo.write(89);
      counter++;
      if (counter >= counterMax) {
        counter = 0;
        backLightState = !backLightState;
      }
    }
  }
  if (buttonState == HIGH) {
    beep(10);
  } else {
    analogWrite(3, 0);       // 0 turns it off
  }

  if (lightLevel >= 900) { putColor(255); }
  else { putColor(0); }
  digitalWrite(backLightPin, backLightState);
}


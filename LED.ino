#include <Servo.h>
#include <Ultrasonic.h>
Servo leftServo;
Servo rightServo;
Ultrasonic sonic1(9,10);
Ultrasonic sonic2(12,11);
bool backLightState = true;
int buttonPin = 4;
int backLightPin = 5;
int photoPin = A5;
long duration;

int counter, counterMax, distanceCm1, distanceCm2;
int rightAdd = 0;
int leftAdd = 0;

void setup() {
  rightServo.attach(A4);
  leftServo.attach(A3);
  rightServo.write(89);
  leftServo.write(89);
  Serial.begin(9600);
  counter = 0;
  counterMax = 50;
  pinMode(backLightPin, OUTPUT);
  pinMode(3, OUTPUT);
}

void beep(unsigned char delayms){
  analogWrite(3, 50);
}  

void putColor(int w) {
  analogWrite(7, w);
}

void loop() {
  distanceCm1 = sonic1.distanceRead();
  distanceCm2 = sonic2.distanceRead();

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
  boolean frontStop = true;
  boolean backStop = true;
  if (distanceCm1 == 0 || distanceCm1 > 10) { frontStop = false; }
  if (distanceCm2 == 0 || distanceCm2 > 10) { backStop = false; }
//  Serial.print("Front: ");
//  Serial.println(distanceCm1);
  Serial.print("Back: ");
  Serial.println(backStop);
  
  if (!frontStop & !backStop) {
    rightServo.write(rightServoVal);
    leftServo.write(leftServoVal);
    if (abs(89-rightServoVal) <= 1) { backLightState = true;}
    else { backLightState = false; }
  }
  else {
    if (frontStop) {
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
    } else if (backStop) {
      if (rightServoVal < 89 && leftServoVal > 89) {
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
  }
  if (buttonState == HIGH) {
    beep(10);
  } else {
    analogWrite(3, 0);
  }

  if (lightLevel >= 900) { putColor(255); }
  else { putColor(0); }
  digitalWrite(backLightPin, backLightState);
}


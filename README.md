// Soft Serial
#include <SoftwareSerial.h>
#define RxD 2
#define TxD 8

char recvChar;                                        //variable to store character received
String recvStr;                                       //variable to store the series of received character
SoftwareSerial slave(RxD, TxD);                       //Slave arduino acts as a serial communication device

// Servo
#include <Servo.h>
Servo vaultServo;  
Servo trapServo;

// Stepper Motor
#include "UCN5804.h"
#define motorSteps 200     
#define dirPin   6
#define stepPin  5
#define halfPin  4
#define phasePin 3
UCN5804 myStepper(motorSteps, dirPin, stepPin, halfPin, phasePin);

int potValue;
float potPercentage;
int time = 0;
bool vaultLockedState = true;
bool trapLockedState = true;
int toggleState;
char incomingByte;

const int potPin = A0;
const int led1 = 9;
const int led2 = 10;
const int led3 = 11;
const int led4 = 12;
const int led5 = 13;
const int togglePin = 7; // toggle button pin

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                                //start serial monitor, for communicating with the PC
  pinMode(RxD, INPUT);                               //set mode of receive pin (from bluetooth)
  pinMode(TxD, OUTPUT);                              //set mode of transmit pin (to bluetooth)
  slave.begin(9600);                                 //start the bluetooth serial "port"
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(togglePin, INPUT);
  myStepper.setSpeed(60);
}

int readPot(){
  potValue = analogRead(potPin);
  potPercentage = potValue/1023.0*100.0;
  return potPercentage;
}

void ledBar(int potPercentage){
  if (potPercentage >= 0 && potPercentage < 20){
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
    digitalWrite(led5,HIGH);}
  else if (potPercentage >= 20 && potPercentage < 40){
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
    digitalWrite(led5,HIGH);}
  else if (potPercentage >= 40 && potPercentage < 60){
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,HIGH);
    digitalWrite(led5,HIGH);}
  else if (potPercentage >= 60 && potPercentage < 80){
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,HIGH);}
  else {
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);}
}

void unlockVault(){
  int a = 1;
  slave.write(a);
  delay(1000);
  //vaultServo.write(45);
  vaultLockedState = false;
}

void celebration(){
  for (int i = 0; i <= 5; i++) {
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
    delay(200);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
    digitalWrite(led5,HIGH);
    delay(200);
  }
}

void lockvault(){
  int b = 2;
  slave.write(b);
  delay(1000);
  //vaultServo.write(135);
  vaultLockedState = true;
}

void trapActivation(){
  int b = 2;
  int c = 3;
  int d = 4;
  slave.write(b);
  //vaultServo.write(135);
  delay(3000);
  slave.write(c);
  //trapServo.write(20);
  delay(5000);
  myStepper.step(1100);
  delay(3000);
  slave.write(d);
  //trapServo.write(110);
  delay(3000);
  myStepper.step(-1100);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  potPercentage = readPot();
  delay(50);
  ledBar(potPercentage);
  time = 0;

  if (potPercentage < 80 && potPercentage >= 60){
    while (potPercentage < 80 && potPercentage >= 60 && vaultLockedState == true){
      potPercentage = readPot();
      delay(50);
      time += 50;
      if (time >= 5000){
        celebration();
        unlockVault();
        delay(5000);
        toggleState = digitalRead(togglePin);
        if (toggleState == 0){
          trapActivation();
          vaultLockedState = true;
          delay(3000);
        }
      }
    }
  }

  if ((potPercentage >= 80 || potPercentage < 60) && vaultLockedState == false){
    lockvault();
  }
}

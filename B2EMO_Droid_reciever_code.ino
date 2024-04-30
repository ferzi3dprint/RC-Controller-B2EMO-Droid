#include <SPI.h>
#include<nRF24L01.h>
#include <RF24.h>
#include <printf.h>
#include <Servo.h>
                                                                                                                                                                                                             
#define driveMotorsLeftEnA 5
#define driveMotorsLeftIn1 7
#define driveMotorsLeftIn2 6
#define driveMotorsRightEnB 3
#define driveMotorsRightIn3 4
#define driveMotorsRightIn4 2
#define gearMotorIn3 A1
#define gearMotorIn4 A0
#define gearMotorEnB 9
#define whiteLed A2
#define redLed A4
#define redLed1 A5

RF24 radio(8,10);

const byte address[6] = "00001";
unsigned long lastReceiveTime = 0;
unsigned long currentTime =0;

Servo servo1;

struct Data_Package{
  byte leftJoyX;
  byte leftJoyY;                              
  byte buttonUp;
  byte buttonDown;
  byte toggleSwitch;
  };

Data_Package data;

int leftJoyXValue;
int leftJoyYValue;
int buttonUpValue;
int buttonDownValue;
int motorSpeedDriveMotorsLeft=0;
int motorSpeedDriveMotorsRight=0;
int motorSpeedGearMotor=0;
int toggleSwitchValue=0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
radio.begin();
printf_begin();
radio.openReadingPipe(0,address);
radio.setPALevel(RF24_PA_MAX);
radio.startListening();
radio.printDetails();
radio.enableDynamicPayloads();
radio.setDataRate(RF24_1MBPS);
radio.setAutoAck(false);

pinMode(driveMotorsLeftEnA, OUTPUT);
pinMode(driveMotorsLeftIn1, OUTPUT);
pinMode(driveMotorsLeftIn2, OUTPUT);
pinMode(driveMotorsRightEnB, OUTPUT);
pinMode(driveMotorsRightIn3, OUTPUT);
pinMode(driveMotorsRightIn4, OUTPUT);
pinMode(gearMotorIn3, OUTPUT);
pinMode(gearMotorIn4, OUTPUT);
pinMode(gearMotorEnB, OUTPUT);
pinMode(whiteLed, OUTPUT);
pinMode(redLed, OUTPUT);
pinMode(redLed1, OUTPUT);
digitalWrite(gearMotorIn3, LOW);
digitalWrite(gearMotorIn4, LOW);
digitalWrite(driveMotorsLeftIn1, LOW);
digitalWrite(driveMotorsLeftIn2, LOW);
digitalWrite(driveMotorsRightIn3, LOW);
digitalWrite(driveMotorsRightIn4, LOW);
analogWrite(driveMotorsLeftEnA, 0);
analogWrite(driveMotorsRightEnB, 0);
analogWrite(gearMotorEnB, 0); 
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (radio.available()){
  char text[32] = "";
  radio.read(&text, sizeof(text));
  Serial.println(text);
  }*/
  
currentTime = millis();
if (currentTime - lastReceiveTime > 1000){
  resetData();
  }

if (radio.available()){
  radio.read(&data, sizeof(Data_Package));
  lastReceiveTime = millis();
  }
  
leftJoyXValue = data.leftJoyX;
leftJoyYValue = data.leftJoyY;
buttonUpValue = data.buttonUp;
buttonDownValue = data.buttonDown;
toggleSwitchValue = data.toggleSwitch;

/*Serial.println(leftJoyXValue);*/
/*Serial.println(buttonUpValue);
Serial.println(buttonDownValue);
Serial.println(toggleSwitchValue);*/

/*Drive*/
if (leftJoyXValue < 110){
   digitalWrite(driveMotorsLeftIn1, HIGH);
   digitalWrite(driveMotorsLeftIn2, LOW);
   digitalWrite(driveMotorsRightIn3, HIGH);
   digitalWrite(driveMotorsRightIn4, LOW);
   motorSpeedDriveMotorsLeft = map(leftJoyXValue, 110, 0, 0, 255);
   motorSpeedDriveMotorsRight = map(leftJoyXValue, 110, 0, 0, 255);
  }

else if (leftJoyXValue>140){
   digitalWrite(driveMotorsLeftIn1, LOW);
   digitalWrite(driveMotorsLeftIn2, HIGH);
   digitalWrite(driveMotorsRightIn3, LOW);
   digitalWrite(driveMotorsRightIn4, HIGH);
   motorSpeedDriveMotorsLeft = map(leftJoyXValue, 140, 255, 0, 255);
   motorSpeedDriveMotorsRight = map(leftJoyXValue, 140, 255, 0, 255);
  }

else {
  motorSpeedDriveMotorsLeft=0;
  motorSpeedDriveMotorsRight=0;
  }

/*Steering*/
if (leftJoyYValue < 110) {
  int yMapped = map(leftJoyYValue, 110,0,0,255);
  motorSpeedDriveMotorsLeft=motorSpeedDriveMotorsLeft + yMapped;
  motorSpeedDriveMotorsRight=motorSpeedDriveMotorsRight + yMapped;

   digitalWrite(driveMotorsLeftIn1, HIGH);
   digitalWrite(driveMotorsLeftIn2, LOW);
   digitalWrite(driveMotorsRightIn3, LOW);
   digitalWrite(driveMotorsRightIn4, HIGH);
  
  if(motorSpeedDriveMotorsLeft>255){
    motorSpeedDriveMotorsLeft=255;
    }
  if(motorSpeedDriveMotorsRight>255){
    motorSpeedDriveMotorsRight=255;
    }
  }

  if (leftJoyYValue > 140) {
  int yMapped = map(leftJoyYValue, 140,255,0,255);
  motorSpeedDriveMotorsLeft=motorSpeedDriveMotorsLeft + yMapped;
  motorSpeedDriveMotorsRight=motorSpeedDriveMotorsRight + yMapped;
  
   digitalWrite(driveMotorsLeftIn1, LOW);
   digitalWrite(driveMotorsLeftIn2, HIGH);
   digitalWrite(driveMotorsRightIn3, HIGH);
   digitalWrite(driveMotorsRightIn4, LOW);
  
  if(motorSpeedDriveMotorsLeft>255){
    motorSpeedDriveMotorsLeft=255;
    }
  if(motorSpeedDriveMotorsRight>255){
    motorSpeedDriveMotorsRight=255;
    }
  }

if (motorSpeedDriveMotorsLeft < 70) {
  motorSpeedDriveMotorsLeft = 0;
  }
if (motorSpeedDriveMotorsRight < 70) {
  motorSpeedDriveMotorsRight = 0;
  }
  
analogWrite(driveMotorsLeftEnA, motorSpeedDriveMotorsLeft);
analogWrite(driveMotorsRightEnB, motorSpeedDriveMotorsRight);

if (buttonUpValue == 0 && buttonDownValue == 1){
  digitalWrite(gearMotorIn3, LOW);
  digitalWrite(gearMotorIn4, HIGH);
  motorSpeedGearMotor=200;
  }
else if (buttonUpValue == 1 && buttonDownValue == 0){
  digitalWrite(gearMotorIn3, HIGH);
  digitalWrite(gearMotorIn4, LOW);
  motorSpeedGearMotor=200;
  }
 else{
    motorSpeedGearMotor=0;
  }

analogWrite(gearMotorEnB, motorSpeedGearMotor);  

if (toggleSwitchValue == 0){
  digitalWrite(redLed, HIGH);
  digitalWrite(redLed1, HIGH);
  digitalWrite(whiteLed, HIGH);
  }
else{
  digitalWrite(redLed, LOW);
  digitalWrite(redLed1, LOW);
  digitalWrite(whiteLed, LOW);
  }
  
}

void resetData(){
  data.leftJoyX=127;
  data.leftJoyY=127;
  data.buttonUp=1;
  data.buttonDown=1;
  data.toggleSwitch=0;
  }

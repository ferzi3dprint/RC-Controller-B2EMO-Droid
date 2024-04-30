#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <printf.h>

const int button_up=2;
const int button_down=3;
const int toggle_switch=9;

RF24 radio(8,10);

const byte address[6]= "00001";

struct Data_Package{
  byte leftJoyX;
  byte leftJoyY;
  byte buttonUp;
  byte buttonDown;
  byte toggleSwitch;
  };
  
Data_Package data;

void setup() {
Serial.begin(9600);
radio.begin();
printf_begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MIN);
radio.stopListening();
radio.printDetails();
radio.enableDynamicPayloads();
radio.setDataRate(RF24_1MBPS);
radio.setAutoAck(false);

pinMode(button_up, INPUT_PULLUP);
pinMode(button_down, INPUT_PULLUP);
pinMode(toggle_switch, INPUT_PULLUP);

data.leftJoyX=127;
data.leftJoyY=127;
data.buttonUp=1;
data.buttonDown=1;
data.toggleSwitch=0;
}

void loop() {
data.leftJoyX = map(analogRead(A0), 0, 1023, 0, 255);
data.leftJoyY = map(analogRead(A1), 0, 1023, 0, 255);
data.buttonUp = digitalRead(button_up);
data.buttonDown = digitalRead(button_down);
data.toggleSwitch = digitalRead(toggle_switch);

Serial.println(data.leftJoyX);
radio.write(&data,sizeof(Data_Package));
}

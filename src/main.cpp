#include <Arduino.h>
#include <RF24.h>

#define channel 100

#define pinCSN 9
#define pinCE 10
RF24 radio(pinCE,pinCSN);


#define leftControlGPIO 1
#define rightControlGPIO 2

byte dataSent;
byte leftOut, rightOut, last_leftOut, last_rightOut = 0;
int leftIn, rightIn = 0;
int max = 800;
int min = 250;
String str = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Hello Controller!");

  pinMode(leftControlGPIO,INPUT);
  pinMode(rightControlGPIO,INPUT);

  radio.begin();

  radio.setChannel(channel);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);

  radio.openWritingPipe(10101);

  radio.stopListening();

}

void loop() {
  delay(100);
  leftIn = analogRead(leftControlGPIO);
  rightIn = analogRead(rightControlGPIO);
  //Serial.println(str+"raw left value : " + leftIn +" , raw right value : " + rightIn);

  if (leftIn > max) {
    leftOut = byte(2);
  } else if (leftIn > min)
  {
    leftOut = byte(1);
  } else
  {
    leftOut = byte(0);
  }

  if (rightIn > max) {
    rightOut = byte(2);
  } else if (rightIn > min)
  {
    rightOut = byte(1);
  } else
  {
    rightOut = byte(0);
  }

  if ((rightOut != last_rightOut) || (leftOut != last_leftOut)) {
    Serial.println(str+"To send => left value : " + leftOut +" , right value : " + rightOut);
    dataSent = rightOut | (leftOut << 4);
    radio.write(&dataSent,sizeof(dataSent));
    //Serial.println(dataSent,BIN);

    last_leftOut  = leftOut;
    last_rightOut = rightOut;
  }
}
#include <Arduino.h>
#include <TNCTransceiver.h>

#define control_pin 23
#define TX_controlPin 22
#define AF_IN 0
#define verbros false
#define red_led 19
#define greren_led 20
#define blue_led 21

bool complete=false;
char s[50];
int num = 0;

char *data = "";

TNCTransceiver TNC(AF_IN,control_pin);


void setup() {
  pinMode(red_led,OUTPUT);
  pinMode(greren_led,OUTPUT);
  pinMode(blue_led,OUTPUT);
  Serial.begin(9600);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  Serial.println("Started");
  //cli();
}

void loop() {

  // if(digitalRead(control_pin)){
  //   digitalWrite(blue_led,0);
  //   digitalWrite(red_led,1);
  //   TNC.Transmit_stop();
  //   Serial.println("Receiving");
  //   if(TNC.receive(data)){
  //     digitalWrite(greren_led,1);
  //     Serial.println(data);delay(50);
  //     digitalWrite(greren_led,0);
  //     if(verbros)Serial.println("Stopped");
  //   }
    
  // }else{
    
  //   digitalWrite(red_led,0);
  //   TNC.Transmit_start();
  //   delay(100);
  //   digitalWrite(blue_led,1);
  //   //if(digitalRead(TX_controlPin)){
  //     Serial.println("Transmitting");
  //     TNC.modulate("GOOD!");
  //     digitalWrite(blue_led,0);
  //   //}
  // }
  if(Serial.available()){
    int i=0;
    while (Serial.available())
    {
      s[i] = Serial.read();
      i++;
    }
    complete = true;
  }
  if(complete==true){
    complete = false;
    digitalWrite(red_led,0);
    TNC.Transmit_start();
    delay(100);
    digitalWrite(blue_led,1);
      Serial.println("Transmitting");
      TNC.modulate(s);
      digitalWrite(blue_led,0);  
  }
  else if(digitalRead(control_pin)){
    digitalWrite(red_led,0);
    TNC.Transmit_start();
    
    digitalWrite(blue_led,1);
    Serial.println("Transmitting");
    num++;
    TNC.floatTocharArray(s,num);
    TNC.modulate(s);
    digitalWrite(blue_led,0); 
    delay(1000);   
  }
}
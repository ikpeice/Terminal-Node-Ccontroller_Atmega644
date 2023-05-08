#include <Arduino.h>
#include <TNCTransceiver.h>
#include <stdlib.h>

#define control_pin 23
#define TX_controlPin 22
#define AF_IN 0
#define verbros true
#define red_led 19
#define greren_led 20
#define blue_led 21

bool complete=false;
char s[500];
int num = 0;

char data[50];

String inputString="";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

TNCTransceiver TNC(AF_IN,control_pin);


void setup() {
  pinMode(red_led,OUTPUT);
  pinMode(greren_led,OUTPUT);
  pinMode(blue_led,OUTPUT);
  Serial.begin(9600);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  TNC.Transmit_start();
  Serial.println("Started");
  //cli();
}

void clear_buff(char *s,int siz){
  for(int i=0;i<siz;i++){
    s[i]='\0';
  }
}

int len[500];
void loop() {
  digitalWrite(red_led,1);


    if(TNC.receive(data)){
      digitalWrite(greren_led,1);
      Serial.println(data);delay(100);
      digitalWrite(greren_led,0);
      clear_buff(data,50);
      if(verbros)Serial.println("Stopped");
    }
    
//    Transmitter


  if(stringComplete==true){
    digitalWrite(red_led,0);
    stringComplete = false;
    Serial.println("Received= "+inputString);
    inputString.toCharArray(s,inputString.length()+1);
    for(int i=0;i<500;i++){
      Serial.print(s[i]);
    }
    
    digitalWrite(blue_led,1);
    TNC.Transmit_start();
    delay(100); 
    Serial.println("Transmitting");
    TNC.modulate(s);
    clear_buff(s,500);
    delay(100);
    //TNC.Transmit_stop();
    digitalWrite(blue_led,0);  
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    //char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString = Serial.readString();
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    //  if (inChar == '\n') {
    //    stringComplete = true;
    //  }
  }
  stringComplete = true;
}
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

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

TNCTransceiver TNC(AF_IN,control_pin);


void setup() {
  pinMode(red_led,OUTPUT);
  pinMode(greren_led,OUTPUT);
  pinMode(blue_led,OUTPUT);
  Serial.begin(9600);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  //TNC.Transmit_start();
  TNC.Transmit_stop();
  Serial.println("Started");
  //cli();
}

void clear_buff(){
  for(int i=0;i<50;i++){
    s[i]='\0';
  }
}

void loop() {

  // if(digitalRead(control_pin)){
  //   digitalWrite(blue_led,0);
  //   digitalWrite(red_led,1);
    
//    RECEIVER


    if(TNC.receive(data)){
      digitalWrite(greren_led,1);
      Serial.println(data);delay(500);
      digitalWrite(greren_led,0);
      data = "";
      if(verbros)Serial.println("Stopped");
    }
    
//    Transmitter


  // if(stringComplete==true){
  //   stringComplete = false;
  //   Serial.print("Received= "+inputString);
  //   inputString.toCharArray(s,inputString.length()+1);
  //   for(int i=0;i<50;i++){
  //     Serial.print(s[i]);
  //   }
  //   digitalWrite(red_led,0);
  //   digitalWrite(blue_led,1);
  //   TNC.Transmit_start();
  //   delay(100); 
  //   Serial.println("Transmitting");
  //   TNC.modulate(s);
  //   clear_buff();
  //   delay(1000);
  //   digitalWrite(blue_led,0);  
  // }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    //char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString  = Serial.readString();//+= inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    // if (inChar == '\n') {
    //   stringComplete = true;
    // }
    stringComplete = true;
  }
  
}
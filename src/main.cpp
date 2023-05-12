#include <Arduino.h>
#include <TNCTransceiver.h>
#include <stdlib.h>


#define control_pin 23
#define TX_controlPin 22
#define AF_IN 0
#define verbros false
#define red_led 19
#define greren_led 20
#define blue_led 21

bool complete=false;
int num = 0;

char incoming_data[200];
char inChar;
String inputString="";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

TNCTransceiver TNC(AF_IN,control_pin);


void clear_buff(char *s,int size){
  for(int i=0;i<size;i++){
    s[i]='\0';
  }
}

void setup() {
  pinMode(red_led,OUTPUT);
  pinMode(greren_led,OUTPUT);
  pinMode(blue_led,OUTPUT);
  Serial.begin(9600);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  
  Serial.println("Started");
  TNC.set_destination_add("WIDE1-7");
  TNC.set_source_add("XYZSp-1");
  TNC.set_digipeater_add("Ideal_space_tech-01",0);
  TNC.set_FCS("FG");
  TNC.stop_transmitter();
  clear_buff(incoming_data,200);
}



void loop() {
  
  digitalWrite(red_led,1);


    if(TNC.receive(incoming_data)){
      //digitalWrite(greren_led,1);
      //Serial.println(strlen(incoming_data));
      for(unsigned int i=0;i<strlen(incoming_data);i++){
        Serial.print(incoming_data[i]);
      }
      Serial.println(" ");

      
      clear_buff(incoming_data,200);
      //if(verbros)Serial.println("Stopped");
    }
    
//    Transmitter


  if(stringComplete==true){
    digitalWrite(red_led,0);
    stringComplete = false;
    Serial.println("Received= "+inputString);
    inputString.toCharArray(incoming_data,inputString.length()+1);
    inputString="";
    for(int i=0;i<strlen(incoming_data);i++){
      Serial.print(incoming_data[i]);
    }
    TNC.set_info(incoming_data);
    
    digitalWrite(blue_led,1);
    TNC.start_transmitter();
    delay(100); 

    Serial.println("Transmitting");
    TNC.Transmit_packet();
    clear_buff(incoming_data,200);
    delay(100);
    TNC.stop_transmitter();
    digitalWrite(blue_led,0);  
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
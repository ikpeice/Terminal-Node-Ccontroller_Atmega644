#include <Arduino.h>
#include <TNCTransceiver.h>

#define control_pin 23
#define TX_controlPin 22
#define AF_IN 0
#define verbros true

char *data = "";

TNCTransceiver TNC(AF_IN,control_pin);


void setup() {
  Serial.begin(9600);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  Serial.println("Started");
  //cli();
}

void loop() {

  if(digitalRead(control_pin)){
    TNC.Transmit_stop();
    Serial.println("Receiving");
    if(TNC.receive(data)){
      Serial.println(data);
    }
    if(verbros)Serial.println("Stopped");
  }else{
    TNC.Transmit_start();
    delay(100);
    //if(digitalRead(TX_controlPin)){
      Serial.println("Transmitting");
      TNC.modulate("GOOD!");
    //}
  }

}
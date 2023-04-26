#include <Arduino.h>
#include <TNCTransceiver.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(24,25,26,27,28,29);

#define control_pin 23
#define TX_controlPin 22
#define AF_IN 0
#define verbros false
#define red_led 19
#define greren_led 20
#define blue_led 21

char *data = "";

TNCTransceiver TNC(AF_IN,control_pin);


void setup() {
  pinMode(red_led,OUTPUT);
  pinMode(greren_led,OUTPUT);
  pinMode(blue_led,OUTPUT);
  Serial.begin(9600);
  lcd.begin(16,2);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  Serial.println("Started");
  lcd.print("Started");
  delay(100);
}

void loop() {

  if(digitalRead(control_pin)){
    digitalWrite(blue_led,0);
    digitalWrite(red_led,1);
    TNC.Transmit_stop();
    Serial.println("Receiving");
    if(TNC.receive(data)){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Received:");
      digitalWrite(greren_led,1);
      Serial.println(data);delay(50);
      digitalWrite(greren_led,0);
      lcd.setCursor(0,1);
      lcd.print(data);      
      if(verbros)Serial.println("Stopped");
    }
    
  }else{
    
    digitalWrite(red_led,0);
    TNC.Transmit_start();
    delay(100);
    digitalWrite(blue_led,1);
    //if(digitalRead(TX_controlPin)){
      Serial.println("Transmitting");
      TNC.modulate("GOOD!");
      digitalWrite(blue_led,0);
    //}
  }

}
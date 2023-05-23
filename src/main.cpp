#include <Arduino.h>
#include <TNCTransceiver.h>
#include <stdlib.h>


#define control_pin 23
#define TX_controlPin 22
#define AF_IN 0
#define verbros false
#define verbros2 false
#define red_led 19
#define greren_led 20
#define blue_led 21

bool complete=false;
int num = 0;

char incoming_data[50];
char packet[150];
bool flag_packet=false;
int cnt=0;
char inChar;
String inputString="";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

TNCTransceiver TNC(AF_IN,control_pin);

struct Recv_packet{
  char destination[7];
  char source[7];
  char digipeater_add[56];
  char command[50];
  char control_field;
  char protocol_id;
  char FCS[2];
};

Recv_packet my_packet;

void clear_buff(char *s,int size){
  for(int i=0;i<size;i++){
    s[i]='\0';
  }
}

void decode_packet(char *packet,int size, Recv_packet *recv_p){
  clear_buff(recv_p->command,50);
  clear_buff(recv_p->destination,7);
  clear_buff(recv_p->source,7);
  clear_buff(recv_p->digipeater_add,56);
  int cnt=0;
  int i=0,x=0,y=0;
  char c;
  for(int i=0;i<size;i++){
    c = packet[i];
    if(c=='>'){
      x++;
      y=0;
      continue;
    }
    if(x==0){
      recv_p->destination[y] = c;
      y++;
    }
    if(x==1){
      recv_p->source[y] = c;
      y++;
    }
    if(x==2){
      recv_p->digipeater_add[y] = c;
      y++;
    }
    if(x==3){
      recv_p->control_field = c;
      y++;
    }
    if(x==4){
      recv_p->protocol_id = c;
      y++;
    }
    if(x==5){
      recv_p->command[y] = c;
      y++;
    }
    if(x==6){
      recv_p->FCS[y] = c;
      y++;
    }
  }

  long sum=0;
  for(int i=0;i<size-2;i++){
    sum += packet[i];
    if(verbros2)Serial.print(packet[i]);
  }
  //sum += (0x7e)*2;
  if((sum>>8 & 0xff)==(uint8_t)recv_p->FCS[0] && (sum&0xff)==(uint8_t)recv_p->FCS[1]){
    if(verbros2)Serial.println("Packet correct!!");
    Serial.print(String(recv_p->command));
  }else{
    if(verbros2)Serial.println("Packet not correct!!");
  }


}

void setup() {
  pinMode(red_led,OUTPUT);
  pinMode(greren_led,OUTPUT);
  pinMode(blue_led,OUTPUT);
  Serial.begin(2400);
  TNC.begin(verbros);
  pinMode(TX_controlPin,INPUT);
  TNC.stop_transmitter();
  if(verbros2)Serial.println("Started");
  TNC.set_destination_add("WIDE1-7");
  TNC.set_source_add("XYZSp-1");
  TNC.set_digipeater_add("N-SPACE-001",0);
  
  clear_buff(incoming_data,50);
  clear_buff(packet,150);
}



void loop() {
  
  digitalWrite(red_led,1);

  if(Serial.available()){
    inputString = Serial.readString();
    stringComplete = true;
  }


    if(TNC.receive(incoming_data)){

      for(unsigned int i=0;i<strlen(incoming_data);i++){
        if(incoming_data[i]=='~' && flag_packet==false){
          flag_packet=true;
          continue;
        }
        if(incoming_data[i]=='~' && flag_packet==true){
          flag_packet=false;
          if(verbros2)Serial.println(String(packet));
          decode_packet(packet,cnt,&my_packet);

          clear_buff(packet,150);
          cnt = 0;
          continue;
        }
        if(flag_packet==true){
          packet[cnt] = incoming_data[i];
          cnt++;
        }
        //Serial.print(incoming_data[i]);
      }
      //Serial.println(" ");

      
      clear_buff(incoming_data,50);
      //if(verbros)Serial.println("Stopped");
    }
    
//    Transmitter


  if(stringComplete==true){
    digitalWrite(red_led,0);
    stringComplete = false;
    
    inputString.toCharArray(incoming_data,inputString.length()+1);
    inputString="";
    if(verbros2)Serial.print("Received= ");
    if(verbros2)Serial.println(String(incoming_data));

    TNC.set_info(incoming_data);
    
    digitalWrite(blue_led,1);
    TNC.start_transmitter();
    delay(100); 

    if(verbros2)Serial.println("Transmitting");
    TNC.Transmit_packet();
    clear_buff(incoming_data,50);
    delay(100);
    TNC.stop_transmitter();
    digitalWrite(blue_led,0);  
  }

}

// void serialEvent() {
//     char inChar = (char)Serial.read();
//     if (inChar == '\n') {
//       stringComplete = true;
//     }else{
//       inputString += inChar;
//     }
// }
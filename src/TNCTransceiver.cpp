
#include <TNCTransceiver.h>

volatile bool dt = true, ct = true;
volatile bool msg = true;

#define counterRegister 320
const int steps = counterRegister/2;
volatile unsigned int counter = 0;
int *dutyCycle_A= new int[steps];
int *dutyCycle_B= new int[steps];

ISR(TIMER1_OVF_vect){
  
    if(msg==false && ct==false){
      OCR1B = dutyCycle_B[counter]; // low freq
    }else if (msg==true && ct == false)
    {
      OCR1B = dutyCycle_A[counter];  // high freq
    }else{
      OCR1B = dutyCycle_A[counter];
    }
    OCR1A = counterRegister/2;//dutyCycle_A[counter];
    counter++;
    if(counter==(steps/4)){ // detect zero crossing
      counter=0;
      ct = true;
    }

}


TNCTransceiver::TNCTransceiver(int AF_pin, int control_pin):
AF_pin(AF_pin),control_pin(control_pin)
{
}

TNCTransceiver::~TNCTransceiver()
{
}

void TNCTransceiver::begin(bool verbros){
    pinMode(AF_pin,INPUT);
    pinMode(control_pin,INPUT);
    debug = verbros;
  for(int i=0; i<steps/4; i++){
    dutyCycle_A[i] = counterRegister*solve(TetaToradian(teta(i)),frequency_A); 
    dutyCycle_B[i] = counterRegister*solve(TetaToradian(teta(i)),frequency_B); 
  }

  if(debug)Serial.println("Setup duty done!");
  delay(1);
  
  setup_freq();
  Transmit_stop();
  if(debug)Serial.println("Setup frequency done!");
}

char TNCTransceiver::bits_to_char(bool buffer[], int buff_size){
  char s=0x00; // = new char[num_of_char];
  char x = 0x01;
    for(int j=buff_size;j>0;j--){
      s = s | (buffer[j-1] * x);
      x = x << 1;
    }
  if(debug){
    Serial.println(s);
  }
  return s;
}

char  TNCTransceiver::decode(){
  //cli();
    period = pulseIn(AF_pin,HIGH);
    if(period>450 && period<550){
      bit = 1;
    }else if(period>180 && period<280){
      period = pulseIn(AF_pin,HIGH);
      if(period>180 && period<280){
         bit = 0;
      }else{
        bit =  -1;
      }
    }
    else{
      bit = -1;
     }
    //sei();
    return bit;
  
}
bool TNCTransceiver::receive(char *msg){
  char bit=1;
  int counter=0;
  int bit_counter=0;
  int no_of_char=0;
  while(bit){ // wait for start bit
    bit = decode();
    if(bit<0){
      //if(debug)Serial.println("No signal");
      return false; //digitalRead(control_pin) ||  terminate if control pin is low
    }
  }
  flag = false;
  while(1){

    while (flag==false)
    {
      bit = decode();
      if(bit>=0){
        buffer[counter] = bit;
      }else{
        if(debug)Serial.println("Error!");
        counter = 0;
        return false;
      }
      counter++;
      bit_counter++;
      if(bit_counter==8){
        msg[no_of_char] = bits_to_char(buffer, bit_counter);
        no_of_char++;
        bit_counter=0; // clear
        flag=true;
      }
    }
    
    bit = decode();
    if(bit==1 && decode()==0){
      flag = false;
    }else{
      if(debug)Serial.println("end!");
      break;
    }
  }
  return true;
  
}


void TNCTransceiver::setup_freq(){
  cli(); // stop global interrupt
      // Register initilisation, see datasheet for more detail.
    TCCR1A = 0b10100010;
       /*10 clear on match, set at BOTTOM for compA.
         10 clear on match, set at BOTTOM for compB.
         00
         10 WGM1 1:0 for waveform 15.
       */
    TCCR1B = 0b00011001;
       /*000
         11 WGM1 3:2 for waveform 15.
         001 no prescale on the counter.
       */
    //TIMSK1 = 0b00000001;
       /*0000000
         1 TOV1 Flag interrupt enable. 
       */
    ICR1   = counterRegister;     // Period for 16MHz crystal, for a switching frequency of 100KHz for 400 subdevisions per 50Hz sin wave cycle.
    sei();             // Enable global interrupts.
    //DDRB = 0b11111111; // Set PB1 and PB2 as outputs.
    pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
}
char TNCTransceiver::intTochar(int num) {
	switch (num)
	{
    case 0:
      return '0';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    default:
      break;
	}
}

int TNCTransceiver::len(char s[]){
  int c=0;
  while(s[c]){
    c++;
  }
  return c;
}

void TNCTransceiver::floatTocharArray(char *s, double num) {
	int temp = 0;
	temp = num / 1000;
	s[0] = intTochar(temp);
	temp = (num / 100);
	temp %= 10;
	s[1] = intTochar(temp);
	temp = (num / 10);
	temp %= 10;
	s[2] = intTochar(temp);
	temp = (num);
	temp %= 10;
	s[3] = intTochar(temp);
	temp = (num * 10);
	temp %= 10;
	s[5] = intTochar(temp);
	temp = (num * 100);
	temp %= 10;
	s[6] = intTochar(temp);
  s[4]='.';
}

void TNCTransceiver::modulate(char *s){
  // sei();
  // while(counter!=0); // wait for zero crossing
  // for(int i =0; i<len(s); i++){ //select character
  //   char x = 0x01;
  //   msg = false;
  //   ct = false;
  //   while(ct==false);
  //   for(int j=0;j<8;j++){
  //     msg = (s[i] & x)? true: false;
  //     ct = false;
  //     while(ct==false);
  //     x = x<<1;
  //   }
  //   msg = false;
  //   ct = false;
  //   while(ct==false);    
  // }
  // OCR1B = 0;

  int listSize = (len(s)*10);
  bool *list = new bool[listSize];
  int inc=0;

  for(int i =0; i<len(s); i++){ //select character
    char x = 0x01;
    list[inc] = false;
    inc++;

    for(int j=0;j<8;j++){
      list[inc] = (s[i] & x)? true: false;
      inc++;
      x = x<<1;
    }
    list[inc] = true;
    inc++;
  }
  sei();
  while(counter!=0);
  for(int i=0; i<listSize;i++){
    msg = list[i];
    ct = false;
    while(ct==false);
  }

  delete [] list;
  OCR1B = 0;
}

void TNCTransceiver::Transmit_start(){
  TIMSK1 = 1;
  delay(500);
}

void TNCTransceiver::Transmit_stop(){
  OCR1B = 0;
  OCR1A = 0;
  TIMSK1 = 0;
}

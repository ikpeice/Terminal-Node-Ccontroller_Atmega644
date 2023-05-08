#ifndef TNCTRANSCEIVER_H
#define TNCTRANSCEIVER_H

#include <Arduino.h>




class TNCTransceiver{
    
private:
//Receiver 
    const int AF_pin;
    const int control_pin;
    long period;
    bool flag=true;
    bool buffer[1000];
    char bit = 1;
    bool debug = false;
// Transmitter


const int frequency_A = 1;
const int frequency_B = 2;
#define teta(V) (V*(9.0/2.0))
#define TetaToradian(T) ((T*PI)/180)
#define solve(teta,f) (abs(sin(teta*f)))



private:
    void bits_to_char(bool buffer[], int buff_size,char *msg);
    char decode();
    void setup_freq();
    
    int len(char s[]);
    char intTochar(int num);
    char *myString="";


public:

    TNCTransceiver(int AF_pin,int control_pin);
    ~TNCTransceiver();
    void floatTocharArray(char *s, double num);
    void begin(bool verbros);
    bool receive(char *msg);
    void modulate(char *s);
    void Transmit_start();
    void Transmit_stop();
    

};



#endif
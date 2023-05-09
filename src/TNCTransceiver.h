#ifndef TNCTRANSCEIVER_H
#define TNCTRANSCEIVER_H

#include <Arduino.h>




class TNCTransceiver{
    
private:
// Packet 
    char flag_byte =  0x7e;
    char destination_add[7];
    char source_add[7];
    char digipeater_add[8][56];
    char control_field = 0x03;
    char protocol_id = 0xf0;
    char info[256];
    char FCS[2];


//Receiver 
    const int AF_pin;
    const int control_pin;
    long period;
    bool flag=true;
    bool buffer[1200];
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


public:

    TNCTransceiver(int AF_pin,int control_pin);
    ~TNCTransceiver();
    void floatTocharArray(char *s, double num);
    void begin(bool verbros);
    bool receive(char *msg);
    void modulate(char *s,int size);
    void Transmit_packet();
    void start_transmitter();
    void stop_transmitter();
    void set_source_add(const char *s);
    void set_destination_add(const char *s);
    void set_info(const char *s);
    void set_FCS(const char *s);
    void set_digipeater_add(const char *s,int x);
};



#endif
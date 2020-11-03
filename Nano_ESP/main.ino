#include <SoftwareSerial.h>


SoftwareSerial ESPport(1, 0);   // RX, TX

int sensePin0 = 0;
int sensePin1 = 1;
int sensePin2 = 2;
int sensePin3 = 3;

int LedPin2 = 2;
int LedPin3 = 3;
int LedPin4 = 4;
int LedPin5 = 5;

void setup(){ 
    
    Serial.begin(9600);
    pinMode(2, OUTPUT); 
    pinMode(3, OUTPUT); 
    pinMode(4, OUTPUT); 
    pinMode(5, OUTPUT); 
}
void loop(){
    
    int val1 = analogRead(sensePin0);
    int val2 = analogRead(sensePin1);
    int val3 = analogRead(sensePin2);
    int val4 = analogRead(sensePin3);

    if(val1 > 0) 
        digitalWrite(LedPin2, HIGH);
    else 
        digitalWrite(LedPin2, LOW);

    if(val2 > 0) 
        digitalWrite(LedPin3, HIGH);
    else 
        digitalWrite(LedPin3, LOW);

    if(val3 > 0) 
        digitalWrite(LedPin4, HIGH);
    else 
        digitalWrite(LedPin4, LOW);

    if(val4 > 0) 
        digitalWrite(LedPin5, HIGH);
    else 
        digitalWrite(LedPin5, LOW);

    delay(5000);
    
}  

  

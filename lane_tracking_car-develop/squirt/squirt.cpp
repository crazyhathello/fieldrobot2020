#include "Arduino.h"

#include "squirt.h"
int squirt_pin=3;

void init_squirt(){
   pinMode(squirt_pin,OUTPUT);
}

void squirting(int delay_ms){
   digitalWrite(squirt_pin,HIGH);
   for(int i=0;i<delay_ms;i++){
       delay(1);
   }
   digitalWrite(squirt_pin,LOW);
}

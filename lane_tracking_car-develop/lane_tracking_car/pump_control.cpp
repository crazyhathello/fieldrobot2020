#include "pump_control.h"

void open_pump(){
  digitalWrite(PUMP_IN1, HIGH);
  digitalWrite(PUMP_IN2, LOW);
 }

 void close_pump(){
  digitalWrite(PUMP_IN1, LOW);
  digitalWrite(PUMP_IN2, LOW);
 }

void pump_init(){
  pinMode(PUMP_IN1,OUTPUT);
  pinMode(PUMP_IN2,OUTPUT);
  close_pump();
}

void pump_control(String cmd){
  
   if (cmd.indexOf("Open1")  == 0) {
     Serial.println("OpenPump1");
     //   open_pump();
        delay(PUMP_OPEN_DELAY_1); 
         Serial.println("leave delay1");
    }
    else if (cmd.indexOf("Open2")  == 0) {
     Serial.println("OpenPump2");
       // open_pump();
        delay(PUMP_OPEN_DELAY_2); 
         Serial.println("leave delay2");
    }
    else if (cmd.indexOf("Close") == 0) {
       Serial.println("ClosePump");
        //close_pump();
    }
    
}


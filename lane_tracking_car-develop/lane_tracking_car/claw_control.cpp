#include "claw_control.h"

void close_claw(){
    digitalWrite(CLAW_IN1, LOW);
    digitalWrite(CLAW_IN2, HIGH);
}
void open_claw(){
    digitalWrite(CLAW_IN1, HIGH);
    digitalWrite(CLAW_IN2, LOW);
}
void stop_claw(){
    digitalWrite(CLAW_IN1, LOW);
    digitalWrite(CLAW_IN2, LOW);
}

void claw_init(){
  pinMode(CLAW_IN1,OUTPUT);
  pinMode(CLAW_IN2,OUTPUT);
  stop_claw();
}


void claw_control(String cmd){
  
    if (cmd.indexOf("Open")  >= 0) {
      Serial.println("openclaw");
        open_claw();
        delay(CLAW_OPEN_DELAY); 
        stop_claw();
         Serial.println("stopclaw");
    }
    else if (cmd.indexOf("Close")  >= 0) {
        close_claw();
          Serial.println("closeClaw");
         delay(CLAW_CLOSE_DELAY); 
        stop_claw();
         Serial.println("StopClaw");
    }

}


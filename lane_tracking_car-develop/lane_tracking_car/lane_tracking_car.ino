#include <Wire.h>
#include "src/arm_utils.h"
#include "claw_control.h"
#include "pump_control.h"
#include "move_arm.h"
#include "move_car.h"

int y=220;
float down;
int height = 100;


void setup() {

  arm_init();
  pump_init();
  claw_init();  
  car_init();
  
  Wire.begin(0x2A);
  Wire.onReceive(receiveEvent); // register event
  //  Wire.onRequest(requestEvent); // register event
  Serial.begin(115200);
  Serial.println("ready");

 

  /*
 moveTo(0,100,100);
open_claw();
 delay(7500);
 stop_claw();
 close_claw();
 delay(7500);
 stop_claw();
*/


}




void loop() {


  String received;
  while (0 < Wire.available()) //pra rolar com i2cset no shell
  {
    char c = Wire.read(); // receive byte as a character
    received.concat(String(c));
  }
 // Serial.println(received);
  
  if (received.indexOf("arm") >= 0) {
    move_arm(received.substring(3));
  }
  else if (received.indexOf("claw") >= 0) {
    claw_control(received.substring(4));
  }
  else if (received.indexOf("pump") >= 0) {
    pump_control(received.substring(4));
  }
  else if (received.indexOf("move") >= 0) {
    Serial.print("motor value: ");
    Serial.println(received.substring(4).toInt());
    move_car(received.substring(4).toInt());
  }
  else if (received.indexOf("stop") >= 0)
  {
    stop();
  }
  else if(Wire.available()>0){
    moveTo(-150,0,140);
      Serial.println("in");
  }
  
}
void receiveEvent(int howMany){


}

void requestEvent()
{
  char req[] = "";

  Wire.write(req);
  //delay(50);
}





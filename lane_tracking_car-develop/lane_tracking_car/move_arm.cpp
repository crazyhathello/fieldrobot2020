#include "move_arm.h"

int current_position[3] = { 0, 100, 100 };
int stride = 1;
int prepared_pos1[3] = { 0, 100, 100 };
int prepared_pos2[3] = { 0, 200, 167 };
int prepared_pos3[3] = {0 , 0, 0 };
int home_pos[3] = {-150,0,140};
void move_arm(String cmd) {
  // Serial.println(cmd);
  if (cmd.indexOf("X") == 0) {
    int xpos = cmd.substring(1).toInt();
    current_position[0] = xpos;
    moveTo(current_position[0], current_position[1], current_position[2]);
  }
  else if (cmd.indexOf("Y") == 0) {
    int ypos = cmd.substring(1).toInt();
    current_position[1] = ypos;
    moveTo(current_position[0], current_position[1], current_position[2]);
  }
  else if (cmd.indexOf("pumpDescend") == 0) {
    current_position[2]-=80;
  }
  else if (cmd.indexOf("R") == 0) {
    current_position[0] += stride;
  }
  else if (cmd.indexOf("L") == 0) {
    current_position[0] -= stride;
  }
  else if (cmd.indexOf("F") == 0) {
    current_position[1] += stride;
  }
  else if (cmd.indexOf("B") == 0) {
    current_position[1] -= stride;
  }
  else if (cmd.indexOf("Descend") == 0) {
    int depth = cmd.substring(7).toInt();
    //current_position[2] -=  depth;
    current_position[2] =  -0.443 * current_position[1] + 120 ;  //calibrate term
  }
  else if (cmd.indexOf("Ascend") == 0) {
    int depth = cmd.substring(6).toInt();
    current_position[2] = 100;
  }
  else if (cmd.indexOf("PreparedPos") == 0) {
    int mode = cmd.substring(11).toInt();
    Serial.print("mode = ");
    Serial.println(mode);
    switch(mode){
      case 1:
        current_position[0] = prepared_pos1[0];
        current_position[1] = prepared_pos1[1];
        current_position[2] = prepared_pos1[2];
        Serial.println("mode1");
        break;
      case 2:
        current_position[0] = prepared_pos2[0];
        current_position[1] = prepared_pos2[1];
        current_position[2] = prepared_pos2[2];
         Serial.println("mode2");
        break;
      case 3:
        current_position[0] = prepared_pos3[0];
        current_position[1] = prepared_pos3[1];
        current_position[2] = prepared_pos3[2];
         Serial.println("mode3");
        break;
      default:
       Serial.println("default");
        break;
    }
  }
  else if (cmd.indexOf("Home") == 0) {   // separate the returning home path into to step pos 
        current_position[0] = home_pos[0];
        current_position[1] = home_pos[1];
        current_position[2] = home_pos[2];
  }
  else {
    Serial.println("Default");
  }


  /////////////////////////check range ////////////////////////////////
  if (current_position[0] > XMAX) current_position[0] = XMAX;
  else if (current_position[0] < XMIN)  current_position[0] = XMIN;
  if (current_position[1] > YMAX) current_position[1] = YMAX;
  else if (current_position[1] < YMIN)  current_position[1] = YMIN;
  if (current_position[2] > ZMAX) current_position[2] = ZMAX;
  else if (current_position[2] < ZMIN)  current_position[2] = ZMIN;
  /////////////////////////check range ////////////////////////////////

  moveTo(current_position[0], current_position[1],  current_position[2]);
  Serial.print("X: ");
  Serial.print(current_position[0]);
  Serial.print("Y: ");
  Serial.print(current_position[1]);
  Serial.print("Z: ");
  Serial.println(current_position[2]);

}

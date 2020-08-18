/*
  現在轉彎方式:50~150
  100~110之間以PID微調，>100右轉，<100左轉
  110~130間右轉只轉左輪，由慢到快/130~150間原地旋轉，由慢到快
  因此110、130時速度非連續
*/

#include <Wire.h>   //Wire.h? It may about serial?
//------------------------------------------------------PID PART
#include "PID_v1.h"

double Input, Output;
<<<<<<< HEAD
int BaseSpeed = 255;         // intial speed
=======
int BaseSpeed = 170;         // intial speed
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
double goal_direction = 0;     //initail degree
double Kp = 5, Ki = 0, Kd = 0;
//參數說明Kp:初始值和每一級增加的倍數 / Ki:會一直累加在Kp上  / Kd:第一個PID後的值會暴增，後回復Kp和Ki的規律
PID PID(&Input, &Output, &goal_direction, Kp, Ki, Kd, DIRECT, BaseSpeed);

int temp_fromTX2 = 100; //假設從0~255,以100作為中間值,+-100

//--------------------------------------motor pins
int pin_motor_L_forward = 10;
int pin_motor_L_backward = 11;
int pin_motor_R_forward = 5;
int pin_motor_R_backward = 6;

void setup() {

  PID.SetMode(AUTOMATIC);
  Serial.begin(250000);//arduino available : 115200,250000,500000,1000000,2000000
  Wire.begin(0x2A);
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event

  pinMode(pin_motor_L_forward, OUTPUT);
  pinMode(pin_motor_L_backward, OUTPUT);
  pinMode(pin_motor_R_forward, OUTPUT);
  pinMode(pin_motor_R_backward, OUTPUT);
  Serial.println("ready");

<<<<<<< HEAD
  stop();
=======
  Stop();
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
}


void loop() {
  /**--------------------------通訊封包格式
    static int speed[6];
    static char buff[30];
    int counter = 0;

    // read command from TX2,TX2端print出字元
    while (Serial.available()) {
      buff[counter] = Serial.read();
      if (counter > 30 || buff[counter] == '*') {
        buff[counter] = '\0';
        speed[0] = atoi(strtok(buff, ","));
        speed[1] = atoi(strtok(NULL, ","));
        speed[2] = atoi(strtok(NULL, ","));
        speed[3] = atoi(strtok(NULL, ","));
        speed[4] = atoi(strtok(NULL, ","));
        speed[5] = atoi(strtok(NULL, ","));
      }
      else {
        counter++;
      }
    }
  */
/*
  static char buff[10];
  int counter = 0;
  while (Serial.available()) {
    buff[counter] = Serial.read();
    if (counter > 10 || buff[counter] == '*') {
      buff[counter] = '\0';
      temp_fromTX2 = atoi(buff);
    }
    else {
      counter++;
    }
  }
*/

//  int speed_fromTX2 = temp_fromTX2; // - 100;  //0當中間值
  //Serial.println(speed_fromTX2);
  delay(100);
  
}

void move_car(int speed_fromTX2){
<<<<<<< HEAD
    go_forward_pid(speed_fromTX2);                          //在此小範圍用PID來前進
=======
  if (speed_fromTX2 >= -10 && speed_fromTX2 <= 10)
    go_forward_pid(speed_fromTX2);                          //在此小範圍用PID來前進
  else if (speed_fromTX2 >= -30 && speed_fromTX2 < -10)
    TurnLeft(1, speed_fromTX2);                             //左轉:只轉右輪,左輪固定
  else if (speed_fromTX2 >= -50 && speed_fromTX2 < -30)
    TurnLeft(0, speed_fromTX2);                             //左轉:原地旋轉式
  else if (speed_fromTX2 > 10 && speed_fromTX2 <= 30)
    TurnRight(1, speed_fromTX2);                             //右轉:只轉左輪,右輪固定
  else if (speed_fromTX2 > 30 && speed_fromTX2 <= 50)
    TurnRight(0, speed_fromTX2);                             //右轉:原地旋轉式
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
}

void motor_L_motion(int value) {
  if (value > 0) {
    analogWrite(pin_motor_L_forward, value);
    analogWrite(pin_motor_L_backward, 0);
  }
  else if (value < 0) {
    analogWrite(pin_motor_L_forward, 0);
    analogWrite(pin_motor_L_backward, -value);
  }
  else {
    analogWrite(pin_motor_L_forward, 0);
    analogWrite(pin_motor_L_backward, 0);
  }
  Serial.print(value);
  Serial.print("/");

}
void motor_R_motion(int value) {
  if (value > 0) {
    analogWrite(pin_motor_R_forward, value);
    analogWrite(pin_motor_R_backward, 0);
  }
  else if (value < 0) {
    analogWrite(pin_motor_R_forward, 0);
    analogWrite(pin_motor_R_backward, -value);
  }
  else {
    analogWrite(pin_motor_R_forward, 0);
    analogWrite(pin_motor_R_backward, 0);
  }
  Serial.println(value);
}

<<<<<<< HEAD
void stop() {
=======
void Stop() {
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
  motor_R_motion(0);
  motor_L_motion(0);
  Serial.println("stop");
}

void go_forward_pid(int value) {
<<<<<<< HEAD
  BaseSpeed = 255;
=======
  BaseSpeed = 200;
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
  goal_direction = 0;
  static int motor_left;
  static int motor_right;
  Input = value;
  PID.Compute();
  motor_left = BaseSpeed - (int)Output;
  motor_right = BaseSpeed + (int)Output;
<<<<<<< HEAD

=======
  Serial.println(Output);
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
  motor_L_motion(motor_left);
  motor_R_motion(motor_right);
}

void go_backward_pid(int value) {
  BaseSpeed = 200;
  goal_direction = 0;
  static int motor_left;
  static int motor_right;
  Input = value;
  PID.Compute();
  motor_left = BaseSpeed - (int)Output;
  motor_right = BaseSpeed + (int)Output;

  motor_L_motion(-motor_left);
  motor_R_motion(-motor_right);

}

void TurnLeft(int method, int value) {
  BaseSpeed = 155;
  
  static int motor_left;
  static int motor_right;
  Input = value;
  

  if (method == 1) {
    motor_left = 0;
    goal_direction = -10;
    PID.Compute();
  }
  else if (method == 0) {
    goal_direction = -30;
    PID.Compute();
    motor_left = -(BaseSpeed + (int)Output);
  }

  motor_right = BaseSpeed + (int)Output;

  motor_L_motion(motor_left);
  motor_R_motion(motor_right);
}

void TurnRight(int method, int value) {
  BaseSpeed = 155;

  static int motor_left;
  static int motor_right;
  Input = value;


  if (method == 1) {
    motor_right = 0;
    goal_direction = 10;
    PID.Compute();
  }
  else if (method == 0) {
    goal_direction = 30;
    PID.Compute();
    motor_right = -(BaseSpeed - (int)Output);
  }

  motor_left = BaseSpeed - (int)Output;

  motor_L_motion(motor_left);
  motor_R_motion(motor_right);

}

/** I2C controls here **/
void receiveEvent(int howMany)
{
  Serial.println("receiving write data... ");
  String txt;
  
  while(0 < Wire.available()) //pra rolar com i2cset no shell
  {
    char c = Wire.read(); // receive byte as a character
    txt.concat(String(c));
  }    
  Serial.print("received: ");         // print the character
  Serial.println(txt);         // print the character
  if(txt.indexOf("move") >= 0)
  {
      Serial.print("motor value: ");
      Serial.println(txt.substring(4).toInt());
      move_car(txt.substring(4).toInt());
  }else if(txt.indexOf("arm") >=0)
  {
    //move_arm(txt.substring(3).toInt());
  }else if(txt.indexOf("stop") >= 0)
  {
<<<<<<< HEAD
    stop();
  }else
  {
    stop();
=======
    Stop();
  }else
  {
    Stop();
>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0
    Serial.println("stop");
  }
}

void requestEvent()
{
  char req[] = "ab";
  
  Wire.write(req);
  //Wire.write('b');
  //Serial.print("i get request!");

 
   delay(50);
}


<<<<<<< HEAD

=======

>>>>>>> 94e202e096e9f4dd3f8e3e01d553f7f8a6b1cec0

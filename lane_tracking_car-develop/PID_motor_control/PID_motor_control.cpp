/*
  現在轉彎方式:50~150
  100~110之間以PID微調，>100右轉，<100左轉
  110~130間右轉只轉左輪，由慢到快/130~150間原地旋轉，由慢到快
  因此110、130時速度非連續
*/

//#include <Wire.h>   //Wire.h? It may about serial?
//------------------------------------------------------PID PART
#include "PID_v1.h"
#include "PID_motor_control.h"
#include <Arduino.h>

double Input, Output;
int Base_Speed=200;         // intial speed, the PID range won't larger than the number.
double goal_direction = 0;     //initail degree
double Kp = 5, Ki = 0, Kd = 0;
//
//參數說明Kp:初始值和每一級增加的倍數 / Ki:會一直累加在Kp上  / Kd:第一個PID後的值會暴增，後回復Kp和Ki的規律
PID PID(&Input, &Output, &goal_direction, Kp, Ki, Kd, DIRECT, Base_Speed);

//--------------------------------------motor pins
int pin_motor_L_forward = 10;
int pin_motor_L_backward = 11;
int pin_motor_R_forward = 5;
int pin_motor_R_backward = 6;

void PID_motor_control_init() {
  PID.SetMode(AUTOMATIC);
  Serial.begin(250000);//arduino available : 115200,250000,500000,1000000,2000000

  pinMode(pin_motor_L_forward, OUTPUT);
  pinMode(pin_motor_L_backward, OUTPUT);
  pinMode(pin_motor_R_forward, OUTPUT);
  pinMode(pin_motor_R_backward, OUTPUT);
}


void car_control(int temp_fromTX2, int tmp_basespeed) {
  Base_Speed = tmp_basespeed;
  int speed_fromTX2 = temp_fromTX2 - 100; // ;  //0當中間值
  if (temp_fromTX2 == 0) {
    stop_driving();
  }
  else {
    //Serial.println(speed_fromTX2);

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
  }
  //Serial.println(temp_fromTX2);
  //Serial.println(Base_Speed);
  delay(10);
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
void stop_driving() {
  motor_R_motion(0);
  motor_L_motion(0);
}
void go_forward_pid(int value) {
  //BaseSpeed = 200;
  goal_direction = 0;
  static int motor_left;
  static int motor_right;
  Input = value;
  PID.Compute();
  motor_left = Base_Speed - (int)Output;
  motor_right = Base_Speed + (int)Output;

  motor_L_motion(motor_left);
  motor_R_motion(motor_right);
}

void go_backward_pid(int value) {
  //BaseSpeed = 200;
  goal_direction = 0;
  static int motor_left;
  static int motor_right;
  Input = value;
  PID.Compute();
  motor_left = Base_Speed - (int)Output;
  motor_right = Base_Speed + (int)Output;

  motor_L_motion(-motor_left);
  motor_R_motion(-motor_right);

}

void TurnLeft(int method, int value) {
  //BaseSpeed = 155;

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
    motor_left = -(Base_Speed + (int)Output);
  }

  motor_right = Base_Speed + (int)Output;

  motor_L_motion(motor_left);
  motor_R_motion(motor_right);
}

void TurnRight(int method, int value) {
  //BaseSpeed = 155;

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
    motor_right = -(Base_Speed - (int)Output);
  }

  motor_left = Base_Speed - (int)Output;

  motor_L_motion(motor_left);
  motor_R_motion(motor_right);

}




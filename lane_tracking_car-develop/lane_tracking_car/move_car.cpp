/*
  現在轉彎方式:50~150
  100~110之間以PID微調，>100右轉，<100左轉
  110~130間右轉只轉左輪，由慢到快/130~150間原地旋轉，由慢到快
  因此110、130時速度非連續
*/

//------------------------------------------------------PID PART
#include "PID_v1.h"
#include "Arduino.h"
#include "move_car.h"

double Input, Output;
int BaseSpeed = 240;         // intial speed
double goal_direction = 0;     //initail degree
double Kp = 4.5, Ki = 0.15, Kd = 0.12;
//參數說明Kp:初始值和每一級增加的倍數 / Ki:會一直累加在Kp上  / Kd:第一個PID後的值會暴增，後回復Kp和Ki的規律
PID PID(&Input, &Output, &goal_direction, Kp, Ki, Kd, DIRECT, BaseSpeed);

//--------------------------------------motor pins
int pin_motor_L_forward = 10;
int pin_motor_L_backward = 11;
int pin_motor_R_forward = 5;
int pin_motor_R_backward = 6;

void car_init() {

  PID.SetMode(AUTOMATIC);

  pinMode(pin_motor_L_forward, OUTPUT);
  pinMode(pin_motor_L_backward, OUTPUT);
  pinMode(pin_motor_R_forward, OUTPUT);
  pinMode(pin_motor_R_backward, OUTPUT);
  Serial.println("car ready");
  stop();
}

void move_car(int speed_fromTX2) {
  BaseSpeed = 240;
  goal_direction = 0;
  static int motor_left;
  static int motor_right;
  Input = speed_fromTX2;
  PID.Compute();

  if (Output >= 0) {
    motor_left = BaseSpeed - (int)Output;    //output>0:heading right, <0:heading left
    motor_right = BaseSpeed;
  }
  else if (Output < 0) {
    motor_left = BaseSpeed;   //output>0:heading right, <0:heading left
    motor_right = BaseSpeed + (int)Output;

  }

  motor_L_motion(motor_left);
  motor_R_motion(motor_right);
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

void stop() {
  motor_R_motion(0);
  motor_L_motion(0);
  Serial.println("stop");
}

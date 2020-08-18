#ifndef PID_motor_control_h
#define PID_motor_control_h
//int *Base_Speed;
void PID_motor_control_init();
void car_control(int,int);

void motor_L_motion(int);
void motor_R_motion(int);
void stop_driving();
void go_forward_pid(int);
void go_backward_pid(int);
void TurnLeft(int, int);
void TurnRight(int, int);

#endif.

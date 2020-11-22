int IN1R = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM); Right wheel
int IN2R = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM); Right wheel

int IN1L = 8; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM); Left wheel
int IN2L = 9; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM); Left wheel

int dir = 0, pwmR = 0, pwmL = 0;
char cmd[9], pwm1[4], pwm2[4];

void setup()
{
  pinMode(IN1R, OUTPUT);
  pinMode(IN2R, OUTPUT);
  pinMode(IN1L, OUTPUT);
  pinMode(IN2L, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
//  forward(255, 100);
//  delay(2000);
//  left(255, 255);
//  delay(1450);
//  right(255, 255);
//  delay(1450);
  if (Serial.available())
  {
    Serial.readBytes(cmd, 9);
    Serial.println("recieved");
    dir = cmd[0];
    for (int i = 0; i < 3; i++)
    {
      pwm1[i] = cmd[i + 2];
      pwm2[i] = cmd[i + 6];
    }
    dir = cmd[0]-'0';
    pwm1[3] = '\0';
    pwm2[3] = '\0';
    pwmL = atoi(pwm1);    //pwm1 is left wheel pwm
    pwmR = atoi(pwm2);    //pwm2 is right wheel pwm
    //Serial.print("cmd: ");  //, "; dir: ", dir, "; pwm1: ", pwm1, "; pwm2: ", pwm2
    //Serial.println(cmd);
  }

  switch (dir)    //dir: 0(forward), 1(right), 2(left), 3(backward), 4(stop)
  {
  case 0:
    forward(pwmL, pwmR);
    break;
  case 1:
    right(pwmL, pwmR);
    break;
  case 2:
    left(pwmL, pwmR);
    break;
  case 3:
    backward(pwmL, pwmR);
    break;
  case 4:
    stopp();
    break;
  }

}


void forward(int PWML, int PWMR) //dir = 0
{
  analogWrite(IN1L, 0);
  analogWrite(IN2L, PWML);
  analogWrite(IN1R, 0);
  analogWrite(IN2R, PWMR);

//  forward(255, 245);    //almost, but bias happen  by height diff
//  delay(2500);
}

void right(int PWML, int PWMR) //dir = 1; right wheel backward, left forward;
{
  analogWrite(IN1L, 0);
  analogWrite(IN2L, PWML);
  analogWrite(IN1R, PWMR);
  analogWrite(IN2R, 0);
  
//  right(210, 210);    //90 degree
//  delay(1700);
//  right(255, 245);    // full, 90 degree
//  delay(1450);
}

void left(int PWML, int PWMR) //dir = 2; left wheel backward, right forward;
{
  analogWrite(IN1L, PWML);
  analogWrite(IN2L, 0);
  analogWrite(IN1R, 0);
  analogWrite(IN2R, PWMR);
  
//  left(255, 255);
//  delay(1450);
}

void backward(int PWML, int PWMR) //dir = 3
{
  analogWrite(IN1L, PWML);
  analogWrite(IN2L, 0);
  analogWrite(IN1R, PWMR);
  analogWrite(IN2R, 0);

  
//  backward(255, 250);
//  delay(2500);
}

void stopp()
{
  analogWrite(IN1L, 0);
  analogWrite(IN2L, 0);
  analogWrite(IN1R, 0);
  analogWrite(IN2R, 0);
}
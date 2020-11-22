int IN1R = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM); Right wheel
int IN2R = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM); Right wheel

int IN1L = 8; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM); Left wheel
int IN2L = 9; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM); Left wheel

int dir = 0, pwmR = 0, pwmL = 0;
char cmd[7], pwm1[3], pwm2[3];

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
  if (Serial.available())
  {
    Serial.readBytes(cmd, 10);
    dir = atoi(cmd[0]);
    for (int i = 0; i < 3; i++)
    {
      pwm1[i] = cmd[i + 1];
      pwm2[i] = cmd[i + 4];
    }
    pwmR = atoi(pwm1);
    pwmL = atoi(pwm2);
    Serial.print("cmd: ");  //, "; dir: ", dir, "; pwm1: ", pwm1, "; pwm2: ", pwm2
    Serial.println(cmd);
  }

  switch (dir)    //dir: 0(forward), 1(right), 2(left), 3(backward)
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

void serialread()   //其他serial用法 沒用到 到時候再刪
{
  String s = "";
  while (Serial.available())
  {
    char c = Serial.read();
    if (c != '\n')
    {
      s += c;
    }
    delay(5); // 沒有延遲的話 UART 串口速度會跟不上Arduino的速度，會導致資料不完整
  }

  if (s != "")
  {
    Serial.println(s);
  }
  return s;
}
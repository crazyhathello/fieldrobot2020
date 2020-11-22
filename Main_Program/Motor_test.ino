// int ENA = 2;
// int ENB = 4;
// int IN1A = 3;
// int IN1B = 5;
// int IN2A = 6;
// int IN2B = 9;

// void setup()
// {
//     pinMode(ENA, OUTPUT);
//     pinMode(ENB, OUTPUT);
//     pinMode(IN1A, OUTPUT);
//     pinMode(IN1B, OUTPUT);
//     pinMode(IN2A, OUTPUT);
//     pinMode(IN2B, OUTPUT);
// }
// void loop(){
//     forward(220,220);
//     delay(5000);
//     stop();
//     delay(3000);
// }

// void forward(int pwma, int pwmb){
//     digitalWrite(ENA,HIGH);
//     digitalWrite(ENB,HIGH);
//     analogWrite(IN1A,pwma);
//     analogWrite(IN2A, 0);
//     analogWrite(IN1B,pwmb);
//     analogWrite(IN2B, 0);
// }

// void backward(int pwma, int pwmb){
//     digitalWrite(ENA,HIGH);
//     digitalWrite(ENB,HIGH);
//     analogWrite(IN1A,0);
//     analogWrite(IN2A,pwma);
//     analogWrite(IN1B,0);
//     analogWrite(IN2B,pwmb);
// }

// void stop(){
//     digitalWrite(ENA,LOW);
//     digitalWrite(ENB,LOW);
// }
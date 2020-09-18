int ena = 11;
int left1 = 10;
int left2 = 9;
int right1 = 8;
int right2 = 7;
int enb = 6;

void setup()
{
    pinMode(left1, OUTPUT);
    pinMode(left2, OUTPUT);
    pinMode(right1, OUTPUT);
    pinMode(ena, OUTPUT);
    pinMode(enb, OUTPUT);
    Serial.begin(9600);
}
void loop(){
    delay(500);
}
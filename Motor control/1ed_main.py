import serial
STATE = 0
pwmL = 0
pwmR = 0

def startSerialCom():
    ser = serial.Serial(
        port='/dev/ttyACM0',
        baudrate=9600,
    )
    if (ser):
        print("Serial communication success!!")
        return ser

def runMotor(ser,dir,pwm1,pwm2):  ## dir: 0(forward), 1(right), 2(left), 3(backward)
    print("Through Serial Communication :")
    input=str(dir)+","+str(pwm1)+","+str(pwm2)
    print(input)
    ser.write(str.encode(input))

pwm2=50
ser = startSerialCom()
#runMotor(ser,0,pwm1,pwm2)

while()
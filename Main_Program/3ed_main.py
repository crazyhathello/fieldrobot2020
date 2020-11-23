import serial
import cv2
import numpy as np
import utilities
import time
STATE = 0
pwmL_base = 155
pwmR_base = 145
pwmL = 0
pwmR = 0
x_delta = 0
X_MID = 630
pwm_delta = 0
HSV = True
GUI = True

def startSerialCom():
    ser = serial.Serial(
        port='/dev/ttyACM0',
        baudrate=9600,
    )
    print('Opening serial port...')
    time.sleep(2)
    if (ser):
        print("Serial communication success!!")
        return ser

def runMotor(ser,dir,pwm1,pwm2):  ## dir: 0(forward), 1(right), 2(left), 3(backward)
    print("Through Serial Communication :")
    pwm1 = str(pwm1)
    pwm2 = str(pwm2)
    if len(pwm1) == 2:
        pwm1 = "0" + pwm1
    if len(pwm2) == 2:
        pwm2 = "0" + pwm2
    if len(pwm1) == 1:
        pwm1 = "00" + pwm1
    if len(pwm2) == 1:
        pwm2 = "00" + pwm2

    input=str(dir)+","+pwm1+","+pwm2
    print(input)
    ser.write(str.encode(input))

def checkHSV(capture):
    if HSV:
        ret,frame = capture.read()
        utilities.find_range(frame)
    return True

ser = startSerialCom()

video = cv2.VideoCapture(0)
if video.isOpened() == False:
    print("Error opening video stream or file")
if checkHSV(video):
    print("HSV ready")

# runMotor(ser,0,200,200)
# time.sleep(2)
# runMotor(ser,4,000,000)
# time.sleep(2)
# runMotor(ser,3,200,200)
# time.sleep(2)
# runMotor(ser,4,000,000)
# time.sleep(2)
try:
    while STATE==0:
        
        pwmL = pwmL_base
        pwmR = pwmR_base
        ret,frame = video.read()
        if ret == True:
            print("into loop")
            close,frame = utilities.filter_green(frame)
            original = frame.copy()

            green_area = cv2.countNonZero(close)
            image_area = frame.shape[0]*frame.shape[1]
            green_percent = (green_area/image_area)*100
            if green_percent < 18:
                runMotor(ser,4,0,0)
                STATE = 1
                # runMotor(ser,4,0,0)
                # time.sleep(2)
                # runMotor(ser,1,pwmL,pwmR)
                # time.sleep(2)
                # runMotor(4,0,0)
                # time.sleep(2)
                continue
            cv2.putText(frame,"Green percent " + str(green_percent),(50,300),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2,cv2.LINE_AA)
            res = cv2.bitwise_and(original, original, mask=close)

            frame, x_delta = utilities.find_lane(close,frame,X_MID)
            print(x_delta)
            pwm_delta = x_delta/2 #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            pwm_delta = int(pwm_delta)
            # if x_delta <0:
            #     pwmL -= int(1.5*pwm_delta) #%%%%%%%%%%%%%%%%%%%%%%%%
            #     pwmR += int(1.5*pwm_delta) #%%%%%%%%%%%%%%%%%%%%%%%%
            # else:
            #     pwmL -= pwm_delta
            #     pwmR += pwm_delta
            if x_delta < 0:
                pwmL += pwm_delta
                pwmR -= pwm_delta
            else:
                pwmL += pwm_delta
                pwmR -= pwm_delta
            # pwm gate
            if pwmL > 250:
                pwmL = 250
            if pwmR > 250:
                pwmR = 250

            runMotor(ser,0,pwmL,pwmR)
            
            if GUI:    
                output = cv2.hconcat([frame, res])
                cv2.imshow("Output", output)
                cv2.imshow("mask", close)
                if cv2.waitKey(25) & 0xFF == ord("q"):
                    break
            time.sleep(0.01)
        else:
            break
except KeyboardInterrupt:
    pass



if GUI:
    video.release()
    cv2.destroyAllWindows()
runMotor(ser,4,0,0)
time.sleep(2)
ser.close()

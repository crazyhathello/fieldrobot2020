import serial
import cv2
import numpy as np
import utilities
import time
pwmL_base = 205
pwmR_base = 195
pwmL = 0
pwmR = 0
x_delta = 0
X_MID = 670
pwm_delta = 0
HSV = False
GUI = True
DEBUG = False
STATE_MAP = [3,0,1,0,1,4,2,0,2,0]

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

# STATES: 0(line follow), 1(right turn), 2(left turn),3(rock),4(watering),5(special)
try:
    for STATE in STATE_MAP:
        if DEBUG:
            STATE = 5
        if STATE == 0:
            while STATE==0:
                pwmL = pwmL_base
                pwmR = pwmR_base
                ret,frame = video.read()
                if ret == True:
                    print("Line tracking")
                    close,frame = utilities.filter_green(frame)
                    original = frame.copy()

                    green_area = cv2.countNonZero(close)
                    image_area = frame.shape[0]*frame.shape[1]
                    green_percent = (green_area/image_area)*100
                    if green_percent < 38:
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
                    print("x_delta: ",x_delta)
                    pwm_delta = x_delta/2.5 #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    print("pwm_delta: ",pwm_delta)
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
                        pwmL += int(1.2*pwm_delta)
                        pwmR -= pwm_delta
                    # pwm gate
                    if pwmL > 250:
                        pwmL = 250
                    if pwmR > 250:
                        pwmR = 250
                    if pwmL < 0:
                        pwmL = 0
                    if pwmR < 0:
                        pwmR = 0

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
            continue

        elif STATE == 1:
            #time.sleep(2)
            runMotor(ser,1,pwmL_base,pwmR_base)
            time.sleep(1.7)
            runMotor(ser,4,0,0)
            time.sleep(1)
            continue

        elif STATE == 2:
            #time.sleep(2)
            runMotor(ser,2,pwmL_base,pwmR_base)
            time.sleep(1.7)
            runMotor(ser,4,0,0)
            time.sleep(1)
            continue

        elif STATE == 3: # rock
            while True:
                pwmL = pwmL_base
                pwmR = pwmR_base
                ret,frame = video.read()
                if ret == True:
                    print("Rock_phase")
                    close,frame = utilities.filter_green(frame)
                    original = frame.copy()
                    green_area = cv2.countNonZero(close)
                    image_area = frame.shape[0]*frame.shape[1]
                    green_percent = (green_area/image_area)*100
                    if green_percent > 50:
                        break
                    runMotor(ser,0,pwmL,pwmR)
                #cv2.putText(frame,"Green percent " + str(green_percent),(50,300),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2,cv2.LINE_AA)
            continue

        elif STATE == 4:#watering
            continue  
        elif STATE ==5: 
            runMotor(ser,0,pwmL_base,pwmR_base)
            time.sleep(2)
except KeyboardInterrupt:
    pass

if GUI:
    video.release()
    cv2.destroyAllWindows()
runMotor(ser,4,0,0)
time.sleep(2)
ser.close()

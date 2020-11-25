import serial
import cv2
import numpy as np
import utilities
import time
pwmL_base = 210
pwmR_base = 155
pwmL = 0
pwmR = 0
x_delta = 0
X_MID = 690
pwm_delta = 0
HSV = False
GUI = True
DEBUG = False
STATE_MAP = [3,1,0,1,0,2,0,2,0]
#STATE_MAP = [4,2,0,2,0]
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
        ret,img = capture.read()
        for i in range(20):
            ret,img = capture.read()
        scale_percent = 50 # percent of original size
        width = int(img.shape[1] * scale_percent / 100)
        height = int(img.shape[0] * scale_percent / 100)
        dim = (width, height)
        # resize image
        resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
        utilities.find_range(resized)
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
if DEBUG:
    try:
        while True:
            runMotor(ser,0,255,195)
            time.sleep(0.1)
    except KeyboardInterrupt:
        pass
try:
    for STATE in STATE_MAP:
        if DEBUG:
            STATE = 5
        if STATE == 0:
            while STATE==0:
                pwmL = pwmL_base
                pwmR = pwmR_base
                ret,frame_original = video.read()
                if ret == True:
                    print("Line tracking")
                    close,frame = utilities.filter_color(frame_original,"green")
                    white_mask, white_frame = utilities.filter_color(frame_original,"white")
                    original1 = frame.copy()
                    original2 = white_frame.copy()

                    white_area = cv2.countNonZero(white_mask)
                    image_area = frame.shape[0]*frame.shape[1]
                    white_percent = (white_area/image_area)*100

                    green_area = cv2.countNonZero(close)
                    image_area = frame.shape[0]*frame.shape[1]
                    green_percent = (green_area/image_area)*100
                    if white_percent > 3.2:
                        break
                    cv2.putText(frame,"White percent " + str(white_percent),(50,400),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2,cv2.LINE_AA)
                    cv2.putText(frame,"Green percent " + str(green_percent),(50,300),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2,cv2.LINE_AA)
                    res = cv2.bitwise_and(original1, original1, mask=close)
                    white_res = cv2.bitwise_and(original2,original2,mask=white_mask)

                    mask = cv2.fillPoly(white_res, np.array([[(0,0),(0,647),(250,647),(250,0)]],dtype=np.int32), 0)
                    mask = cv2.fillPoly(mask, np.array([[(902,0),(902,647),(1152,647),(1152,0)]],dtype=np.int32), 0)

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
                    if x_delta > 0:
                        pwmL += int(1.5*pwm_delta)
                        pwmR -= int(1*pwm_delta)
                    else:
                        pwmL += int(1*pwm_delta)
                        pwmR -= pwm_delta
                    # pwm gate
                    if pwmL > 250:
                        pwmL = 250
                    if pwmR > 250:
                        pwmR = 250
                    if pwmL < 80:
                        pwmL = 80
                    if pwmR < 80:
                        pwmR = 80

                    runMotor(ser,0,pwmL,pwmR)
                    
                    if GUI:
                        output = cv2.hconcat([frame, res])
                        cv2.imshow("Output", output)
                        cv2.imshow("mask", close)
                        cv2.imshow("white",mask)
                        
                    if cv2.waitKey(25) & 0xFF == ord("q"):
                        break
                    time.sleep(0.01)
                else:
                    break
            continue

        elif STATE == 1:
            #time.sleep(2)
            runMotor(ser,0,pwmL_base,pwmR_base)
            time.sleep(1)
            runMotor(ser,1,pwmL_base,pwmR_base)
            time.sleep(2.5)
            runMotor(ser,0,pwmL_base,pwmR_base)
            time.sleep(1)
            #runMotor(ser,4,0,0)
            #time.sleep(1)
            continue

        elif STATE == 2:
            #time.sleep(2)
            runMotor(ser,2,pwmL_base,pwmR_base)
            time.sleep(2.3)
            #runMotor(ser,4,0,0)
            #time.sleep(1)
            continue

        elif STATE == 3: # rock
            #runMotor(ser,0,250,220)
            #time.sleep(20)
            while True:
                pwmL = 255
                pwmR = 195
                ret,frame = video.read()
                if ret == True:
                    
                    print("Rock_phase")
                    close,frame = utilities.filter_color(frame,"green")
                    original = frame.copy()
                    green_area = cv2.countNonZero(close)
                    image_area = frame.shape[0]*frame.shape[1]
                    green_percent = (green_area/image_area)*100
                    if green_percent > 25:
                        while True:
                            runMotor(ser,0,pwmL,pwmR)
                            ret,frame_original = video.read()
                            white_mask, white_frame = utilities.filter_color(frame_original,"white")
                            white_area = cv2.countNonZero(white_mask)
                            image_area = frame.shape[0]*frame.shape[1]
                            white_percent = (white_area/image_area)*100
                            if white_percent > 2.4:
                                print("+++++++++++++++++++++++")
                                break
                        break
                    runMotor(ser,0,pwmL,pwmR)
                #cv2.putText(frame,"Green percent " + str(green_percent),(50,300),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2,cv2.LINE_AA)
            continue

        elif STATE == 4:#watering
            runMotor(ser,0,255,195)
            time.sleep(4)
            runMotor(ser,0,100,75)
            time.sleep(2)
            runMotor(ser,0,255,195)
            time.sleep(10)
            runMotor(ser,0,100,75)
            time.sleep(2)
            runMotor(ser,0,255,195)
            time.sleep(3)
            runMotor(ser,0,100,75)
            time.sleep(2)
            runMotor(ser,0,255,195)
            time.sleep(3)
            continue  
        elif STATE == 5: 
            print("debug mode")
            runMotor(ser,0,250,220)
            time.sleep(0.1)
            continue
except KeyboardInterrupt:
    pass

if GUI:
    video.release()
    cv2.destroyAllWindows()
runMotor(ser,4,0,0)
time.sleep(2)
ser.close()

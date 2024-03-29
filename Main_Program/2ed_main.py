import serial
import cv2
import numpy as np
import range_finder
import time
STATE = 0
pwmL_base = 155
pwmR_base = 145
pwmL = 0
pwmR = 0
x_delta = 0
pwm_delta = 0
HSV = False
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
    # ser.flushInput()
    ser.write(str.encode(input))
    # time.sleep(0.05)
    # ser_bytes = ser.readline()
    # arduino = ser_bytes.decode()
    # print(str.encode(input))
    # print('arduino: ',arduino)

def checkHSV(capture):
    if HSV:
        ret,frame = capture.read()
        range_finder.find_range(frame)
    return True

ser = startSerialCom()
#video = cv2.VideoCapture("camera_test_code\\test_video.mp4")

video = cv2.VideoCapture(0)
if video.isOpened() == False:
    print("Error opening video stream or file")
if checkHSV(video):
    print("HSV ready")

runMotor(ser,0,200,200)
time.sleep(2)
runMotor(ser,4,000,000)
time.sleep(2)
runMotor(ser,3,200,200)
time.sleep(2)
runMotor(ser,4,000,000)
time.sleep(2)
while STATE==0:
    pwmL = pwmL_base
    pwmR = pwmR_base
    ret,frame = video.read()
    if ret == True:
        print("into loop")
        # Image scaling
        scale_percent = 60  # percent of original size
        width = int(frame.shape[1] * scale_percent / 100)
        height = int(frame.shape[0] * scale_percent / 100)
        dim = (width, height)
        frame = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

        original = frame.copy()

        # Convert Color range BGR --> RGB --> HSV
        path = cv2.cvtColor(original, cv2.COLOR_BGR2RGB)
        path = cv2.cvtColor(path, cv2.COLOR_RGB2HSV)

        # Color filtering
        # HSV ranges (0,76,0) to (20,255,255) (use range_finder.py)
        lower_green = np.array([18, 115, 0])
        upper_green = np.array([60, 255, 255])
        
        mask = cv2.inRange(path, lower_green, upper_green)
        
        # Morphological Closing
        close_kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
        erosion_kernel = np.ones((5,5),np.uint8)
        close = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, close_kernel, iterations=1)
        erosion = cv2.erode(close,erosion_kernel,iterations = 1)
        dilation = cv2.dilate(erosion,erosion_kernel,iterations = 1)
        
        close = dilation

        res = cv2.bitwise_and(original, original, mask=close)

        contours, hierarchy = cv2.findContours(close, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        if contours:    
            contours = sorted(contours,key=cv2.contourArea,reverse=True)[0]
            hull = cv2.convexHull(contours)
            epsilon = 0.05*cv2.arcLength(hull,True)
            approx = cv2.approxPolyDP(hull,epsilon,True)

            pt_list = approx.tolist()
            pt_list.append(pt_list[0])
            slope_list = []
            
            for i in range(len(pt_list)-1):
                point1 = pt_list[i][0]
                point2 = pt_list[i+1][0]
                if (point1[0]-point2[0]) is not 0:
                    slope = (point1[1]-point2[1])/(point1[0]-point2[0])
                else:
                    slope = 0
                slope_list.append(slope)
            print(slope_list)
            
            min = np.argmin(np.asarray(slope_list))
            max = np.argmax(np.asarray(slope_list))
            if min == 3:
                min = (3,0)
            else:
                min = (min,min+1)
            if max == 3:
                max = (3,0)
            else:
                max = (max,max+1)
            print(min,max)
            
            if len(approx) == 4:    
                cv2.line(frame,tuple(approx[min[0]][0]),tuple(approx[min[1]][0]),(0,0,255),2)
                cv2.line(frame,tuple(approx[max[0]][0]),tuple(approx[max[1]][0]),(0,0,255),2)
            
                lineA = [approx[min[0]][0],approx[min[1]][0]]
                lineB = [approx[max[0]][0],approx[max[1]][0]]
                if lineA[1][1]>lineA[0][1]:
                    temp = lineA[0]
                    lineA[0] = lineA[1]
                    lineA[1] = temp
                if lineB[1][1]>lineB[0][1]:
                    temp = lineB[0]
                    lineB[0] = lineB[1]
                    lineB[1] = temp
                MID_LINE = [((lineA[0][0]+lineB[0][0])/2,(lineA[0][1]+lineB[0][1])/2),((lineA[1][0]+lineB[1][0])/2,(lineA[1][1]+lineB[1][1])/2)]
                MID_LINE = tuple(tuple(map(int, tup)) for tup in MID_LINE) 
                cv2.line(frame,MID_LINE[0],MID_LINE[1],(0,0,255),2)
                x_delta = MID_LINE[0][0]-MID_LINE[1][0]
        print(x_delta)
        pwm_delta = x_delta/3.5
        pwm_delta = int(pwm_delta)
        if x_delta <0:
            pwmL -= int(1.5*pwm_delta)
            pwmR += int(1.5*pwm_delta)
        else:
            pwmL -= pwm_delta
            pwmR += pwm_delta
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


if GUI:
    video.release()
    cv2.destroyAllWindows()
runMotor(ser,4,000,000)
time.sleep(2)
#runMotor(ser,3,200,200)
ser.close()

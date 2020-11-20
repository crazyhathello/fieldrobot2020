import serial
import cv2
import numpy as np
import range_finder
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

def checkHSV(capture):
    ret,frame = capture.read()
    range_finder.find_range(frame)
    return True

#ser = startSerialCom()
#runMotor(ser,0,pwmL,pwmR)

#video = cv2.VideoCapture("camera_test_code\\test_video.mp4")
video = cv2.VideoCapture(1)
if video.isOpened() == False:
    print("Error opening video stream or file")
if checkHSV(video):
    print("HSV ready")

while STATE==0:

    ret,frame = video.read()
    if ret == True:
        print("into loop")
        # Image scaling
        scale_percent = 60  # percent of original size
        width = int(frame.shape[1] * scale_percent / 100)
        height = int(frame.shape[0] * scale_percent / 100)
        dim = (width, height)
        frame = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

        # Convert Color range BGR --> RGB --> HSV
        path = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        path = cv2.cvtColor(path, cv2.COLOR_RGB2HSV)

        # Color filtering
        # HSV ranges (0,76,0) to (20,255,255) (use range_finder.py)
        lower_green = np.array([31, 103, 0])
        upper_green = np.array([47, 255, 255])
        
        mask = cv2.inRange(path, lower_green, upper_green)
        
        # Morphological Closing
        #kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (11, 11))
        #close = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

        #res = cv2.bitwise_and(frame, frame, mask=close)
        res = cv2.bitwise_and(frame, frame, mask=mask)

        # Blurring and Smoothing
        blur = cv2.GaussianBlur(mask, (11, 11), 0)

        # Canny edge detection
        edges = cv2.Canny(blur, 100, 200)
        output = cv2.hconcat([frame, res])
        cv2.imshow("Output", output)
        cv2.imshow("Canny", edges)
        #cv2.imshow('frame', mask)
        if cv2.waitKey(25) & 0xFF == ord("q"):
            break
    else:
        break



video.release()
cv2.destroyAllWindows()
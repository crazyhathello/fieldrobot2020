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
        #opening = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel, iterations=1)
        close = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, close_kernel, iterations=1)
        erosion = cv2.erode(close,erosion_kernel,iterations = 1)
        dilation = cv2.dilate(erosion,erosion_kernel,iterations = 1)
        
        close = dilation

        res = cv2.bitwise_and(original, original, mask=close)
        #res = cv2.bitwise_and(original, original, mask=mask)

        # Blurring and Smoothing
        # blur = cv2.GaussianBlur(close, (13, 13), 0)

        # Canny edge detection
        # edges = cv2.Canny(blur, 100, 200)
        
        # cnts = cv2.findContours(close, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # cnts = cnts[0] if len(cnts) == 2 else cnts[1]
        # blank_mask = np.zeros(original.shape, dtype=np.uint8)
        # if cnts:
        #     cnts = sorted(cnts, key=cv2.contourArea, reverse=True)[0]
        #     cv2.fillPoly(blank_mask, [cnts], (255,255,255))
        # blank_mask = cv2.cvtColor(blank_mask, cv2.COLOR_BGR2GRAY)
        # result = cv2.bitwise_and(original,original,mask=blank_mask)

        # x,y,w,h = cv2.boundingRect(blank_mask)
        # ROI = result[y:y+h, x:x+w]

        contours, hierarchy = cv2.findContours(close, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        if contours:    
            contours = sorted(contours,key=cv2.contourArea,reverse=True)[0]
            hull = cv2.convexHull(contours)
            #cv2.drawContours(frame, [hull], 0, (0, 255, 0), 2)
            epsilon = 0.05*cv2.arcLength(hull,True)
            approx = cv2.approxPolyDP(hull,epsilon,True)
            # if len(approx) == 4:    
            #     cv2.drawContours(frame, [approx], 0, (0,255,0), 2)
            #     cv2.line(frame,tuple(approx[0][0]),tuple(approx[1][0]),(0,0,255),2)
            #     cv2.line(frame,tuple(approx[2][0]),tuple(approx[3][0]),(0,0,255),2)
            
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
                #cv2.drawContours(frame, [approx], 0, (0,255,0), 2)
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
            #print(approx[0][0],approx[1][0]) 
            
        # minLineLength = 500
        # maxLineGap = 10
        # lines = cv2.HoughLinesP(edges,1,np.pi/180,100,minLineLength,maxLineGap)
        # if lines is not None:
        #     for x1,y1,x2,y2 in lines[0]:
        #         cv2.line(frame,(x1,y1),(x2,y2),(0,255,0),2)
        
        output = cv2.hconcat([frame, res])
        cv2.imshow("Output", output)
        cv2.imshow("mask", close)
        if cv2.waitKey(25) & 0xFF == ord("q"):
            break
    else:
        break



video.release()
cv2.destroyAllWindows()
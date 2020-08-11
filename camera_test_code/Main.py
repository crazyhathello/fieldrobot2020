import cv2
import numpy as np
import matplotlib.pyplot as plt

def average(image,lines):
    left = []
    right = []
    for line in lines:
        print(line)
        x1, y1, x2, y2 = line.reshape(4)
        parameters = np.polyfit((x1, x2), (y1, y2), 1)
        slope = parameters[0]
        y_int = parameters[1]
        if slope < 0:
            left.append((slope, y_int))
        else:
            right.append((slope, y_int))

    # Average for left and right lines
    right_avg = np.average(right, axis=0)
    left_avg = np.average(left, axis=0)
    left_line = make_points(image, left_avg)
    right_line = make_points(image, right_avg)

    return np.array([left_line, right_line])

def make_points(image, average): 
    slope, y_int = average 
    y1 = image.shape[0]
    y2 = int(y1 * (3/5))
    x1 = int((y1 - y_int) // slope)
    x2 = int((y2 - y_int) // slope)
    
    return np.array([x1, y1, x2, y2])

def display_lines(image, lines):
 lines_image = np.zeros_like(image)
 if lines is not None:
   for line in lines:
     x1, y1, x2, y2 = line
     cv2.line(lines_image, (x1, y1), (x2, y2), (255, 0, 0), 10)
 return lines_image

 
# Start stream
cap = cv2.VideoCapture("camera_test_code\\test_video.mp4")

# Check if camera opened successfully
if cap.isOpened() == False:
    print("Error opening video stream or file")
n = 0
# Read until video is completed
while cap.isOpened():
    # Capture frame-by-frame
    ret, frame = cap.read()
    if ret == True:
        # Image scaling
        scale_percent = 60  # percent of original size
        width = int(frame.shape[1] * scale_percent / 100)
        height = int(frame.shape[0] * scale_percent / 100)
        dim = (width, height)
        frame = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

        # Convert Color range BGR --> RGB --> HSV
        red_line = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        red_line = cv2.cvtColor(red_line, cv2.COLOR_RGB2HSV)

        # Color filtering
        # HSV ranges (0,76,0) to (20,255,255) (use range_finder.py)
        lower_red = np.array([0, 76, 0])
        upper_red = np.array([20, 255, 255])

        mask = cv2.inRange(red_line, lower_red, upper_red)
        
        # Morphological Closing
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (11, 11))
        close = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
        
        res = cv2.bitwise_and(frame, frame, mask=close)

        # Blurring and Smoothing
        blur = cv2.GaussianBlur(close, (11, 11), 0)

        # Canny edge detection
        edges = cv2.Canny(blur, 100, 200)

        # Hough Line transform

        lines = cv2.HoughLinesP(
            edges,
            rho=1,
            theta=np.pi / 180,
            threshold=50,
            minLineLength=150,
            maxLineGap=20,
        )
        if lines is not None:
            for i in range(0, len(lines)):
                l = lines[i][0]
                cv2.line(frame, (l[0], l[1]), (l[2], l[3]), (0, 255, 0), 3, cv2.LINE_AA)
                print(i)

            """
            for x1, y1, x2, y2 in lines[0]:
                cv2.line(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                print((x1,y1,x2,y2))
            print(n)
           """
        # Display the resulting frame
        output = cv2.hconcat([frame, res])
        cv2.imshow("Output", output)
        cv2.imshow("Canny", edges)
        # Press Q on keyboard to  exit
        if cv2.waitKey(25) & 0xFF == ord("q"):
            break

    # Break the loop
    else:
        break

# When everything done, release the video capture object
cap.release()

# Closes all the frames
cv2.destroyAllWindows()

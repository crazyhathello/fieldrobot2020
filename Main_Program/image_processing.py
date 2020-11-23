import cv2
import numpy as np
def filter_green(frame):
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
    
    return dilation
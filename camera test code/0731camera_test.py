#!/usr/bin/env python
# coding: utf-8

# In[1]:


import cv2
import numpy as np
cam = cv2.VideoCapture(0)

if not cam.isOpened():
    print("Cannot open camera")
    exit()

while(True):
    # Capture frame-by-frame
    ret, frame = cam.read()

    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    # show the img
    cv2.imshow('frame', frame)
    # press 'q' to quit
    if cv2.waitKey(1) == ord('q'):
        break

# release
cam.release()
cv2.destroyAllWindows()


# In[ ]:





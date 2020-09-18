import cv2
import numpy as np
import os

img = cv2.imread("road_dataset\\train_segmentation\\road001.png")
print(img.shape)
print(img[1,270])
img[10:200,20:200]=100
cv2.imshow("test",img)
cv2.waitKey(0)
cv2.destroyAllWindows()

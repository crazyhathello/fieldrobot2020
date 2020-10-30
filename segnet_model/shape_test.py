import cv2
import numpy as np
import os

img = cv2.imread("segnet_model\\road_json\\road0_json\\label.png")
print(img.shape)
print(img[800,270])
img_new = img[:,:,2]/128
img_new = img_new.astype(int)
print(img_new.shape)

print(img_new[800,270])
cv2.imshow("test",img)
cv2.waitKey(0)
cv2.destroyAllWindows()

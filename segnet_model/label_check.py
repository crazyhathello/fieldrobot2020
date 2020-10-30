import numpy as np
import cv2
import PIL.Image

lbl = np.asarray(PIL.Image.open("segnet_model\\road_dataset\\train_segmentation\\road001.png"))
print(lbl.dtype)
print(np.unique(lbl))
print(lbl.shape)
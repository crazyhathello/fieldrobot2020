import glob
import cv2
import numpy as np
import os

counter = 1
path = 'C:\\Users\\jense\\Desktop\\fieldrobot\\SegNet model\\road_dataset'
for subdir, dirs, files in os.walk(r'C:\\Users\\jense\\Desktop\\fieldrobot\\SegNet model\\road_json'):
    for filename in files:
        filepath = subdir + os.sep + filename

        if filepath.endswith(".png"):
            if filename == 'label.png':
                label = cv2.imread(filepath)
                label_new = label[:,:,2]
                label_new[:,:] = label_new[:,:]/128

                img = cv2.imread(subdir+os.sep+'img.png')
                if counter <= 455:
                    path_img ='C:\\Users\\jense\\Desktop\\fieldrobot\\SegNet model\\road_dataset\\train_images'
                    path_seg ='C:\\Users\\jense\\Desktop\\fieldrobot\\SegNet model\\road_dataset\\train_segmentation'
                    name = 'road' + str(f'{counter:03}') +'.png'
                    cv2.imwrite(os.path.join(path_img,name),img)
                    cv2.imwrite(os.path.join(path_seg,name),label_new)
                else:
                    path_img ='C:\\Users\\jense\\Desktop\\fieldrobot\\SegNet model\\road_dataset\\val_images'
                    path_seg ='C:\\Users\\jense\\Desktop\\fieldrobot\\SegNet model\\road_dataset\\val_segmentation'
                    name = 'road' + str(f'{counter:03}') +'.png'
                    cv2.imwrite(os.path.join(path_img,name),img)
                    cv2.imwrite(os.path.join(path_seg,name),label_new)

                counter += 1
            print(filepath)


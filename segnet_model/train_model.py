from keras_segmentation.models.segnet import vgg_segnet
import os


model = vgg_segnet(n_classes=2,input_height = 416,input_width = 608) #960,540


model.train(
	train_images = "/home/nas/Research_Group/Personal/Jensen/road_dataset/train_images",
	train_annotations = "/home/nas/Research_Group/Personal/Jensen/road_dataset/train_segmentation",
	val_images = "/home/nas/Research_Group/Personal/Jensen/road_dataset/val_images",
	val_annotations = "/home/nas/Research_Group/Personal/Jensen/road_dataset/val_segmentation",
	checkpoints_path = "/home/nas/Research_Group/Personal/Jensen/road_dataset/checkpoints", epochs = 5
	)

out = model.predict_segmentation(
	inp = "/home/nas/Research_Group/Personal/Jensen/road_dataset/temp/road647_pic.png",
	out_fname="/home/nas/Research_Group/Personal/Jensen/road_dataset/temp/road647_predict.png"
	)

import matplotlib.pyplot as plt 
plt.imshow(out)

print(model.evaluate_segmentation(inp_images_dir = "/home/nas/Research_Group/Personal/Jensen/road_dataset/test_images",annotations_dir = "/home/nas/Research_Group/Personal/Jensen/road_dataset/test_segmentation"))
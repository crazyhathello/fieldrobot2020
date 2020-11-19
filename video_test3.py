from keras_segmentation.predict
import cv2
import numpy as np
cam = cv2.VideoCapture(0)

if not cam.isOpened():
    print("Cannot open camera")
    exit()

while(True):
    # Capture frame-by-frame
    ret, frame = cam.read()
    print(frame.shape)
    #frame = cv2.rotate(frame,cv2.ROTATE_90_COUNTERCLOCKWISE)
    # if frame is read correctly ret is True
    frame = cv2.resize(frame,(960,540))
    print(frame.shape)
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break

    # show the img
    cv2.namedWindow('frame',cv2.WINDOW_KEEPRATIO)
    cv2.imshow('frame', frame)
    # press 'q' to quit
    if cv2.waitKey(1) == ord('q'):
        break

# release
cam.release()
cv2.destroyAllWindows()

# python -m keras_segmentation predict_video \
#  --checkpoints_path="/segnet_model/resnet_segnet_weights.h5" \
#  --input = 0 \
#  --output_file="1113OUTPUT.mp4" \
#  --display

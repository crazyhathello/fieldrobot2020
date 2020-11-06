import cv2
import numpy as np
from PIL import Image
from keras_segmentation.models.segnet import resnet50_segnet
from keras_segmentation.predict import predict_video
from keras_segmentation.predict import visualize_segmentation
from keras_segmentation.data_utils.data_loader import class_colors
#predict_video(checkpoints_path = 'road_dataset\\checkpoints',inp ='C:\\Users\\jense\\Desktop\\fieldrobot\\segnet_model\\testing_videos\\grass_vid_1.mp4',output = 'output.mp4',overlay_img = True,display = True )


# video = cv2.VideoCapture('testing_videos\\grass_vid_1.mp4')

seg_model = resnet50_segnet(n_classes=2, input_height=416, input_width=608)
seg_model.load_weights('resnet_segnet_weights.h5')

# predict_video(seg_model,'testing_videos\\grass_vid_2.mp4','output2.mp4',display = False,overlay_img = True,colors = class_colors)
# predict(seg_model,)

cap = cv2.VideoCapture(0)
while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    pr = predict(seg_model,frame)
    fused_img = visualize_segmentation(
                pr, frame)
    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Display the resulting frame
    cv2.imshow('frame',gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()


def predict(model=None, inp=None, out_fname=None,
            checkpoints_path=None, overlay_img=False,
            class_names=None, show_legends=False, colors=class_colors,
            prediction_width=None, prediction_height=None):

    if model is None and (checkpoints_path is not None):
        model = model_from_checkpoint_path(checkpoints_path)

    assert (inp is not None)
    assert ((type(inp) is np.ndarray) or isinstance(inp, six.string_types)),\
        "Input should be the CV image or the input file name"

    if isinstance(inp, six.string_types):
        inp = cv2.imread(inp)

    assert len(inp.shape) == 3, "Image should be h,w,3 "

    output_width = model.output_width
    output_height = model.output_height
    input_width = model.input_width
    input_height = model.input_height
    n_classes = model.n_classes

    x = get_image_array(inp, input_width, input_height,
                        ordering=IMAGE_ORDERING)
    pr = model.predict(np.array([x]))[0]
    pr = pr.reshape((output_height,  output_width, n_classes)).argmax(axis=2)

    seg_img = visualize_segmentation(pr, inp, n_classes=n_classes,
                                     colors=colors, overlay_img=overlay_img,
                                     show_legends=show_legends,
                                     class_names=class_names,
                                     prediction_width=prediction_width,
                                     prediction_height=prediction_height)

    if out_fname is not None:
        cv2.imwrite(out_fname, seg_img)

    return pr
# while video.isOpened():
#     ret, frame = video.read()
#     frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
#     pr = predict(model = seg_model,inp=frame)
#     fused_img = visualize_segmentation(pr, frame, n_classes=2,colors = colors)
#     print('hit')
#     # output = cv2.hconcat([frame,out])
#     cv2.imshow('frame', seg_img)
#     if cv2.waitKey(25) & 0xFF == ord('q'):
#         break
# video.release()
# cv2.destroyAllWindows()

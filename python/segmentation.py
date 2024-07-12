import cv2
import numpy as np
from ultralytics import YOLO

model = YOLO("yolov8n-seg.yaml")  # build a new model from scratch
model = YOLO("../models/yolov8n-seg.pt")  # load a pretrained model (recommended for training)
model.to("cuda")


def segment(frame):
   
    if frame is None:
        exit()
    original_W=frame.shape[1]
    original_H=frame.shape[0]
    frame=frame.astype(np.uint8)
    results = model(frame,verbose=False)
    
    result=results[0].to("cpu")
    if result.masks is None:
        return np.ones((original_H,original_W),dtype=np.uint8)*255

    total_mask=result.masks.data.numpy().astype(np.uint8)

    conf=result.boxes.conf.numpy()
    cls=result.boxes.cls.numpy()
    
    lable_image=np.zeros([original_H,original_W],dtype=np.uint8)
    
    for i in range(len(conf)):
        if cls[i]>9:
            continue
        if conf[i]<0.5:
            continue

        lable_image=cv2.bitwise_or(lable_image,total_mask[i])
    kernel = np.ones((13,13), np.uint8) 
    lable_image=lable_image*255
    img_dialation = cv2.dilate(lable_image, kernel, iterations=1) 
    img_dialation = cv2.bitwise_not(img_dialation)
    return img_dialation








    # print(results[0])
    # print(results[0].masks)
    # results[0].show()
    
    # return results

# def main(): 
#     cap = cv2.VideoCapture(0)

#     if not cap.isOpened():
#         print("Error: Could not open webcam.")
#         exit()

#     while True:
#         # Capture frame-by-frame
#         ret, frame = cap.read()

#         # If the frame was not captured correctly, break the loop
#         if not ret:
#             print("Error: Could not read frame.")
#             break

#         # Display the frame
#         results=plot(frame)
#         print(np.unique(results))
#         # print(results)
#         cv2.imshow('Webcam Video', results*255)

#         # Break the loop if the user presses the 'q' key
#         if cv2.waitKey(1) & 0xFF == ord('q'):
#             break

#         # Release the webcam and close the window
#     cap.release()
#     cv2.destroyAllWindows()
  
  
# # Using the special variable  
# # __name__ 
# if __name__=="__main__": 
#     main() 
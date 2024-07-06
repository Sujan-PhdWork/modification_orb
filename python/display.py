import cv2
import numpy as np
from ultralytics import YOLO

model = YOLO("yolov8n-seg.yaml")  # build a new model from scratch
model = YOLO("../models/yolov8n-seg.pt")  # load a pretrained model (recommended for training)
model.to("cuda")


def plot(x):
    print("right this time")
    x=x.astype(np.uint8)
    results = model(x,verbose=False)
    edges = cv2.Canny(x,100,200)
    return edges


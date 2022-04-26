import tensorflow as tf
from tf.keras.models import load_model
from PIL import Image, ImageOps
import numpy as np
import cv2
# Load the model
model=load_model('keras_model.h5') 
# teachablemachine으로부터 다운받은 keras_model.h5 와 labels.txt 파일을 현재 폴더로 카피해옴
data=np.ndarray(shape=(1, 224, 224, 3), dtype=np.float32)
size= (224, 224) 
# teachablemachine에서 학습에 사용된 영상의 사이즈는 244 X 244이기 때문에 나중에 usb camera로부터 취득된 영상을 이 사이즈로 변경해줘야 함
cap=cv2.VideoCapture(0)  
while True:
    ret, image=cap.read() 
    image1=cv2.resize(image, dsize=size, interpolation=cv2.INTER_AREA)
    image_array=np.asarray(image1) # 리사이즈된 영상을 image_array에 저장
    normalized_image_array= (image_array.astype(np.float32) / 127.0) - 1
    data[0] = normalized_image_array # 
    # run the inference
    prediction=model.predict(data)
    print(prediction) 
    cv2.imshow('object detection',image) 
    # 카메라로부터의 영상을 화면에 실시간으로 표시함
    cv2.waitKey(30) & 0xff
    if k==27: 
        break
cap.release()
cv2.destroyAllWindows()
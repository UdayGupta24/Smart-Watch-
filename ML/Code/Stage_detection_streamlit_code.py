import streamlit as st
import pandas as pd
import io
import numpy as np
from scipy.signal import butter, filtfilt
import tensorflow as tf
import copy
st.title("Hi I am Streamlit Web App for your Stage Detection")

file=st.file_uploader("pls upload your excel file here")

#code for uploading the model

@st.cache_resource
def load_model():
    model = tf.keras.models.load_model('stage_model.h5')  
    return model

model=load_model()


def Z_score_Normalization(sig):
  signals=copy.deepcopy(sig)
  for j in range(3):
    sns=signals[:,j]
    m=np.mean(sns)
    std=np.std(sns)
    normal=(sns-m)/std
    signals[:,j]=normal
  return signals

def butterworth(sig,cutoff,order,sampling_frequency):
  signals=copy.deepcopy(sig)
  wn=(2*cutoff)/(sampling_frequency)
  b,a=butter(order,wn,btype='low',analog=False)
  for j in range(3):
    sns=signals[:,j]
    filter_signal=filtfilt(b,a,sns)
    signals[:,j]=filter_signal
  return signals

if file is not None:
    df = pd.read_csv(io.BytesIO(file.read()))
    acc_x=df.iloc[0:200,6]
    acc_y=df.iloc[0:200,7]
    acc_z=df.iloc[0:200,8]
    signal=np.zeros((200,3))
    signal[:,0]=acc_x
    signal[:,1]=acc_y
    signal[:,2]=acc_z
    normalized=Z_score_Normalization(signal)
    filtered_signal = butterworth(normalized, cutoff=20, order=4, sampling_frequency=70)
    input_data = filtered_signal.reshape(1, 200, 3) #batch_size,timestamps,feature


    prediction = model.predict(input_data)
    predicted_class = np.argmax(prediction, axis=1)[0]
    class_name = ["Walking", "Running", "Standing", "hand movement while walking"]
    
    st.write(f"Predicted Stage is : {class_name[predicted_class]}")


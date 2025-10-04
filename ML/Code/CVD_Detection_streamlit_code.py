import streamlit as st
import pandas as pd
import io
import numpy as np
from scipy.signal import butter, filtfilt
import tensorflow as tf

st.title("Hi I am Streamlit Web App for your Cardio Vascular Disease Detection")

file=st.file_uploader("pls upload your excel file here")

#code for uploading the model

@st.cache_resource
def load_model():
    model = tf.keras.models.load_model('ppg_model.h5') 
    return model

model=load_model()

def Z_score_Normalization(signal):
    m = np.mean(signal)
    std = np.std(signal)
    normalized = (signal - m) / std
    return normalized

def butterworth(signal, cutoff, order, sampling_frequency):
    wn = (2 * cutoff) / sampling_frequency
    b, a = butter(order, wn, btype='low', analog=False)
    filtered_signal = filtfilt(b, a, signal)
    return filtered_signal

if file is not None:
    df = pd.read_csv(io.BytesIO(file.read()))
    green_count=df.iloc[0:2100,2]
    green2_count=df.iloc[0:2100,3]
    ppg_signal=(green_count+green2_count)/2
    ppg_signal = ppg_signal.values
    
    normalized_signal = Z_score_Normalization(ppg_signal)
    filtered_signal = butterworth(normalized_signal, cutoff=15, order=4, sampling_frequency=1000)
    input_data = filtered_signal.reshape(1, 2100, 1) #batch_size,timestamps,feature

    prediction = model.predict(input_data)
    predicted_class = np.argmax(prediction, axis=1)[0]
    class_name = ["Normal", "Prehypertension", "Stage 1 Hypertension", "Stage 2 Hypertension"]
    st.write(f"Predicted CVD: {class_name[predicted_class]}")


        




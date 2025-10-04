# Machine Learning Work - Smart Watch Project
This Folder contains code for deep learning model,research paper,result and the final trained models.

# Introduction
 Two Deep Learning Models have been used.
 1.Detection of Cardiovascular Disease.
 2.Person State.

# Purpose of each model
1. **Cardiovascular Disease Detection**
   - Uses PPG signals from MAXREFDES103 sensor.  
   - Output : `Hypertension (High) Stage-1`,`Hypertension Stage-2`, `Prehypertension  (Medium)`,       `Normal`.  
   - Input: Excel files with sensor data.  
   - Remark: Rough estimate of cardiovascular health.
2. **Person State Detection**
   - Ensures accurate step counting by checking hand-leg synchronization.  
   - Input: Accelerometer data (Excel file).  
   - Output: `Standing`, `Walking`, `Running`, `Hand involvement in walking`.
## Model Details

- **Framework:** Python (Google Colab)  
- **Architecture:** CNN (signal patterns), RNN (time-dependent features)  
- **Trained Models:** `.h5` files 

## GUI

- Built using **Streamlit**.  
- Upload Excel files and get predictions.  
- Currently runs on **local host only**.

## Folder Contents

|Folder | Description |
|-------------|-------------|
| `Code` | Training script for models and Code for Streamlit (GUI) |
| `Models` | Final trained model in .h5 |
| `research paper` | Research notes and reference |
| `results/` | Graphs |

## Future Enhancements
 
- Stress and emotional health detection  
- Sleep stage and apnea monitoring  

---

## Author

**Uday Gupta**

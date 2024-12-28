import tkinter as tk
from tkinter import messagebox
import joblib
import numpy as np
import serial
import time

# Load the trained model
model = joblib.load('temperature_model.pkl')

# Setup serial communication with Arduino
arduino = serial.Serial('COM3', 9600, timeout=1)  # Replace 'COM3' with the correct port
time.sleep(2)  # Give the Arduino some time to reset

# Function to predict temperature based on humidity
def predict_temperature(humidity):
    # Make the prediction using the trained model
    predicted_temp = model.predict(np.array([[humidity]]))
    result_label.config(text=f"Predicted Temperature: {predicted_temp[0]:.2f}°C")

# Function to read data from Arduino and update the GUI
def read_arduino_data():
    if arduino.in_waiting > 0:
        # Read data from Arduino
        data = arduino.readline().decode('utf-8').strip()
        
        # Print the raw data for debugging purposes
        print(data)

        # Check if the data contains all necessary fields
        if "Humidity" in data and "Temperature" in data and "Predicted Temp" in data and "Water Level" in data:
            # Parse the data from Arduino
            parts = data.split(',')
            humidity = float(parts[0].split(':')[1].strip())
            temperature = float(parts[1].split(':')[1].strip())
            predicted_temp = float(parts[2].split(':')[1].strip())
            water_level = float(parts[3].split(':')[1].strip())

            # Update the labels with the Arduino data
            arduino_temp_label.config(text=f"Arduino Temperature: {temperature:.2f}°C")
            arduino_humidity_label.config(text=f"Arduino Humidity: {humidity:.2f}%")
            arduino_water_label.config(text=f"Water Level: {water_level:.2f} cm")

            # Predict the temperature based on the humidity
            predict_temperature(humidity)

    # Call this function again after a short delay
    window.after(1000, read_arduino_data)

# Create the main window
window = tk.Tk()
window.title("Temperature Prediction App with Arduino")

# Create and place widgets
label = tk.Label(window, text="Humidity and Temperature from Arduino")
label.pack(pady=10)

arduino_temp_label = tk.Label(window, text="Arduino Temperature: N/A", font=("Arial", 12))
arduino_temp_label.pack(pady=10)

arduino_humidity_label = tk.Label(window, text="Arduino Humidity: N/A", font=("Arial", 12))
arduino_humidity_label.pack(pady=10)

arduino_water_label = tk.Label(window, text="Water Level: N/A", font=("Arial", 12))
arduino_water_label.pack(pady=10)

result_label = tk.Label(window, text="Predicted Temperature: N/A", font=("Arial", 14))
result_label.pack(pady=20)

# Start reading data from Arduino
read_arduino_data()

# Run the app
window.mainloop()

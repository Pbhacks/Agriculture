import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
import joblib

# Sample data: temperature (Celsius) vs. humidity (%)
data = {
    'Temperature': [22, 23, 24, 25, 26, 27, 28, 29, 30, 31],
    'Humidity': [60, 62, 63, 64, 65, 66, 68, 70, 72, 74],
}

# Create a DataFrame
df = pd.DataFrame(data)

# Define features (humidity) and target (temperature)
X = df[['Humidity']]  # Feature (humidity)
y = df['Temperature']  # Target (temperature)

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Create the model
model = LinearRegression()

# Train the model
model.fit(X_train, y_train)

# Save the trained model to a file
joblib.dump(model, 'temperature_model.pkl')

print("Model trained and saved successfully!")

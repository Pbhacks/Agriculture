#include <LiquidCrystal.h>
#include <DHT.h>

// Pin Definitions
#define DHTPIN 2
#define DHTTYPE DHT11 // Change to DHT22 if applicable
#define TRIGPIN 3
#define ECHOPIN 4
#define CONTRAST_PIN 6 // Contrast pin (PWM)

// Initialize LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Variables for Prediction
float temperatureReadings[5] = {0, 0, 0, 0, 0};
int index = 0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Set up contrast pin
  pinMode(CONTRAST_PIN, OUTPUT);
  analogWrite(CONTRAST_PIN, 100); // Initial contrast (adjust value as needed)

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Initialize DHT Sensor
  dht.begin();

  // Set up sonar sensor pins
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    delay(2000);
    lcd.clear();
    return;
  }

  // Update temperature readings for prediction
  temperatureReadings[index] = temperature;
  index = (index + 1) % 5; // Circular buffer
  float predictedTemp = predictTemperature();

  // Measure water level using HC-SR04
  float waterLevel = measureDistance();

  // Display readings on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Pred: ");
  lcd.print(predictedTemp);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(waterLevel);
  lcd.print("cm");
  delay(2000);
  lcd.clear();

  // Send the data to Python via Serial
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(", Temperature: ");
  Serial.print(temperature);
  Serial.print(", Predicted Temp: ");
  Serial.print(predictedTemp);
  Serial.print(", Water Level: ");
  Serial.println(waterLevel);

  // Optionally, adjust contrast dynamically in future enhancements
  // analogWrite(CONTRAST_PIN, newValue); 
}

// Predict temperature using a moving average
float predictTemperature() {
  float sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += temperatureReadings[i];
  }
  return sum / 5; // Return the average
}

// Measure distance using HC-SR04
float measureDistance() {
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);

  long duration = pulseIn(ECHOPIN, HIGH);
  float distance = (duration * 0.034) / 2; // Convert to cm
  return distance;
}

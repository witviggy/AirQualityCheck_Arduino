#include <LiquidCrystal_I2C.h>

// Define the pins for the sensors, LED display, and buzzer
const int MQ135Pin = A0;
const int MQ4Pin = A1;
const int MQ3Pin = A2;
const int HumidityPin = A3;
const int buzzerPin = 8;
LiquidCrystal_I2C lcd(0x27, 16, 2);
float MQ135Resistance = 0, MQ4Resistance = 0, MQ3Resistance = 0, Humidity = 0;
float AQIThreshold = 100.0; // Define the AQI threshold for bad air quality

void setup() {
  // Set up the LED display and buzzer
  lcd.begin(16, 2);
  lcd.init();
  lcd.clear();
  lcd.backlight(); // Make sure backlight is on
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  lcd.setCursor(2, 0);
  // Print the initial message on the LED display
  lcd.print("Air Quality:");
}

void loop() {
  // Read the sensor values
  MQ135Resistance = analogRead(MQ135Pin);
  MQ4Resistance = analogRead(MQ4Pin);
  MQ3Resistance = analogRead(MQ3Pin);
  Humidity = analogRead(HumidityPin);
  // Convert the sensor values to corresponding units
  float MQ135Voltage = MQ135Resistance * (5.0 / 1023.0);
  float MQ4Voltage = MQ4Resistance * (5.0 / 1023.0);
  float MQ3Voltage = MQ3Resistance * (5.0 / 1023.0);
  float HumidityVoltage = Humidity * (5.0 / 1023.0);
  float MQ135PPM = 0, MQ4PPM = 0;
  // Calculate the PPM for MQ135 and MQ4 sensors
  MQ135PPM = (9.9 * pow((MQ135Voltage / 5.0), -1.5)) - 0.1;
  MQ4PPM = (pow(10, ((log10(MQ4Voltage / 5.0) - 1.2535) / (-0.2375))))/10000;

  // Display the data on the LED display
  lcd.setCursor(0, 1);
  lcd.print("MQ135 PPM:");
  Serial.print("MQ135: ");
  Serial.print(MQ135PPM);
  Serial.println(" PPM");
  lcd.print(MQ135PPM);
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("MQ4 PPM: ");
  Serial.print("MQ4: ");
  Serial.print(MQ4PPM);
  Serial.println(" PPM");
  lcd.print(MQ4PPM);
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Humidity:   ");
  Serial.print("Humidity: ");
  Serial.print(HumidityVoltage);
  Serial.println(" V");
  lcd.print(HumidityVoltage);
  delay(2000);

  // Calculate and display overall AQI percentage
  float overallAQI = calculateOverallAQI(MQ135PPM, MQ4PPM, HumidityVoltage);
  Serial.print("Overall AQI: ");
  Serial.print(overallAQI);
  Serial.println("%");

  // Check if air quality is bad (above the threshold)
  if (overallAQI > AQIThreshold) {
    activateBuzzer();
  }
}

float calculateOverallAQI(float MQ135PPM, float MQ4PPM, float HumidityVoltage) {
  // Calculate the overall AQI based on the sensor readings
  // You can customize the calculation method according to your requirements
  float overallAQI = ((MQ135PPM + MQ4PPM + HumidityVoltage) / 3.0)/2 ;
  return overallAQI;
}

void activateBuzzer() {
  // Activate the buzzer to indicate bad air quality
  digitalWrite(buzzerPin, HIGH);
  delay(1000); // Buzzer active for 1 second
  digitalWrite(buzzerPin, LOW);
}

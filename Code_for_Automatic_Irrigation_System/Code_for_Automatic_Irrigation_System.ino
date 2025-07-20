#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 7       // Connect DHT11 data pin to Arduino digital pin 7
#define DHTTYPE DHT11  // Specify DHT11 sensor type
#define RELAY_PIN 2    // Relay pin (unchanged)
#define MOISTURE_PIN A0 // Soil moisture sensor pin (unchanged)

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  
  dht.begin();  // Initialize DHT sensor
  
  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
  delay(3000);
  lcd.clear();
}

void loop() {
  // Read sensors
  int soilMoisture = analogRead(MOISTURE_PIN);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Print sensor values to Serial Monitor
  Serial.println("Soil Moisture: " + String(soilMoisture));
  Serial.println("Temperature: " + String(temperature) + "°C");
  Serial.println("Humidity: " + String(humidity) + "%");
  
  // Control water pump based on soil moisture
  if (soilMoisture > 950) {
    digitalWrite(RELAY_PIN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is ON ");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is OFF");
  }
  
  // Display moisture level
  if (soilMoisture < 300) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture: HIGH");
  } else if (soilMoisture > 300 && soilMoisture < 950) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture: MID ");
  } else if (soilMoisture > 950) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture: LOW ");
  }
  
  // Display temperature and humidity every 5 seconds
  delay(5000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity, 0);
  lcd.print("%");
  
  delay(3000);
  lcd.clear();
}
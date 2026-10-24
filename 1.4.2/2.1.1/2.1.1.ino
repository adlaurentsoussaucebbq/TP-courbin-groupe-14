
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

Adafruit_BME680 bme;

void setup() {
  Serial.begin(115200);

  // SDA = GPIO 21, SCL = GPIO 22
  Wire.begin(21, 22);

  Serial.println("[Binome 14 - Allouche & Randrianarisoa] Initialisation BME680");

  // Adresse I2C par defaut : 0x77
  if (!bme.begin(0x77)) {
    Serial.println("[Binome 14 - Allouche & Randrianarisoa] Capteur non detecte !");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("[Binome 14 - Allouche & Randrianarisoa] Capteur detecte !");
}

void loop() {

  if (!bme.performReading()) {
    Serial.println("[Binome 14 - Allouche & Randrianarisoa] Erreur de lecture");
    delay(2000);
    return;
  }

  // Temperature en Celsius
  float tempC = bme.temperature;

  // Conversion en Fahrenheit
  float tempF = tempC * 9.0 / 5.0 + 32.0;

  // Humidite en %
  float humidite = bme.humidity;

  Serial.print("[Binome 14 - Allouche & Randrianarisoa] ");

  Serial.print("Temperature : ");
  Serial.print(tempC, 2);
  Serial.print(" C | ");

  Serial.print(tempF, 2);
  Serial.print(" F | ");

  Serial.print("Humidite : ");
  Serial.print(humidite, 2);
  Serial.println(" %");

  delay(2000);
}

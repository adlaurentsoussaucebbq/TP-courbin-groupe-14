
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <AdafruitIO_WiFi.h>

// Informations Wi-Fi
#define WIFI_SSID "Iphone de Evan"
#define WIFI_PASS "86754321"

// Informations Adafruit IO
#define IO_USERNAME "naveeeeeeeeee"
#define IO_KEY "aio_nKpP51m2V80UEa4qqaJcRHUrbbfY"

// Prefixe du binome
const String PREFIX =
  "[Binome 14 - Allouche & Randrianarisoa] ";

// Connexion Adafruit IO
AdafruitIO_WiFi io(
  IO_USERNAME,
  IO_KEY,
  WIFI_SSID,
  WIFI_PASS
);

// Capteur BME680
Adafruit_BME680 bme;

// Feeds Adafruit IO
AdafruitIO_Feed *temperatureFeed =
  io.feed("temperature");

AdafruitIO_Feed *humiditeFeed =
  io.feed("humidite");

// Envoi toutes les 30 secondes
const unsigned long intervalle = 30000;
unsigned long dernierEnvoi = 0;
bool premierEnvoi = true;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialisation I2C
  Wire.begin(21, 22);

  Serial.println(PREFIX + "Initialisation BME680");

  if (!bme.begin(0x77) && !bme.begin(0x76)) {
    Serial.println(PREFIX + "BME680 non detecte !");

    while (true) {
      delay(1000);
    }
  }

  Serial.println(PREFIX + "BME680 detecte !");

  // Connexion Wi-Fi + Adafruit IO
  Serial.println(PREFIX + "Connexion Adafruit IO...");

  io.connect();

  // Attente de la connexion
  unsigned long debut = millis();

  while (io.status() < AIO_CONNECTED &&
         millis() - debut < 20000) {
    Serial.println(PREFIX + "Connexion en cours...");
    delay(500);
  }

  if (io.status() >= AIO_CONNECTED) {
    Serial.println(PREFIX + "Adafruit IO connecte !");
  } else {
    Serial.println(PREFIX + "Connexion non etablie, nouvelle tentative dans loop");
  }
}

void loop() {
  // Maintenir la connexion MQTT
  io.run();

  if (io.status() < AIO_CONNECTED) {
    return;
  }

  // Envoi toutes les 30 secondes
  if (premierEnvoi ||
      millis() - dernierEnvoi >= intervalle) {

    premierEnvoi = false;
    dernierEnvoi = millis();

    // Lecture du BME680
    if (!bme.performReading()) {
      Serial.println(PREFIX + "Erreur lecture BME680");
      return;
    }

    float temperature = bme.temperature;
    float humidite = bme.humidity;

    // Affichage local
    Serial.print(PREFIX + "Temperature : ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print(PREFIX + "Humidite : ");
    Serial.print(humidite);
    Serial.println(" %");

    // Envoi vers Adafruit IO
    bool envoiTemp = temperatureFeed->save(temperature);
    bool envoiHum = humiditeFeed->save(humidite);

    if (envoiTemp && envoiHum) {
      Serial.println(PREFIX + "Donnees envoyees !");
    } else {
      Serial.println(PREFIX + "Erreur envoi MQTT");
    }
  }
}

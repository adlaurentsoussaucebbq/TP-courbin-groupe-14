
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Informations Wi-Fi
const char* ssid = "Iphone de Evan";
const char* password = "86754321";

// Informations Weatherstack
const char* apiKey = "3e7ec78893115d1e4d0e15f786c91071";
const char* ville = "Montcuq";

const String PREFIX =
  "[Binome 14 - Allouche & Randrianarisoa] ";

// Donnees meteo
String nomVille = "";
String heure = "";
float temperature = 0;
float vent = 0;

bool donneesDisponibles = false;
bool pageMeteo = false;

unsigned long dernierChangement = 0;

void afficherLCD() {
  lcd.clear();

  if (pageMeteo == false) {
    // Premiere page : ville et heure
    lcd.setCursor(0, 0);
    lcd.print("Ville: ");
    lcd.print(nomVille.substring(0, 9));

    lcd.setCursor(0, 1);
    lcd.print("Heure: ");

    // Extraction HH:MM de la date
    if (heure.length() >= 16) {
      lcd.print(heure.substring(11, 16));
    } else {
      lcd.print(heure);
    }

  } else {
    // Deuxieme page : temperature et vent
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Vent: ");
    lcd.print(vent, 1);
    lcd.print(" km/h");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialisation LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connexion WiFi");

  // Connexion Wi-Fi
  Serial.println(PREFIX + "Connexion au Wi-Fi...");

  WiFi.begin(ssid, password);

  int tentatives = 0;

  while (WiFi.status() != WL_CONNECTED &&
         tentatives < 40) {
    delay(500);
    tentatives++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(PREFIX + "Echec connexion Wi-Fi");

    lcd.clear();
    lcd.print("Erreur WiFi");
    return;
  }

  Serial.println(PREFIX + "Wi-Fi connecte !");

  lcd.clear();
  lcd.print("WiFi connecte");
  delay(1000);

  // Construction URL API
  String url = "https://api.weatherstack.com/current";
  url += "?access_key=";
  url += apiKey;
  url += "&query=";
  url += ville;
  url += "&units=m";

  WiFiClientSecure client;
  client.setInsecure(); // Uniquement pour test

  HTTPClient http;
  http.setTimeout(10000);

  lcd.clear();
  lcd.print("Chargement...");
  
  if (!http.begin(client, url)) {
    Serial.println(PREFIX + "Erreur connexion HTTP");
    lcd.clear();
    lcd.print("Erreur HTTP");
    return;
  }

  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.print(PREFIX + "Erreur HTTP : ");
    Serial.println(httpCode);

    lcd.clear();
    lcd.print("Erreur API");
    http.end();
    return;
  }

  String reponse = http.getString();
  http.end();

  // Lecture JSON
  JsonDocument doc;

  DeserializationError erreur =
    deserializeJson(doc, reponse);

  if (erreur) {
    Serial.println(PREFIX + "Erreur JSON");
    lcd.clear();
    lcd.print("Erreur JSON");
    return;
  }

  if (doc["error"].is<JsonObject>()) {
    Serial.print(PREFIX + "Erreur API : ");
    Serial.println(doc["error"]["info"].as<String>());

    lcd.clear();
    lcd.print("Erreur API");
    return;
  }

  // Recuperation des donnees
  nomVille = doc["location"]["name"].as<String>();
  heure = doc["location"]["localtime"].as<String>();

  temperature =
    doc["current"]["temperature"].as<float>();

  vent =
    doc["current"]["wind_speed"].as<float>();

  donneesDisponibles = true;

  // Affichage sur le Serial Monitor
  Serial.println(PREFIX + "Donnees meteo :");
  Serial.println(PREFIX + "Ville : " + nomVille);
  Serial.println(PREFIX + "Heure : " + heure);

  Serial.print(PREFIX + "Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print(PREFIX + "Vent : ");
  Serial.print(vent);
  Serial.println(" km/h");

  // Premier affichage LCD
  afficherLCD();
  dernierChangement = millis();
}

void loop() {
  if (!donneesDisponibles) {
    return;
  }

  // Changement d'ecran toutes les 3 secondes
  if (millis() - dernierChangement >= 3000) {
    dernierChangement = millis();

    pageMeteo = !pageMeteo;

    afficherLCD();
  }
}

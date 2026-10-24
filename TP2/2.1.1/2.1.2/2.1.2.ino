
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String message = "Great IoT Tutorial!    ";

void setup() {
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();
}

void loop() {
  unsigned long secondes = millis() / 1000;

  // Defilement du message sur la premiere ligne
  int position = secondes % (message.length() - 15);

  lcd.setCursor(0, 0);
  lcd.print(message.substring(position, position + 16));

  // Compteur sur la deuxieme ligne
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);

  lcd.print("Launched for ");
  lcd.print(secondes);
  lcd.print("s");

  delay(1000);
}

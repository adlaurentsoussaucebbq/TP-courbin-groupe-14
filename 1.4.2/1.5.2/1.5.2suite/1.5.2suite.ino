int valeurMin = 4095;
int valeurMax = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  Serial.println("[Binome 14 - Allouche & Randrianarisoa] Calibration");

  unsigned long debut = millis();

  while (millis() - debut < 10000) {
    int valeur = analogRead(36);

    if (valeur < valeurMin) {
      valeurMin = valeur;
    }

    if (valeur > valeurMax) {
      valeurMax = valeur;
    }
      Serial.println("[Binome 14 - Allouche & Randrianarisoa] Valeur : ");
      Serial.print(valeur);
      Serial.print(" | Min : ");
      Serial.print(valeurMin);
      Serial.print(" | Max : ");
      Serial.println(valeurMax);
    delay(10);
  }

  Serial.println("[Binome 14 - Allouche & Randrianarisoa] calibration finie");
}

void loop() {
  int valeur = analogRead(36);

  int valeurCalibree = 0;

  if (valeurMax > valeurMin) {
    valeurCalibree = map(
      valeur, valeurMin, valeurMax, 0, 255
    );
  }

  valeurCalibree = constrain(valeurCalibree, 0, 255);

  Serial.println("[Binome 14 - Allouche & Randrianarisoa] Luminosite : ");
  Serial.println(valeurCalibree);

  delay(1000);
}

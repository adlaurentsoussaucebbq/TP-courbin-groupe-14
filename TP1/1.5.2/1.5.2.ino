void setup() {
  Serial.begin(115200);
  analogReadResolution(12); //on met en 12 bits pour que ce soit de 0 à 4095
}

void loop() {
  int valeur = analogRead(36);
  Serial.println("[Binome 14 - Allouche & Randrianarisoa] valeur LDR : ");
  Serial.println(valeur);
  delay(1000);
}

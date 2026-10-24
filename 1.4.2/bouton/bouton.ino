int compteur = 0;
int etatPrecedent = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(34, INPUT);
}

void loop() {
  int etatBouton = digitalRead(34);

  if (etatPrecedent == HIGH && etatBouton == LOW) {
    compteur++;

    Serial.print("[Binome 14 - ALLOUCHE & RANDRIANARISOA] Compteur : ");
    Serial.println(compteur);
  }

  etatPrecedent = etatBouton;

  delay(20);
}
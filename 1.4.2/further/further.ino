
#include <Arduino.h>



void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  
  analogSetPinAttenuation(36, ADC_0db);
}

void loop() {
  
  uint32_t tension_mV = analogReadMilliVolts(TEMP_PIN);

  float temperature = (tension_mV - 500.0f) / 10.0f;

  Serial.print("[Binome 14 - Allouche & Randrianarisoa] Tension : ");
  Serial.print(tension_mV);
  Serial.print(" mV | Temperature : ");
  Serial.print(temperature, 2);
  Serial.println(" C");

  delay(1000);
}

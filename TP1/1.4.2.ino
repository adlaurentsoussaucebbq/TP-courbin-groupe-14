void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

}


void loop() {
  Serial.println("[Binome 14 - Allouche & Randrianarisoa] Led ON");
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(700);        // 14*50
  Serial.println("[Binome 14 - Allouche & Randrianarisoa] Led OFF");
  digitalWrite(LED_BUILTIN, LOW);
     
  delay(1300);     //2000-700                 
}

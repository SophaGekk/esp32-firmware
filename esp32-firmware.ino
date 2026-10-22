#include <WiFi.h>

void setup() {
  Serial.begin(115200, SERIAL_8N1, 16, 17);
  Serial1.begin(115200);
  
  Serial1.println("--- STARTING ---");
  
  // Встроенный светодиод на большинстве плат ESP32 — это GPIO 2
  pinMode(2, OUTPUT);
}

void loop() {
  Serial1.println("Hello from ESP32! System is running.");
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}

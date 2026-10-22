#include <WiFi.h>

const char* ssid = "Redmi Note 13 Pro";       // ВАШЕ ИМЯ СЕТИ
const char* password = "x7ks77du";   // ВАШ ПАРОЛЬ

WiFiServer server(3333);
WiFiClient client;

void setup() {
  Serial.begin(115200); 
  delay(1000); // Пауза, чтобы вы успели открыть PuTTY

  Serial.println("\n--- STARTING WIFI TEST ---");
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 
  Serial.println("\n--- STARTING ---");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 40) {
    delay(500);
    Serial.print(".");
    timeout++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nSUCCESS! Connected to WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); 
    server.begin();
    Serial.println("TCP Server started on port 3333");
  } else {
    Serial.println("\nFAILED to connect.");
  }
}

void loop() {
  if (!client || !client.connected()) {
    client = server.available();
    return;
  }

  // Данные от STM32 -> планшету
  while (Serial2.available()) {
    client.write(Serial2.read());
  }
  // Данные от планшета -> STM32
  while (client.available()) {
    Serial2.write(client.read());
  }
}

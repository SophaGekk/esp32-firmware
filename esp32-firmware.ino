#include <WiFi.h>
#include "esp_task_wdt.h" // Библиотека для управления Watchdog

const char* ssid = "Redmi Note 13 Pro";
const char* password = "x7ks77du"; 

WiFiServer server(3333);
WiFiClient client;

void setup() {
  // Отключаем Watchdog, чтобы ESP32 не перезагружалась во время подключения
  esp_task_wdt_deinit(); 

  Serial.begin(115200, SERIAL_8N1, 16, 17);
  Serial1.begin(115200);

  Serial1.println("");
  Serial1.print("Connecting to ");
  Serial1.println(ssid);

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  
  int timeout = 0;
  // Ждем до 20 секунд
  while (WiFi.status() != WL_CONNECTED && timeout < 40) { 
    delay(500);
    Serial1.print(".");
    timeout++;
  }
  
  Serial1.println("");
  if (WiFi.status() == WL_CONNECTED) {
    Serial1.print("Connected! IP: ");
    Serial1.println(WiFi.localIP()); 
  } else {
    Serial1.println("Failed to connect (Timeout)");
    Serial1.println("Check if phone hotspot is 2.4GHz");
  }
  
  server.begin();
  Serial1.println("Server started on port 3333");
}

void loop() {
  if (!client || !client.connected()) {
    client = server.available();
    return;
  }
  while (Serial.available()) client.write(Serial.read());
  while (client.available()) Serial.write(client.read());
}

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Redmi Note 13 Pro";       
const char* password = "x7ks77du";   

// --- НАСТРОЙКИ MQTT ---
// IP-адрес компьютера, на котором запущен Docker с Mosquitto
const char* mqtt_server = "192.168.1.6"; 
const int mqtt_port = 1883;
const char* mqtt_topic = "comfort/sensor/aud-102";

WiFiClient espClient;
PubSubClient client(espClient);
String jsonBuffer = "";

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Создаем уникальный Client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); 
  // UART для связи со STM32 (RX=16, TX=17) - поменяйте пины, если нужно
  Serial.println("\n--- STARTING WIFI TEST ---");
  Serial2.begin(9600, SERIAL_8N1, 25, 26); 
  Serial.println("\n--- STARTING ---");

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Читаем данные от STM32 по UART
  while (Serial2.available()) {
    char c = Serial2.read();
    
    // Если встретили символ перевода строки - значит JSON собран полностью
    if (c == '\n') {
      jsonBuffer.trim(); // Убираем \r
      
      if (jsonBuffer.length() > 0) {
        Serial.println("Отправка в MQTT: " + jsonBuffer);
        // Отправляем в топик
        client.publish(mqtt_topic, jsonBuffer.c_str());
      }
      jsonBuffer = ""; // Очищаем буфер для следующей строки
    } 
    else if (c != '\r') {
      jsonBuffer += c; // Накапливаем символы
    }
  }
}

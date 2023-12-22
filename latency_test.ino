#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char* clientId = "ESP32";
const char* topic = "test/latency"; 

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long sendTimestamp = 0;
unsigned long receiveTimestamp = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  esp_server_connection();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  sendTimestamp = millis(); // Timestamp before sending
  sendMQTTData();
  delay(1000); 
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void reconnectMQTT() {
  while (!client.connected()) {
    if (client.connect(clientId)) {
      Serial.println("Connected to MQTT");
    } else {
      delay(5000);
    }
  }
}

void esp_server_connection() {
  client.setServer(mqtt_broker, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(clientId)){
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void sendMQTTData() {
  char msg[50];
  snprintf(msg, 50, "Test Message");
  client.publish(topic, msg);

  receiveTimestamp = millis();
  unsigned long latency = receiveTimestamp - sendTimestamp;
  Serial.print("Latency (ms): ");
  Serial.println(latency);
}

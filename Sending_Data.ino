#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Ahmed-home@GIGABIT FIBER";
const char* password = "AoA.0187731709";
const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char* clientId = "ESP32";

WiFiClient espClient;
PubSubClient client(espClient);

const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

#define xAxis 34
#define yAxis 35
#define zButton 23
int xValue, yValue, zValue;

void setup_wifi();
void reconnectMQTT();
void mpu6050_init();
void readMPU6050Data();
void readJoystickData();
void sendMQTTData(const char* topic, int value);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  mpu6050_joystick_init();
  esp_server_connection();

}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  readMPU6050Data();
  readJoystickData();
  sendMQTTData("mpu6050/AcX", AcX);
  sendMQTTData("mpu6050/AcY", AcY);
  sendMQTTData("mpu6050/AcZ", AcZ);
  sendMQTTData("mpu6050/GyX", GyX);
  sendMQTTData("mpu6050/GyY", GyY);
  sendMQTTData("mpu6050/GyZ", GyZ);
  sendMQTTData("joystick/x", xValue);
  sendMQTTData("joystick/y", yValue);
  sendMQTTData("joystick/button", zValue);
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
    } else {
      delay(5000);
    }
  }
}
void mpu6050_joystick_init() {
  Wire.begin(21, 22);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  pinMode(zButton, INPUT);
}
void esp_server_connection() {
    client.setServer(mqtt_broker, mqtt_port);
  while (!client.connected()) {

    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client")){
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

}
void readMPU6050Data() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}
void readJoystickData() {
  xValue = analogRead(xAxis);
  yValue = analogRead(yAxis);
  zValue = digitalRead(zButton);
}
void sendMQTTData(const char* topic, int value) {
  char msg[50];
  snprintf(msg, 50, "%d", value);
  client.publish(topic, msg);
}


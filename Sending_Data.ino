#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Ahmed-home@GIGABIT FIBER";
const char* password = "AoA.0187731709";

const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char* clientId = "ESP32";

const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

#define xAxis 34
#define yAxis 35
#define zButton 23
int xValue, yValue, zValue;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  mpu6050_and_joystick_initialization();
  wifi_and_server_connection();

}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  readMPU6050Data();
  readJOYSTICKData();

  send_raw_data_to_server();
}

void mpu6050_and_joystick_initialization()
{
  Wire.begin(21, 22);  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);

  pinMode(zButton, INPUT);
}

void readMPU6050Data()
{
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

void readJOYSTICKData()
{
  xValue = analogRead(xAxis);
  yValue = analogRead(yAxis); 
  zValue = digitalRead(zButton);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void sendMQTTData(const char* topic, int value) {
  char msg[50];
  snprintf(msg, 50, "%d", value);
  client.publish(topic, msg);
}

void wifi_and_server_connection()
{
    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_broker, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void send_raw_data_to_server()
{
  sendMQTTData("joystick/x", xValue);
  sendMQTTData("joystick/y", yValue);
  sendMQTTData("joystick/button", zValue);
  sendMQTTData("mpu6050/AcX", AcX);
  sendMQTTData("mpu6050/AcY", AcY);
  sendMQTTData("mpu6050/AcZ", AcZ);
  sendMQTTData("mpu6050/GyX", GyX);
  sendMQTTData("mpu6050/GyY", GyY);
  sendMQTTData("mpu6050/GyZ", GyZ);

  delay(5000); 
}

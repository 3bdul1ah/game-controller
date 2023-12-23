#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
    int x;
    int y;
} struct_message;

struct_message myData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Joystick X: ");
    Serial.print(myData.x);
    Serial.print(", Y: ");
    Serial.println(myData.y);
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}

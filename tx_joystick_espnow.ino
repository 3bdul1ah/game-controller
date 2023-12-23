#include <WiFi.h>
#include <esp_now.h>

const int joyXPin = 34; 
const int joyYPin = 35; 

typedef struct struct_message {
    int x;
    int y;
} struct_message;

struct_message myData;

uint8_t broadcastAddress[] = {0x7C, 0x87, 0xCE, 0x2D, 0x97, 0x80};

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Last Packet Send Status: ");
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("Delivery Success");
    }
    else {
        Serial.println("Delivery Fail");
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    Serial.println("Initializing ESP-NOW");
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    } else {
        Serial.println("ESP-NOW Initialized");
    }

    esp_now_register_send_cb(OnDataSent);

    pinMode(joyXPin, INPUT);
    pinMode(joyYPin, INPUT);

    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0; 
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
    } else {
        Serial.println("Peer added");
    }
}

void loop() {
    myData.x = analogRead(joyXPin);
    myData.y = analogRead(joyYPin);
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    delay(200); 
}

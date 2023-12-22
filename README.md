# Latency test 
From: https://github.com/hui6075/mqtt-bm-latency.git
The `mqtt-bm-latency` tool you are using, which is designed for measuring MQTT broker latency
it primarily focuses on measuring the time it takes for MQTT messages to be forwarded by an MQTT broker.

However, if you want to measure the overall end-to-end latency, including both the ESP32 code and the MQTT broker, you can use `mqtt-bm-latency` to measure the latency of the entire system. In this case, you would need to timestamp the messages in your ESP32 code as in `latency_test.ino` so that we can calculate the end-to-end latency when the subscriber receives the messages.

To summarize:

`mqtt-bm-latency` measures MQTT broker latency.
To measure the latency of your ESP32 code and the MQTT broker combined, you need to implement timestamping and latency calculation in your ESP32 code and use `mqtt-bm-latency` to measure the entire system's latency.

### Overall Latency = Latency Introduced by ESP32 Code + Latency Introduced by MQTT Broker

Command Line:
- `go install github.com/hui6075/mqtt-bm-latency@latest`
- `ls $(go env GOPATH)/bin`

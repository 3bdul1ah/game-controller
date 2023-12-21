#include <Wire.h>

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22, 100000); // Initialize I2C communication using SDA and SCL pins (21, 22)

  // Initialize MPU-6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Set to zero to wake up the MPU-6050
  Wire.endTransmission(true);

  Serial.println("Setup complete");
}

void readMPU6050Data() {
  // Request data starting from register 0x3B (ACCEL_XOUT_H)
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

void loop() {
  readMPU6050Data();

  Serial.print(","); Serial.print(AcX); 
  Serial.print(","); Serial.print(AcY);
  Serial.print(","); Serial.print(AcZ);
  Serial.print(","); Serial.print(GyX);
  Serial.print(","); Serial.print(GyY);
  Serial.print(","); Serial.println(GyZ);

  delay(500);
}

#include <Wire.h> // For I2C communication
#include <MPU6050_tockn.h> // For interacting with the MPU-6050 sensor

MPU6050 mpu6050(Wire);

float FALL_THRESHOLD = 50; 
float ROTATION_THRESHOLD = 50; 

void setup() {
   Serial.begin(115200);       // Initialize serial communication
   Wire.begin(21, 22);       // Initialize I2C with SDA on GPIO21, SCL on GPIO22
   mpu6050.begin();          // Initialize the MPU6050 sensor
   mpu6050.calcGyroOffsets(true); // Calibrate (optional, but recommended)
}

void loop() {
   mpu6050.update(); 

   // Get accelerometer and gyroscope values
   float accX = mpu6050.getAccX();
   float accY = mpu6050.getAccY();
   float accZ = mpu6050.getAccZ();
   float gyroX = mpu6050.getGyroX();
   float gyroY = mpu6050.getGyroY();
   float gyroZ = mpu6050.getGyroZ();

   // Very simple fall detection logic (needs refinement!)
   float magnitude = sqrt(accX*accX + accY*accY + accZ*accZ);
   Serial.println(magnitude);
   delay(1000);
   if (magnitude > FALL_THRESHOLD && abs(gyroX) > ROTATION_THRESHOLD) { 
        Serial.print(magnitude);
        delay(1000);
        Serial.println("FALL DETECTED!");
       // Add actions here: send an alert, trigger an alarm, etc.
   }

   delay(100); // Adjust delay as needed
}

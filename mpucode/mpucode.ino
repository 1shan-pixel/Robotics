#include <Wire.h> 
#include <MPU6050_tockn.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Constants (make them easier to adjust)
const float FALL_THRESHOLD = 1.0; 
const float ROTATION_THRESHOLD = 1.0; 
const char* WIFI_SSID = "utsavghising_fbnpa_2.4";
const char* WIFI_PASSWORD = "CLB264E64A";

// Global objects
MPU6050 mpu6050(Wire);
HTTPClient http;

void setup() {
  Serial.begin(115200);

  Serial.println("Starting setup...");

  // Initialize I2C communication
  Wire.begin(21, 22); 
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Attempt Wi-Fi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
}

void loop() {
  mpu6050.update();

  // Sensor readings
  float accX = mpu6050.getAccX();
  float accY = mpu6050.getAccY();
  float accZ = mpu6050.getAccZ();
  float gyroX = mpu6050.getGyroX();
  float gyroY = mpu6050.getGyroY();
  float gyroZ = mpu6050.getGyroZ();

  // Calculate magnitude
  float magnitude = sqrt(accX*accX + accY*accY + accZ*accZ);
  unsigned long timestamp = millis();

  // Print the data with labels and formatting
  Serial.print("Timestamp: "); 
  Serial.println(timestamp);
  Serial.println("Acceleration (X, Y, Z): ");
  Serial.println(accX);
  Serial.println(accY);
  Serial.println(accZ);
  
  // Serial.println("Gyroscope (X, Y, Z): ");
  // Serial.println(gyroX);
  // Serial.println(gyroY);
  // Serial.println(gyroZ);

  delay(500);
  // Fall detection
  if (magnitude > FALL_THRESHOLD && abs(gyroX) > ROTATION_THRESHOLD) { 
    Serial.print(magnitude);
    Serial.println(" FALL DETECTED!");
    // Add actions here: send an alert, trigger an alarm, etc.
  }

  // Send data to server if Wi-Fi is connected
  //sendFallDatatoServer(magnitude); 
}

void sendFallDatatoServer(float magnitude) {
  if (WiFi.status() == WL_CONNECTED) {
    String payload = "{ \"magnitude\": " + String(magnitude) + "}"; 

    http.begin("http://192.168.1.236:5000/data");
    http.addHeader("Content-Type", "application/json"); 

    int httpResponseCode = http.POST(payload); 
    if (httpResponseCode == 200 ) {
      Serial.println("Data sent succesfully");
      delay(10000);
    } else {
      delay(2000);
    }

    http.end(); 
  } else {
    Serial.println("Error: Cannot send data without a WiFi connection.");
  }
}

// tftt or fttf

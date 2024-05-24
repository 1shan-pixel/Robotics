#include <Wire.h> 
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFiManager.h> 

// Constants (make them easier to adjust)



float values[6];
// Global objects
Adafruit_MPU6050 mpu;
HTTPClient http;

void setup() {

  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  
  Serial.println("MPU6050 test!");
  WiFiManager wm; 
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("AutoConnectAP","password"); 
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  



  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  
  while(!res){
  Serial.println("Connecting to WiFi...");
  Serial.print(".");

  }
  
 
 
  Serial.println("Connected to WiFi");
  


 

 
}

void loop() {
  String payload; // Declare payload here with wider scope

  if(mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println(" ");

    values[0] = a.acceleration.x;
    values[1] = a.acceleration.y;
    values[2] = a.acceleration.z;
    values[3] = g.gyro.x;
    values[4] = g.gyro.y;
    values[5] = g.gyro.z;

    payload = "{";
    payload += "\"acc_x\": " + String(values[0]) + ",";
    payload += "\"acc_y\": " + String(values[1]) + ",";
    payload += "\"acc_z\": " + String(values[2]) + ",";
    payload += "\"gyro_x\": " + String(values[3]) + ",";
    payload += "\"gyro_y\": " + String(values[4]) + ",";
    payload += "\"gyro_z\": " + String(values[5]); 
    payload += "}";
    Serial.println(payload);
    sendFallDatatoServer(payload); 

  }

  // Sensor readings
 
  
  

  
}

void sendFallDatatoServer(String payload) {
  if (WiFi.status() == WL_CONNECTED) {

    http.begin("http://192.168.1.70:5000/data");
    http.addHeader("Content-Type", "application/json"); 

    int httpResponseCode = http.POST(payload); 
    if (httpResponseCode == 200 ) {
      Serial.println("Data sent succesfully");
      
    } else {
      Serial.println(httpResponseCode);
      Serial.println("no data sent");
  
    }

    http.end(); 
  } else {
    Serial.println("Error: Cannot send data without a WiFi connection.");
  }
}


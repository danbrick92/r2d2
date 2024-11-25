//// SPDX-FileCopyrightText: 2023 Carter Nelson for Adafruit Industries
////
//// SPDX-License-Identifier: MIT
//// --------------------------------------
//// i2c_scanner
////
//// Modified from https://playground.arduino.cc/Main/I2cScanner/
//// --------------------------------------
//
//#include <Wire.h>
//
//// Set I2C bus to use: Wire, Wire1, etc.
//#define WIRE Wire
//
//void setup() {
//  WIRE.begin();
//
//  Serial.begin(9600);
//  while (!Serial)
//     delay(10);
//  Serial.println("\nI2C Scanner");
//}
//
//
//void loop() {
//  byte error, address;
//  int nDevices;
//
//  Serial.println("Scanning...");
//
//  nDevices = 0;
//  for(address = 1; address < 127; address++ )
//  {
//    // The i2c_scanner uses the return value of
//    // the Write.endTransmisstion to see if
//    // a device did acknowledge to the address.
//    WIRE.beginTransmission(address);
//    error = WIRE.endTransmission();
//
//    if (error == 0)
//    {
//      Serial.print("I2C device found at address 0x");
//      if (address<16)
//        Serial.print("0");
//      Serial.print(address,HEX);
//      Serial.println("  !");
//
//      nDevices++;
//    }
//    else if (error==4)
//    {
//      Serial.print("Unknown error at address 0x");
//      if (address<16)
//        Serial.print("0");
//      Serial.println(address,HEX);
//    }
//  }
//  if (nDevices == 0)
//    Serial.println("No I2C devices found\n");
//  else
//    Serial.println("done\n");
//
//  delay(5000);           // wait 5 seconds for next scan
//}

//#include "Wire.h"
//#include "TFLI2C.h"
//
//TFLI2C sensor;
//
//void setup() {
//  Serial.begin(9600);
//  Wire.begin();
//  Serial.println("TF-Luna I2C Mode: Distance and Signal Strength");
//}
//
//void loop() {
//  int16_t dist;       // Variable to store distance
//  int16_t flux;       // Variable to store signal strength
//  int16_t temp;       // Variable to store temperature (optional)
//
//  // Retrieve distance, signal strength, and temperature
//  if (sensor.getData(dist, flux, temp, 0x10)) {
//    Serial.print("Distance: ");
//    Serial.print(dist);
//    Serial.print(" cm, Signal Strength: ");
//    Serial.print(flux);
//    Serial.print(", Temperature: ");
//    Serial.print(temp / 100.0); // Convert to degrees Celsius
//    Serial.println(" °C");
//  } else {
//    Serial.print("Error: ");
//    sensor.printStatus(); // Print error status
//  }
//
//  delay(100); // Adjust delay as needed
//}

//#include <Wire.h>
//
//int MOTOR_1_PIN_1 = 2;
//int MOTOR_1_PIN_2 = 3;
//int MOTOR_1_PIN_3 = 9;
//
//int MOTOR_2_PIN_1 = 4;
//int MOTOR_2_PIN_2 = 5;
//int MOTOR_2_PIN_3 = 10;
//
//
//void setup() {
//   // Setup Pins
//   pinMode(MOTOR_1_PIN_1, OUTPUT);
//   pinMode(MOTOR_1_PIN_2, OUTPUT);
//   pinMode(MOTOR_1_PIN_3, OUTPUT);
//   pinMode(MOTOR_2_PIN_1, OUTPUT);
//   pinMode(MOTOR_2_PIN_2, OUTPUT);
//   pinMode(MOTOR_2_PIN_3, OUTPUT);
//
//   Wire.begin(); // Initialize I2C
//   Serial.begin(9600);
//   Serial.println("I2C Initialized");
//}
//
// void loop() {
////   // Set motor speeds
//   analogWrite(MOTOR_1_PIN_3, 200);
//   analogWrite(MOTOR_2_PIN_3, 200);
//  
////   // Motor 1
//   digitalWrite(MOTOR_1_PIN_1, HIGH);
//   digitalWrite(MOTOR_1_PIN_2, LOW);
//
////    // Motor 2
//   digitalWrite(MOTOR_2_PIN_1, HIGH);
//   digitalWrite(MOTOR_2_PIN_2, LOW);
//
//   delay(1000);
//
////   // Motor 1
//   digitalWrite(MOTOR_1_PIN_1, LOW);
//   digitalWrite(MOTOR_1_PIN_2, HIGH);
//
//    // Motor 2
//   digitalWrite(MOTOR_2_PIN_1, LOW);
//   digitalWrite(MOTOR_2_PIN_2, HIGH);
//  delay(1000);
//}
//

#include <Wire.h>

#define HW123_ADDRESS 0x68 // Default I2C address for the HW123/MPU6050

void setup() {
  Wire.begin();
  Serial.begin(9600);
  initHW123();
}

void loop() {
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;

  readHW123(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);

  // Print accelerometer values
  Serial.print("Accel X: "); Serial.print(accelX);
  Serial.print(" | Accel Y: "); Serial.print(accelY);
  Serial.print(" | Accel Z: "); Serial.println(accelZ);

  // Print gyroscope values
  Serial.print("Gyro X: "); Serial.print(gyroX);
  Serial.print(" | Gyro Y: "); Serial.print(gyroY);
  Serial.print(" | Gyro Z: "); Serial.println(gyroZ);

  delay(500); // Adjust delay as needed
}

// Initialize the HW123 Sensor
void initHW123() {
  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Wake up the sensor
  Wire.endTransmission();

  Serial.println("HW123 Initialized");
}

// Read Accelerometer and Gyroscope Data
void readHW123(int16_t &accelX, int16_t &accelY, int16_t &accelZ,
               int16_t &gyroX, int16_t &gyroY, int16_t &gyroZ) {
  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x3B); // Starting register for accelerometer data
  Wire.endTransmission(false); // Keep I2C connection open

  Wire.requestFrom(HW123_ADDRESS, 14); // Request 14 bytes: Accel(6), Temp(2), Gyro(6)

  if (Wire.available() == 14) {
    // Read accelerometer data
    accelX = (Wire.read() << 8) | Wire.read();
    accelY = (Wire.read() << 8) | Wire.read();
    accelZ = (Wire.read() << 8) | Wire.read();

    // Skip temperature data (2 bytes)
    Wire.read();
    Wire.read();

    // Read gyroscope data
    gyroX = (Wire.read() << 8) | Wire.read();
    gyroY = (Wire.read() << 8) | Wire.read();
    gyroZ = (Wire.read() << 8) | Wire.read();
  }
}

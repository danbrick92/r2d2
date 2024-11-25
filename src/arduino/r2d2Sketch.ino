/*
  This sketch performs the following functions
  - Reads LIDAR data from the TF Luna sensor
  - Reads Accelerometer & Gyro data from HW-123
  - Allows for control of wheels (motors)

  The code is setup mininally, to allow the Pi full control
  This includes calibration

  Author: Dan Brickner
*/

// TODO: Add motor logic
// TODO: Add debug print function
// TODO: Add serial write and read function
  // Recall: (device,event,"arg0=...,argn=...")

#include "Wire.h"
#include "TFLI2C.h"


const bool SHOULD_PRINT = true;
const int SERIAL_BAUD = 9600;
const byte TFLUNA_ADDRESS = 0x10;
TFLI2C tfLunaSensor;
const byte HW123_ADDRESS = 0x68;


struct tfLuna {
  int16_t distance;
  int16_t strength;
  int16_t temperature;
  bool success;
};
struct hw123 {
  int16_t accelX;
  int16_t accelY;
  int16_t accelZ;
  int16_t gyroX;
  int16_t gyroY;
  int16_t gyroZ;
  int16_t temp;
};


void setup(){
  Serial.begin(SERIAL_BAUD);
  Wire.begin();
  initHW123();
}

void loop(){
  struct tfLuna tfLunaData = getTFLunaData();
  struct hw123 hw123Data = getHW123Data();
  delay(1000); // Adjust delay as needed
}

//----------------------------------
// TF Luna
//----------------------------------
tfLuna getTFLunaData(){
  // Initiate variables
  int16_t distance;
  int16_t strength;
  int16_t temperature; 
  bool success;

  // Get data from sensor
  success = tfLunaSensor.getData(distance, strength, temperature, TFLUNA_ADDRESS);
  if (success) {
    // Print if needed
    if (SHOULD_PRINT){
      Serial.print("(tfluna,debug,\"distance=");
      Serial.print(distance);
      Serial.print(",strength=");
      Serial.print(strength);
      Serial.print(",temperature=");
      Serial.print(temperature);
      Serial.println("\")");
    }
  }
  else {
    Serial.print("(tfluna,error,\"error=");
    tfLunaSensor.printStatus(); // Print error status)
    Serial.println("\")");
  }

  // Return struct
  struct tfLuna data = {distance, strength, temperature, success};
  return data;
}

//----------------------------------
// HW-123 (Accel/Gyro)
//----------------------------------
void initHW123() {
  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Wake up the sensor
  Wire.endTransmission();
  Serial.println("(hw123,state,state=\"initialized\")");
}

hw123 getHW123Data(){
  int16_t accelX;
  int16_t accelY;
  int16_t accelZ;
  int16_t temp;
  int16_t gyroX;
  int16_t gyroY;
  int16_t gyroZ;

  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x3B); // Starting register for accelerometer data
  Wire.endTransmission(false); // Keep I2C connection open
  Wire.requestFrom(HW123_ADDRESS, 14); // Request 14 bytes: Accel(6), Temp(2), Gyro(6)

  if (Wire.available() == 14) {
    // Read accelerometer data
    accelX = (Wire.read() << 8) | Wire.read();
    accelY = (Wire.read() << 8) | Wire.read();
    accelZ = (Wire.read() << 8) | Wire.read();

    // Read temperature data
    temp = (Wire.read() << 8 | Wire.read()); // Temp High, Low

    // Read gyroscope data
    gyroX = (Wire.read() << 8) | Wire.read();
    gyroY = (Wire.read() << 8) | Wire.read();
    gyroZ = (Wire.read() << 8) | Wire.read();
  }

  // Print if needed
  if (SHOULD_PRINT){
      Serial.print("(hw123,debug,\"accelX=");
      Serial.print(accelX);
      Serial.print(",accelY=");
      Serial.print(accelY);
      Serial.print(",accelZ=");
      Serial.print(accelZ);
      Serial.print(",gyroX=");
      Serial.print(gyroX);
      Serial.print(",gyroY=");
      Serial.print(gyroY);
      Serial.print(",gyroZ=");
      Serial.print(gyroZ);
      Serial.print(",temp=");
      Serial.print(temp);
      Serial.println("\")");
  }

  // Generate struct
  struct hw123 data = {
    accelX, accelY, accelZ,
    gyroX, gyroY, gyroZ,
    temp
  };

  return data;
}

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


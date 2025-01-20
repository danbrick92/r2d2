/*
  This sketch performs the following functions
  - Reads LIDAR data from the TF Luna sensor
  - Reads Accelerometer & Gyro data from HW-123
  - Allows for control of wheels (motors)

  The code is setup minimally, to allow the Pi full control
  This includes calibration

  Author: Dan Brickner
*/
// (motors,set,magnitude=200,direction=96)
#include "Wire.h"
#include "TFLI2C.h"

// Common Settings
const bool SHOULD_PRINT = true;
const int SERIAL_BAUD = 9600;

// TFLuna Settings
const byte TFLUNA_ADDRESS = 0x10;
TFLI2C tfLunaSensor;

// HW123 Settings
const byte HW123_ADDRESS = 0x68;

// Motor Settings
const int MOTOR_LEFT_FORW_PIN = 5;
const int MOTOR_LEFT_BACK_PIN = 4;
const int MOTOR_LEFT_SPEED_PIN = 10;
const int MOTOR_RIGHT_FORW_PIN = 3;
const int MOTOR_RIGHT_BACK_PIN = 2;
const int MOTOR_RIGHT_SPEED_PIN = 9;
int MOTOR_MIN_SPEED_LEFT = 0;
int MOTOR_MIN_SPEED_RIGHT = 0;


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
struct motorState {
  bool leftForward;
  bool leftBackward;
  int leftSpeed;
  bool rightForward;
  bool rightBackward;
  int rightSpeed;
};
struct motorState CURRENT_MOTOR_STATE = {false, false, 0, false, false, 0};

//----------------------------------
// Common
//----------------------------------
void setup(){
  Serial.begin(SERIAL_BAUD);
  Wire.begin();
  initHW123();
  initMotors();
}

void loop(){
  struct motorState newMotorStateData = readMotorState();
  struct tfLuna tfLunaData = getTFLunaData();
  struct hw123 hw123Data = getHW123Data();
  struct motorState motorStateData = setMotors(newMotorStateData);
  CURRENT_MOTOR_STATE = motorStateData;
  writeSerialLine("motors", "read", "leftForward=" + String(motorStateData.leftForward) + ",leftBackward=" + String(motorStateData.leftBackward) + ",leftSpeed=" + String(motorStateData.leftSpeed) + ",rightForward=" + String(motorStateData.rightForward) + ",rightBackward=" + String(motorStateData.rightBackward) + ",rightSpeed=" + String(motorStateData.rightSpeed));
  delay(1000); // Adjust delay as needed
}

void writeSerialLine(String device, String event, String data){
  Serial.print("(");
  Serial.print(device);
  Serial.print(",");
  Serial.print(event);
  Serial.print(",\"");
  Serial.print(data);
  Serial.println("\")");
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
    writeSerialLine("tfLuna", "read", "distance=" + String(distance) + ",strength=" + String(strength) + ",temp=" + String(temperature));
  }
  else {
    // Have to do custom print due to API
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
  writeSerialLine("hw123", "state", "state=initialized");
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
  Wire.requestFrom((uint8_t)HW123_ADDRESS, (uint8_t)14); // Request 14 bytes: Accel(6), Temp(2), Gyro(6)

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

  // Print
  writeSerialLine("hw123", "read", "accelX=" + String(accelX) + ",accelY=" + String(accelY) + ",accelZ=" + String(accelZ) + ",gyroX=" + String(gyroX) + ",gyroY=" + String(gyroY) + ",gyroZ=" + String(gyroZ) + ",temp=" + String(temp));

  // Generate struct
  struct hw123 data = {
    accelX, accelY, accelZ,
    gyroX, gyroY, gyroZ,
    temp
  };

  return data;
}

//----------------------------------
// Motors
//----------------------------------
void initMotors() {
  pinMode(MOTOR_LEFT_FORW_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BACK_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_FORW_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BACK_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_SPEED_PIN, OUTPUT);
}

struct motorState setMotors(struct motorState m) {
  // Store end motor state
  struct motorState updatedState;

  // Set motor speeds
  analogWrite(MOTOR_LEFT_SPEED_PIN, m.leftSpeed);
  updatedState.leftSpeed = m.leftSpeed;
  analogWrite(MOTOR_RIGHT_SPEED_PIN, m.rightSpeed);
  updatedState.rightSpeed = m.rightSpeed;
 
  // Left Motor
  if (m.leftForward){
    digitalWrite(MOTOR_LEFT_FORW_PIN, HIGH);
    digitalWrite(MOTOR_LEFT_BACK_PIN, LOW);
    updatedState.leftForward = true;
    updatedState.leftBackward = false;
  }
  else if (m.leftBackward) {
    digitalWrite(MOTOR_LEFT_FORW_PIN, LOW);
    digitalWrite(MOTOR_LEFT_BACK_PIN, HIGH);
    updatedState.leftForward = false;
    updatedState.leftBackward = true;
  }
  else {
    digitalWrite(MOTOR_LEFT_FORW_PIN, LOW);
    digitalWrite(MOTOR_LEFT_BACK_PIN, LOW);
    updatedState.leftForward = false;
    updatedState.leftBackward = false;
  }

  // Right Motor
  if (m.rightForward){
    digitalWrite(MOTOR_RIGHT_FORW_PIN, HIGH);
    digitalWrite(MOTOR_RIGHT_BACK_PIN, LOW);
    updatedState.rightForward = true;
    updatedState.rightBackward = false;
  }
  else if (m.rightBackward) {
    digitalWrite(MOTOR_RIGHT_FORW_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_BACK_PIN, HIGH);
    updatedState.rightForward = false;
    updatedState.rightBackward = true;
  }
  else {
    digitalWrite(MOTOR_RIGHT_FORW_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_BACK_PIN, LOW);
    updatedState.rightForward = false;
    updatedState.rightBackward = false;
  }

  return updatedState;
}

struct motorState convertMagDirToMotorState(int magnitude, int direction) {
    struct motorState data = CURRENT_MOTOR_STATE;

    // Cap magnitude
    if (magnitude > 255) {
        magnitude = 255;
    } else if (magnitude < -255) {
        magnitude = -255;
    }

    // Normalize direction to [0, 360)
    direction = direction % 360;
    if (direction < 0) {
        direction += 360;
    }

    // Motor speed variables
    float leftMotorSpeed = 0, rightMotorSpeed = 0;

    if (direction >= 0 && direction <= 90) {
        // Quadrant 1: Forward motion (0-90 degrees)
        float ratio = (90.0 - direction) / 90.0; // Ratio decreases from 1 to 0
        leftMotorSpeed = (float)magnitude;
        rightMotorSpeed = (float)magnitude * ratio;
    }
    else if (direction > 90 && direction <= 180){
      float ratio = 1.0 - ((180.0 - direction) / 90.0);
      leftMotorSpeed = -(float)magnitude;
      rightMotorSpeed = -(float)magnitude * ratio;
    }
    else if (direction > 180 && direction < 270){
      float ratio = (270.0 - direction) / 90.0;
      leftMotorSpeed = -(float)magnitude * ratio;
      rightMotorSpeed = -(float)magnitude;
    }
    else {
        float ratio = 1.0 - ((360.0 - direction) / 90.0); // Ratio decreases from 1 to 0
        leftMotorSpeed = (float)magnitude * ratio;
        rightMotorSpeed = (float)magnitude;
    }

    // Populate motorState structure
    data.leftForward = (leftMotorSpeed > 0.0);
    data.leftBackward = (leftMotorSpeed < 0.0);
    data.leftSpeed = abs((int)leftMotorSpeed);

    data.rightForward = (rightMotorSpeed > 0.0);
    data.rightBackward = (rightMotorSpeed < 0.0);
    data.rightSpeed = abs((int)rightMotorSpeed);

    return data;
}



struct motorState readMotorState() {
  // Start with motorState
  struct motorState motorStateData = CURRENT_MOTOR_STATE;

  // Read data from console
  if (Serial.available() > 0) {
    String data = Serial.readString(); // Read full data
    data.trim(); // Remove any extra spaces or newlines

    String memory = "";
    int magnitude = 0;
    int direction = 0;
    int currentVariable = 0;

    // Parse input string
    for (int i = 0; i < data.length(); i++) {
      char currentChar = data[i];

      // Handle start of data
      if (currentChar == '(') {
        memory = ""; // Reset memory for new input
        currentVariable = 0;
        continue;
      }

      // Handle end of key-value pair
      if (currentChar == ',' || currentChar == ')') {
        // Process memory content
        if (currentVariable == 0 && memory == "motors") {
          // Check "device" variable
          currentVariable++;
        } else if (currentVariable == 1 && memory == "set") {
          // Check "event" variable
          currentVariable++;
        } else if (currentVariable == 2 && memory.startsWith("magnitude=")) {
          magnitude = memory.substring(10).toInt(); // Extract magnitude
          currentVariable++;
        } else if (currentVariable == 3 && memory.startsWith("direction=")) {
          direction = memory.substring(10).toInt(); // Extract direction
          motorStateData = convertMagDirToMotorState(magnitude, direction);
        }
        memory = ""; // Reset memory for next key-value pair
        continue;
      }

      // Append character to memory
      memory += currentChar;
    }
  }

  return motorStateData;
}

/*
  This sketch performs the following functions
  - Reads LIDAR data from the TF Luna sensor
  - Reads Accelerometer & Gyro data from HW-123
  - Allows for control of wheels (motors)

  The code is setup minimally, to allow the Pi full control
  This includes calibration

  Author: Dan Brickner
*/
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
const int MOTOR_LEFT_FORW_PIN = 2;
const int MOTOR_LEFT_BACK_PIN = 3;
const int MOTOR_LEFT_SPEED_PIN = 9;
const int MOTOR_RIGHT_FORW_PIN = 4;
const int MOTOR_RIGHT_BACK_PIN = 5;
const int MOTOR_RIGHT_SPEED_PIN = 10;
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
  delay(1000); // Adjust delay as needed
}

void writeSerialLine(String device, String event, String data){
  Serial.write("(");
  Serial.write(device);
  Serial.write(",");
  Serial.write(event);
  Serial.write(",\"");
  Serial.write(data);
  Serial.writeLn("\")");
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
    writeSerialLine("tfLuna", "read", "distance=" + distance + ",strength=" + strength + ",temp=" + temperature);
    // if (SHOULD_PRINT){
    //   Serial.print("(tfluna,debug,\"distance=");
    //   Serial.print(distance);
    //   Serial.print(",strength=");
    //   Serial.print(strength);
    //   Serial.print(",temperature=");
    //   Serial.print(temperature);
    //   Serial.println("\")");
    // }
  }
  else {
    writeSerialLine("tfLuna", "error", "error=" + tfLunaSensor.printStatus());

    // Serial.print("(tfluna,error,\"error=");
    // tfLunaSensor.printStatus(); // Print error status)
    // Serial.println("\")");
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

struct motorState convertMagDirToMotorState(int magnitude, int direction){
  struct motorState data = CURRENT_MOTOR_STATE;

  // Cap magnitude
  if (magnitude > 255){
    magnitude = 255;
  }
  else if (magnitude < -255){
    magnitude = -255;
  }

  // Cap direction
  direction = direction % 360;
  if (direction < 0){
    direction += 360;
  }

  // Determine motor control based on direction and magnitude
  int leftMotorSpeed, rightMotorSpeed;

  if (direction >= 0 && direction <= 90) {
    // Forward right quadrant
    float ratio = (float)direction / 90.0;
    rightMotorSpeed = magnitude;                  // Full speed on the right
    leftMotorSpeed = magnitude * (1.0 - ratio);  // Reduce speed on the left
  } else if (direction > 90 && direction <= 180) {
    // Forward left quadrant
    float ratio = (float)(direction - 90) / 90.0;
    leftMotorSpeed = magnitude;                   // Full speed on the left
    rightMotorSpeed = magnitude * (1.0 - ratio);  // Reduce speed on the right
  } else if (direction > 180 && direction <= 270) {
    // Backward left quadrant
    float ratio = (float)(direction - 180) / 90.0;
    leftMotorSpeed = -magnitude;                   // Full reverse on the left
    rightMotorSpeed = -magnitude * (1.0 - ratio);  // Reduce reverse speed on the right
  } else {
    // Backward right quadrant
    float ratio = (float)(direction - 270) / 90.0;
    rightMotorSpeed = -magnitude;                  // Full reverse on the right
    leftMotorSpeed = -magnitude * (1.0 - ratio);   // Reduce reverse speed on the left
  }

  // Populate motorState structure
  data.leftForward = (leftMotorSpeed > 0);
  data.leftBackward = (leftMotorSpeed < 0);
  data.leftSpeed = abs(leftMotorSpeed);

  data.rightForward = (rightMotorSpeed > 0);
  data.rightBackward = (rightMotorSpeed < 0);
  data.rightSpeed = abs(rightMotorSpeed);

  return data;
}

struct motorState readMotorState(){
  // Start with motorState
  struct motorState motorStateData = CURRENT_MOTOR_STATE;

  // Read data from console
  String data = "";
  if (Serial.available() > 0){
    data = Serial.readString();
  }
  else {
    return motorStateData;
  }

  // Loop through data
  String memory = "";
  int magnitude = 0;
  int direction = 0;
  String[] variables = ["device", "event", "magnitude", "direction"];
  int currentVariable = 0;
  bool finishedData = false;
  bool shouldBreak = true;

  for (int i = 0; i < data.length(); i++){
    // Check for important variables
    if (data[i] == "("){
      memory = ""
      currentVariable = 0;
      shouldBreak = false;
      magnitude = 0;
      direction = 0;
      continue; 
    }
    else if (data[i] == "," && shouldBreak == false){
      // Verify meets criteria
      if (variables[currentVariable] == "device"){
        if (memory != "motors"){
          shouldBreak = true;
        }
      }
      else if (variables[currentVariable] == "event"){
        if (memory != "set"){
          shouldBreak = true;
        }
      }
      else if (variables[currentVariable] == "magnitude"){
        shouldBreak = true;
        if (memory.startsWith("magnitude=")){
          magnitude = int(memory.substring(10));
          shouldBreak = false;
        }
      }
      else if (variables[currentVariable] == "direction"){
        shouldBreak = true;
        if (memory.startsWith("direction=")){
          direction = int(memory.substring(10));
          shouldBreak = false;
          motorStateData = convertMagDirToMotorState(magnitude, direction);
        }
      }
      continue; 
    }
    else if (data[i] == "\")"){
      continue; 
    }
    else if (data[i] == "\""){
      // dont add to memory
      continue;
    }

    memory += data[i]; 
  }

  // Decide final values to return
  return motorStateData;
}

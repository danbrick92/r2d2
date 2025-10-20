/*
  This sketch performs the following functions
  - Reads LIDAR data from the TF Luna sensor
  - Reads Accelerometer & Gyro data from HW-123
  - Read and Write multi-color leds
  - Allows for control of wheels (motors)
  - Control stepper motors

  The code is setup minimally, to allow the Pi full control
  This includes calibration

  **NOTE: Make sure to install TFLI2C.h library **

  Author: Dan Brickner
*/
// (motors,set,magnitude=200,direction=96)

#include <Stepper.h>
#include "Wire.h"
#include "TFLI2C.h"

// Common Settings
const bool SHOULD_PRINT = true;
// const int SERIAL_BAUD = 9600;
const int SERIAL_BAUD = 115200;

// Front LED Settings
const int FRONT_LED_R_PIN = 3;
const int FRONT_LED_G_PIN = 5;
const int FRONT_LED_B_PIN = 6;

// Back LED Settings
const int BACK_LED_R_PIN = 11;
const int BACK_LED_G_PIN = 12;
const int BACK_LED_B_PIN = 13;

// TFLuna Settings
const byte TFLUNA_ADDRESS = 0x10;
TFLI2C tfLunaSensor;

// HW123 Settings
const byte HW123_ADDRESS = 0x68;

// Motor Settings
const int MOTOR_LEFT_FORW_PIN = 4;
const int MOTOR_LEFT_BACK_PIN = 2;
const int MOTOR_LEFT_SPEED_PIN = 10;
const int MOTOR_RIGHT_FORW_PIN = 8;
const int MOTOR_RIGHT_BACK_PIN = 7;
const int MOTOR_RIGHT_SPEED_PIN = 9;
int MOTOR_MIN_SPEED_LEFT = 0;
int MOTOR_MIN_SPEED_RIGHT = 0;

// Stepper
#define STEPS_PER_REV 2048   // 28BYJ-48 has ~2048 steps per full rotation
Stepper myStepper(STEPS_PER_REV, A0, A2, A1, A3);
// note the slightly odd order: (IN1, IN3, IN2, IN4) for correct sequence


// Data Structures
struct ledState {
  int red;
  int green;
  int blue;
};
struct tfLunaState {
  int16_t distance;
  int16_t strength;
  int16_t temperature;
  bool success;
};
struct hw123State {
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
struct stepperState {
  int rpm_speed;
  int current_step;
};


// Current States
struct ledState FRONT_LED_STATE = {0, 0, 0};
struct ledState BACK_LED_STATE = {0, 0, 0};
struct tfLunaState TFLUNA_STATE = {0, 0, 0, false};
struct hw123State HW123_STATE = {0, 0, 0, 0, 0, 0, 0};
struct motorState MOTOR_STATE = {false, false, 0, false, false, 0};
struct stepperState STEPPER_STATE = {10, 0};

//----------------------------------
// Common
//----------------------------------
void setup(){
  Serial.begin(SERIAL_BAUD);
  Wire.begin();

  initFrontLED();
  initBackLED();
  // no intialization needed for TFLuna
  inithw123();
  initMotors();
  initStepperMotor();
}

void loop() {
  // Check for incoming commands
  while (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    if (line.length() > 0) {
      routeCommand(line);
    }
    yield();
  }

  // Send sensor data
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 20) {  // every 20 ms = 50 Hz
    frontLEDRead();
    backLEDRead();
    tfLunaRead();
    hw123Read();
    motorStateRead();
    stepperMotorRead();
  }
}

void routeCommand(String line){
  // Send command to the correct function
  line.trim();

  char device = line.charAt(0);
  char event = line.charAt(1);
  String data = line.substring(2);

  if (device == 'f' && event == 'w'){  // front led write
    frontLEDWrite(data);
  }
  else if (device == 'b' && event == 'w'){  // back led write
    backLEDWrite(data);
  }
  else if (device == 'm' && event == 'w'){  // wheel motor write
    motorWrite(data);
  }
  else if (device == 's' && event == 'w'){  // head stepper motor write
    stepperMotorWrite(data);
  }
}

void writeSerialLine(String device, String event, String data){
  // Streamlined interface for writing to serial
  Serial.print(device);
  Serial.print(event);
  Serial.println(data);
}

// ---------------------------------
// Front LED
// ---------------------------------
void initFrontLED(){
  // Initialize
  pinMode(FRONT_LED_R_PIN, OUTPUT);
  pinMode(FRONT_LED_G_PIN, OUTPUT);
  pinMode(FRONT_LED_B_PIN, OUTPUT);
  struct ledState newState = {0, 0, 0};
  frontLEDSetColor(newState);
}

void frontLEDSetColor(struct ledState colors) {
  analogWrite(FRONT_LED_R_PIN, colors.red);
  analogWrite(FRONT_LED_G_PIN, colors.green);
  analogWrite(FRONT_LED_B_PIN, colors.blue);
  FRONT_LED_STATE = {colors.red, colors.green, colors.blue};
}

void frontLEDWrite(String data){
  // Parse data points
  data.trim();
  struct ledState newState = {0, 0, 0};

  // Temporary variables
  int r = 0, g = 0, b = 0;

  // Use sscanf to parse three integers separated by spaces
  if (sscanf(data.c_str(), "%d %d %d", &r, &g, &b) == 3) {
    // Clamp values to 0-255 just in case
    newState.red = constrain(r, 0, 255);
    newState.green = constrain(g, 0, 255);
    newState.blue = constrain(b, 0, 255);
    frontLEDSetColor(newState);
  } else {
    String errorMsg = "Invalid LED command: " + data;
    writeSerialLine("f", "e", errorMsg);
  }
}

void frontLEDRead(){
  writeSerialLine("f", "r", String(FRONT_LED_STATE.red) + " " + String(FRONT_LED_STATE.green) + " " + String(FRONT_LED_STATE.blue));
}

// ---------------------------------
// Back LED
// ---------------------------------
void initBackLED(){
  pinMode(BACK_LED_R_PIN, OUTPUT);
  pinMode(BACK_LED_G_PIN, OUTPUT);
  pinMode(BACK_LED_B_PIN, OUTPUT);
  struct ledState newState = {0, 0, 0};
  backLEDSetColor(newState);
}

void backLEDSetColor(struct ledState colors) {
  analogWrite(BACK_LED_R_PIN, colors.red);
  analogWrite(BACK_LED_G_PIN, colors.green);
  analogWrite(BACK_LED_B_PIN, colors.blue);
  BACK_LED_STATE = {colors.red, colors.green, colors.blue};
}

void backLEDWrite(String data){
  // Parse data points
  data.trim();
  struct ledState newState = {0, 0, 0};

  // Temporary variables
  int r = 0, g = 0, b = 0;

  // Use sscanf to parse three integers separated by spaces
  if (sscanf(data.c_str(), "%d %d %d", &r, &g, &b) == 3) {
    // Clamp values to 0-255 just in case
    newState.red = constrain(r, 0, 255);
    newState.green = constrain(g, 0, 255);
    newState.blue = constrain(b, 0, 255);

    // This LED is binary - 0 or 255
    if (newState.red < 128){
      newState.red = 0;
    }
    else {
      newState.red = 255;
    }
    if (newState.green < 128){
      newState.green = 0;
    }
    else {
      newState.green = 255;
    }
    if (newState.blue < 128){
      newState.blue = 0;
    }
    else {
      newState.blue = 255;
    }

    backLEDSetColor(newState);
  } else {
    String errorMsg = "Invalid LED command: " + data;
    writeSerialLine("f", "e", errorMsg);
  }
}

void backLEDRead(){
  writeSerialLine("b", "r", String(BACK_LED_STATE.red) + " " + String(BACK_LED_STATE.green) + " " + String(BACK_LED_STATE.blue));
}

//----------------------------------
// TF Luna
//----------------------------------
void tfLunaRead(){
  // Initiate variables
  int16_t distance;
  int16_t strength;
  int16_t temperature; 
  bool success;

  // Get data from sensor
  success = tfLunaSensor.getData(distance, strength, temperature, TFLUNA_ADDRESS);
  TFLUNA_STATE = {distance, strength, temperature, success};
  if (success) {
    writeSerialLine("t", "r", String(distance) + " " + String(strength) + " " + String(temperature));
  }
  else {
    // Have to do custom print due to API
    Serial.print("te");
    tfLunaSensor.printStatus(); // Print error status
    Serial.println("");
  }
}

//----------------------------------
// HW-123 (Accel/Gyro)
//----------------------------------
void inithw123() {
  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Wake up the sensor
  Wire.endTransmission();
}

void hw123Read(){
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

  HW123_STATE = {accelX, accelY, accelZ, gyroX, gyroY, gyroZ, temp};

  // Print
  writeSerialLine("h", "r", String(accelX) + " " + String(accelY) + " " + String(accelZ) + " " + String(gyroX) + " " + String(gyroY) + " " + String(gyroZ) + " " + String(temp));
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

void motorWrite(String data){
  data.trim();
  struct motorState newState = {false, false, 0, false, false, 0};

  // Temporary variables
  int lf = 0, lb = 0, ls = 0, rf = 0, rb = 0, rs = 0;

  // Use sscanf to parse three integers separated by spaces
  if (sscanf(data.c_str(), "%d %d %d %d %d %d", &lf, &lb, &ls, &rf, &rb, &rs) == 6) {
    // Assign and constrain
    newState.leftForward = (lf != 0);
    newState.leftBackward = (lb != 0);
    newState.leftSpeed = constrain(ls, 0, 255);
    newState.rightForward = (rf != 0);
    newState.rightBackward = (rb != 0);
    newState.rightSpeed = constrain(rs, 0, 255);

    // Ensure bad states aren't possible
    if (newState.leftForward && newState.leftBackward){
      newState.leftForward = false;
      newState.leftBackward = false;
    }
    if (newState.rightForward && newState.rightBackward){
      newState.rightForward = false;
      newState.rightBackward = false;
    }

    // Write to pins
    digitalWrite(MOTOR_LEFT_FORW_PIN, newState.leftForward);
    digitalWrite(MOTOR_LEFT_BACK_PIN, newState.leftBackward);
    analogWrite(MOTOR_LEFT_SPEED_PIN, newState.leftSpeed);
    digitalWrite(MOTOR_RIGHT_FORW_PIN, newState.rightForward);
    digitalWrite(MOTOR_RIGHT_BACK_PIN, newState.rightBackward);
    analogWrite(MOTOR_RIGHT_SPEED_PIN, newState.rightSpeed);

    // Update State
    MOTOR_STATE = newState;
  }
  else {
    String errorMsg = "Invalid Motor command: " + data;
    writeSerialLine("m", "e", errorMsg);
  }
}

void motorStateRead(){
  writeSerialLine("m", "r", String(MOTOR_STATE.leftForward) + " " + String(MOTOR_STATE.leftBackward) + " " + String(MOTOR_STATE.leftSpeed) + " " + String(MOTOR_STATE.rightForward) + " " + String(MOTOR_STATE.rightBackward) + " " + String(MOTOR_STATE.rightSpeed));
}

//----------------------------------
// Head Stepper Motor
//----------------------------------
void initStepperMotor() {
  myStepper.setSpeed(10);
}

void stepperMotorRead(){
    writeSerialLine("s", "r", String(STEPPER_STATE.rpm_speed) + " " + String(STEPPER_STATE.current_step));
}

void stepperMotorWrite(String data){
    data.trim();
    struct stepperState newState = {0, 0};

    // Temporary variables
    int speed = 0, steps = 0;

    // Use sscanf to parse three integers separated by spaces
    if (sscanf(data.c_str(), "%d %d", &speed, &steps) == 2) {
      newState.rpm_speed = constrain(speed, 0, 20);  // clamp to a safe RPM range
      newState.current_step = constrain(steps, -100, 100);

      myStepper.setSpeed(newState.rpm_speed);
      // Move the motor (blocking call)
      if (newState.current_step != 0) {
        myStepper.step(newState.current_step);
      }
    }
    else {
      String errorMsg = "Invalid Stepper command: " + data;
      writeSerialLine("s", "e", errorMsg);
    }
}

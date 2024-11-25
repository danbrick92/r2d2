// #include <Wire.h>

// int MOTOR_1_PIN_1 = 2;
// int MOTOR_1_PIN_2 = 3;
// int MOTOR_1_PIN_3 = 9;

// int MOTOR_2_PIN_1 = 4;
// int MOTOR_2_PIN_2 = 5;
// int MOTOR_2_PIN_3 = 10;


// void setup() {
//   // Setup Pins
//   pinMode(MOTOR_1_PIN_1, OUTPUT);
//   pinMode(MOTOR_1_PIN_2, OUTPUT);
//   pinMode(MOTOR_1_PIN_3, OUTPUT);
//   pinMode(MOTOR_2_PIN_1, OUTPUT);
//   pinMode(MOTOR_2_PIN_2, OUTPUT);
//   pinMode(MOTOR_2_PIN_3, OUTPUT);

//   Wire.begin(); // Initialize I2C
//   Serial.begin(9600);
//   Serial.println("I2C Initialized");
// }

// void loop() {
//   // Set motor speeds
//   analogWrite(MOTOR_1_PIN_3, 200);
//   analogWrite(MOTOR_2_PIN_3, 200);
  
//   // Motor 1
//   digitalWrite(MOTOR_1_PIN_1, HIGH);
//   digitalWrite(MOTOR_1_PIN_2, LOW);

//    // Motor 2
//   digitalWrite(MOTOR_2_PIN_1, HIGH);
//   digitalWrite(MOTOR_2_PIN_2, LOW);

//   delay(1000);

//   // Motor 1
//   digitalWrite(MOTOR_1_PIN_1, LOW);
//   digitalWrite(MOTOR_1_PIN_2, HIGH);

//    // Motor 2
//   digitalWrite(MOTOR_2_PIN_1, LOW);
//   digitalWrite(MOTOR_2_PIN_2, HIGH);
//   delay(1000);
// }

#include <Wire.h>

// I2C Addresses
#define HW123_ADDRESS 0x68 // Replace with actual HW123 I2C address
#define TFLUNA_ADDRESS 0x10 // Replace with actual TF-Luna I2C address

void setup() {
  Wire.begin(); // Initialize I2C communication
  Serial.begin(9600); // Start serial communication
  Serial.println("Setup started");

  // HW123 Accelerometer Initialization
  initializeHW123();

  // TF-Luna Initialization (if required)
  initializeTFLuna();
}

void loop() {
  // Read Accelerometer Data
  int16_t ax, ay, az;
  readHW123(&ax, &ay, &az);
  Serial.print("Accel (X, Y, Z): ");
  Serial.print(ax); Serial.print(", ");
  Serial.print(ay); Serial.print(", ");
  Serial.println(az);

  // Read Distance from TF-Luna
  int distance = readTFLunaDistance();
  Serial.print("Distance (cm): ");
  Serial.println(distance);

  delay(500); // Adjust delay as necessary
}

// HW123 Accelerometer Functions
void initializeHW123() {
  // Example: Setting up HW123 accelerometer registers
  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x2D); // Power Control Register
  Wire.write(0x08); // Enable measurement mode
  Wire.endTransmission();
}

void readHW123(int16_t *ax, int16_t *ay, int16_t *az) {
  Wire.beginTransmission(HW123_ADDRESS);
  Wire.write(0x32); // Starting register for accelerometer data
  Wire.endTransmission(false);
  Wire.requestFrom(HW123_ADDRESS, 6); // Read 6 bytes (X, Y, Z data)
  
  if (Wire.available() == 6) {
    *ax = (Wire.read() | (Wire.read() << 8));
    *ay = (Wire.read() | (Wire.read() << 8));
    *az = (Wire.read() | (Wire.read() << 8));
  }
}

// TF-Luna Functions
void initializeTFLuna() {
  // Typically, TF-Luna doesn't need initialization for basic operation
}

int readTFLunaDistance() {
  Wire.beginTransmission(TFLUNA_ADDRESS);
  Wire.write(0x00); // Request distance data
  Wire.endTransmission(false);
  Wire.requestFrom(TFLUNA_ADDRESS, 2); // Read 2 bytes (distance data)

  int distance = -1;
  if (Wire.available() == 2) {
    distance = (Wire.read() | (Wire.read() << 8)); // Combine bytes
  }
  return distance;
}

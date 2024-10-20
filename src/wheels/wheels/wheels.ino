int MOTOR_1_PIN_1 = 2;
int MOTOR_1_PIN_2 = 3;
int MOTOR_1_PIN_3 = 9;

int MOTOR_2_PIN_1 = 4;
int MOTOR_2_PIN_2 = 5;
int MOTOR_2_PIN_3 = 10;


void setup() {
  // Setup Pins
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_1_PIN_3, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_3, OUTPUT);
}

void loop() {
  // Set motor speeds
  analogWrite(MOTOR_1_PIN_3, 200);
  analogWrite(MOTOR_2_PIN_3, 200);
  
  // Motor 1
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);

   // Motor 2
  digitalWrite(MOTOR_2_PIN_1, HIGH);
  digitalWrite(MOTOR_2_PIN_2, LOW);

  delay(1000);

  // Motor 1
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, HIGH);

   // Motor 2
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, HIGH);
  delay(1000);


}

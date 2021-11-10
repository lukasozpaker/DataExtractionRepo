// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup() {
  // All mtot control pins are set to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  brake();

  directionControl();
  delay(1000);
  speedControl();
  delay(1000);
}

void loop() {
  
}

// Spinning direction of motors
void directionControl() {
  setBothAnalog();

  setReverseMotors();
  delay(2000);
  
  setBothMotors();
  delay(2000);
  
  brake();
}

// Speed of the motors
void speedControl() {
  
  setBothMotors();
  
  accel();
  decel();
  
  
  brake();
}

void setBothAnalog() {
  // motors at max speed (255)
  // 0 to 255
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}
void setBothMotors() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void setReverseMotors() {
  // Turn on motor A & B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void accel() {
  // Accelerate from 0 to 255
  for (int i = 0; i < 256; i++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }
}

void decel() {
  // Decelerate from 255 to 0
  for (int i = 255; i >= 0; --i) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }
}

void brake() {
  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
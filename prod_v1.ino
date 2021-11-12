//begin ultrasonic imports
#define echoPin 2 
#define trigPin 3 
//end ultrasonic imports

int in1 = 8;  //direction 1
int in2 = 7;  //direction 1
int ena = 9;  //power 1

int in3 = 5;  //direction 2
int in4 = 4;  //direction 2
int enb = 3;  //power 2

void setup() {
  pinMode(in1, OUTPUT);  // sets the pin as output
  pinMode(in2, OUTPUT); // sets the pin as output
  pinMode(ena, OUTPUT);

   pinMode(in3, OUTPUT);  // sets the pin as output
  pinMode(in4, OUTPUT); // sets the pin as output
  pinMode(enb, OUTPUT);


  while(true) {
    setBothMotors(235);
    delay(1000);
    pivotCW(230);
    delay(1000);
    brake();
    delay(1000);
  }



  
}



void loop() {
  
}


void setBothMotors(int SPEED) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,SPEED);

  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb,SPEED);
}

void pivotCW(int SPEED) {
  digitalWrite(in1, LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena,SPEED);

  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb,SPEED);
}

void brake() {
  digitalWrite(in1, LOW);
  digitalWrite(in2,LOW);
  analogWrite(ena,0);

  digitalWrite(in3, LOW);
  digitalWrite(in4,LOW);
  analogWrite(enb,0);
}

bool obstacle() {
  if(readUltrasonicSensor() < 20.0) {
    brake();
    return true;
  }
  return false;
}

double readUltrasonicSensor() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    return pulseIn(echoPin, HIGH) * 0.34 / 2; //distance
}

void dumbAvoid() {
  
}

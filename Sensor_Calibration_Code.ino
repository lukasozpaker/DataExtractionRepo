

#define echoPin 13 
#define trigPin 11 

long duration; 
int distance; 

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600); 
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); 
  Serial.println("with Arduino UNO R3");
}
void loop() {
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance = duration * 0.34 / 2; 
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("mm");
  
}

double sideX(double yDist, double sensorDist) {
  return sqrt(sensorDist * sensorDist - yDist * yDist);

}

#define echoPin 2 
#define trigPin 3 


void setup() {
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
}
void loop() {
    
}


double sideX(double yDist, double sensorDist) { //in case of angled sensor
    return sqrt(sensorDist * sensorDist - yDist * yDist);

}
double readUltrasonicSensor() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    return pulseIn(echoPin, HIGH) * 0.34 / 2; //distance
}




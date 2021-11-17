//begin wifi imports
//#include "Enes100.h"
//end wifi imports

//begin ultrasonic imports
#define echoPin 2 
#define trigPin 3 
//end ultrasonic imports

int in1 = 7;  //direction 1
int in2 = 8;  //direction 1
int ena = 5;  //power 1

int in3 = 12;  //direction 2
int in4 = 13;  //direction 2
int enb = 11;  //power 2

//tolerance in degrees
const double TOL = 6.0;
const double COARSE_PIVOT = 100;
const double FINE_PIVOT = 70;
const double DEFAULT_SPEED = 200;

void setup() {
  pinMode(in1, OUTPUT);  // sets the pin as output
  pinMode(in2, OUTPUT); // sets the pin as output
  pinMode(ena, OUTPUT);
  
  pinMode(in3, OUTPUT);  // sets the pin as output
  pinMode(in4, OUTPUT); // sets the pin as output
  pinMode(enb, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

//  Enes100.begin("Ganymede", DATA, 9, 8, 9);
//  Enes100.updateLocation();

    //code 3, works glitchy
//Serial.begin(9600);
//Serial.println("start");
//
//
//bool cond = true;
//delay(100);
//while(cond){
//  setBothMotors(200);
//  if(obstacle()) {
//    cond  = false;
//    Serial.println("OBS");
//  }
//}
//brake();
//while(true) {
//  pivotCW(255);
//}
  //end code 3, 
  
  



   //code set 2
//Serial.begin(9600);
//while(true){
//  Serial.println(readUltrasonicSensor());
//  if(obstacle()){
//    Serial.println("OBSTACLE");
//  }
//}
  //end code set 2


    int sw = 0;

    switch(sw) {
        case 0:
            forward();
            break;
        case 1:
            navToMissionSite();
            break;
        case 2:
            navPastObstacles();
            break;
        case 3:
            navToDestination();
            break;
        case 4:
            pivotCW(255);
            break;
        case 5:
            pivotCCW(255);
            break;
        case 6:
            fixedForward();
            break;
    }
    
    
    
    
    //mission complete 
}



void loop() {
   
}
void navToMissionSite() {
  finalForward();
}

void navPastObstacles() {
  readUltrasonicSensor();
  delay(100);
  for(int i=0;i<4;i++) {
    while(!obstacle()) {
      setBothMotors(150);
    }
  }



  
}

void navToDestination() {
  
}

void finalForward() {
  while(true){
    setBothMotors(DEFAULT_SPEED);
  }
}
void forward(){
    readUltrasonicSensor();
    delay(100);
    while(true) {
        if(obstacle()) {
            brake();
            //avoid
                dumbCW();
                fixedForward();
                dumbCCW();
            //avoid
        return;
      }
     setBothMotors(DEFAULT_SPEED);
    } 
}


//begin overhead vision system functions
//void turnAbs(double deg, double tol){
//  while((fabs(deg-theta()) > tol)) { //rough control
//    pivotCW(COARSE_PIVOT);
//    Enes100.updateLocation();
//    //Enes100.println(theta());
//  }
//  
//  while((fabs(deg-theta()) > tol/5.0)) { //fine adjustment
//    pivotCW(FINE_PIVOT);
//    Enes100.updateLocation();
//  }
//  brake();
//}
//
//void navTo(double x, double y) {
//  double deltaX = x-Enes100.location.x;
//  double deltaY = y-Enes100.location.y;
//  
//  if(deltaY > 0) { //move north
//    // Enes100.print("north called: delta ");
//    // Enes100.println(deltaY);
//    turnAbs(90, TOL);
//    while(deltaY > 0.05) {
//      if(obstacle()){
//        brake();
//        smartAvoid();
//        return;
//      }
//      setBothMotors(DEFAULT_SPEED);
//      Enes100.updateLocation();
//      deltaY = y-Enes100.location.y;
//      
//    }
//    
//  } else if (deltaY < 0) { //move south
//    // Enes100.print("south called: delta ");
//    // Enes100.println(deltaY);
//    turnAbs(270, TOL);
//    while(deltaY < -0.05) {
//      if(obstacle()){
//        brake();
//        smartAvoid();
//        return;
//      }
//      setBothMotors(DEFAULT_SPEED);
//      Enes100.updateLocation();
//      deltaY = y-Enes100.location.y;
//    }
//  }
//  
//  
//  if(deltaX > 0) { //move east
//    // Enes100.print("east called: delta ");
//    // Enes100.println(deltaX);
//    turnAbs(0, TOL);
//    while(deltaX > 0.05) {
//      if(obstacle()){
//        brake();
//        smartAvoid();
//        return;
//      }
//      setBothMotors(DEFAULT_SPEED);
//      Enes100.updateLocation();
//      deltaX = x-Enes100.location.x;
//      
//    }
//  } else if (deltaX < 0) { //move west
//    // Enes100.print("west called: delta ");
//    // Enes100.println(deltaX);
//    turnAbs(0, TOL);
//    while(deltaX < -0.05) {
//      if(obstacle()){
//        brake();
//        smartAvoid();
//        return;
//      }
//      setBothMotors(DEFAULT_SPEED);
//      Enes100.updateLocation();
//      deltaX = x-Enes100.location.x;
//    }
//  }
//  brake();
//}
//
//void smartAvoid(){
//  Enes100.println("Smart Avoid");
//  if(Enes100.location.y < 2.0/3) { //bottom third
//    Enes100.println("2/3");
//    navTo(Enes100.location.x, 1.0); //basically Enes100.location.y+(2.0/3)
//  } else if(Enes100.location.y < 4.0/3) { //middle third
//    Enes100.println("4/3");
//    navTo(Enes100.location.x, Enes100.location.y+(2.0/3));
//  } else if(Enes100.location.y < 6.0/3) { //top third
//    Enes100.println("6/3");
//    navTo(Enes100.location.x, Enes100.location.y-(4.0/3));
//  }
//  turnAbs(0,TOL);
//  if(!obstacle()) {
//    navTo(Enes100.location.x+0.7, Enes100.location.y);
//  } else {
//        //OBSTACLE
//  }
//  
//  return;
//  
//  
//}
//end overhead vision system code

void setBothMotors(int SPEED) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,SPEED);

  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enb,SPEED);
}

void revBothMotors(int SPEED) {
  digitalWrite(in1, LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena,SPEED);

  digitalWrite(in3, LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb,SPEED);
}


void dumbPivot(int SPEED) {
  digitalWrite(in1, LOW);
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

void pivotCCW(int SPEED) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,SPEED);

  digitalWrite(in3, LOW);
  digitalWrite(in4,HIGH);
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
  if(readUltrasonicSensor() < 250.0) {
    brake();
    return true;
  }
  return false;
}

float readUltrasonicSensor() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    return pulseIn(echoPin, HIGH) * 0.34 / 2.0; //distance
}

void dumbAvoid() {
    dumbCW();
    forward();
    delay(300);
    dumbCW();
    dumbCW();
    dumbCW();
    brake();
}

void dumbCW() {
    brake();
    for(int i=0;i<2;i++) {
        pivotCW(230);
        delay(800);
    }
    brake();
}

void dumbCCW() {
    brake();
    for(int i=0;i<2;i++) {
        pivotCCW(230);
        delay(800);
    }
    brake();
}

void fixedForward() {
    brake();
    for(int i=0;i<2;i++) {
        setBothMotors(DEFAULT_SPEED);
        delay(200);
    }
    brake();
}

//
//double theta() {
//    Enes100.updateLocation();
//  if(Enes100.location.theta < 0) {
//    return 360.0 + (Enes100.location.theta * 180 / M_PI);
//  }
//  return Enes100.location.theta  * 180 / M_PI;
//}

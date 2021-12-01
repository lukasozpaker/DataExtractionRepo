//begin wifi imports
#include "Enes100.h"
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
const double TOL = 8.0;
const double COARSE_PIVOT = 230;
const double FINE_PIVOT = 160;
const double DEFAULT_SPEED = 230;

void setup() {
  pinMode(in1, OUTPUT);  // sets the pin as output
  pinMode(in2, OUTPUT); // sets the pin as output
  pinMode(ena, OUTPUT);
  
  pinMode(in3, OUTPUT);  // sets the pin as output
  pinMode(in4, OUTPUT); // sets the pin as output
  pinMode(enb, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  //Tx 4
  //Rx 9
  Enes100.begin("Ganymede", DATA, 306, 4, 9); //causes around 1 second delay
  Enes100.updateLocation();

  
  

    int sw =9;

    if(sw == -2) {
      
       while(true) {
        while (!Enes100.updateLocation()) {
         // OSV's location was not found
           Enes100.println("404 Not Found");
        }
        Enes100.print("OSV is at (");
        Enes100.print(Enes100.location.x);
        Enes100.print(", ");
        Enes100.print(Enes100.location.y);
        Enes100.print(", ");
        Enes100.print(theta(Enes100.location.theta));
        Enes100.println(")"); 
        }
    }
    if(sw == -1){
      fixedForward();
      brake();
      delay(500);
      
      dumbCW();
      brake();
      delay(500);

      fixedForward(0.5);
      brake();
      delay(500);

      dumbCCW();
      brake();
      delay(500);

      fixedForward();
      brake();
      delay(500);      
//      readUltrasonicSensor();
//      delay(100);
//      for(int i=0;i<3;i++) {
//        while(true){
//        readUltrasonicSensor();
//        setBothMotors(DEFAULT_SPEED);
//        if(obstacle()) {
//          
//          dumbCW();
//          brake();
//          delay(500);
//          
//          fixedForward();
//          brake();
//          delay(500);
//          
//          dumbCCW();
//          brake();
//          delay(500);
//
//          fixedForward();
//          brake();
//          break;
//        }
//      }
//      }
      
    }

    
    if(sw == 0) {
//      for(int i=0;i<3;i++) {
//        forward();
//        brake();
//      }
      readUltrasonicSensor();
      delay(100);
      while(true){
        readUltrasonicSensor();
        setBothMotors(DEFAULT_SPEED);
        if(obstacle()) {
          
          dumbCW();
          fixedForward();
          dumbCCW();
          brake();
          break;
        }
      }
      
    }
    if(sw == 1) {
      navToMissionSite();
    }
    if(sw == 2) {
      navPastObstacles();
    }
    if(sw == 3) {
      navToDestination();
    }
    if(sw == 4) {
      dumbCW();
    }
    if(sw == 5) {
      dumbCCW();
    }
    if(sw == 6) {
      fixedForward();
    }
    if(sw == 7) {
      while(true){
        setBothMotors(255);
      }
      
    }
    if(sw == 8) {
      turnAbs(90,TOL);
    }
    if(Enes100.location.y > 1) {
      sw = 9;
    } else {
      sw = 10;
    }
    if(sw == 9){
      navTo(0.5,2.0/3.0);
    }
    if(sw == 10){
      navTo(0.5,2.0/3.0*2);
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
      readUltrasonicSensor();
      setBothMotors(DEFAULT_SPEED);
        if(obstacle()) {
          //avoid
            dumbCW();
            fixedForward();
            dumbCCW();
            fixedForward();
            dumbCCW();
            fixedForward();
            dumbCW();
            //avoid
            brake();
            return;
            
            
      }
     
    } 
}


//begin overhead vision system functions
void turnAbs(double deg, double tol){
  deg = deg+5;
  if(deg > 360){
    deg = deg -360.0;
  }
  while((fabs(deg-theta(Enes100.location.theta)) > tol)) { //rough control
    pivotCW(COARSE_PIVOT);
    Enes100.updateLocation();
    //Enes100.println(theta());
  }
  

  brake();
}

void navTo(double x, double y) {
  double deltaX = x-Enes100.location.x;
  double deltaY = y-Enes100.location.y;
  
  if(deltaY > 0) { //move north
    // Enes100.print("north called: delta ");
    // Enes100.println(deltaY);
    turnAbs(90, TOL);
    while(deltaY > 0.05) {
      if(obstacle()){
        brake();
        smartAvoid();
        return;
      }
      setBothMotors(DEFAULT_SPEED);
      Enes100.updateLocation();
      deltaY = y-Enes100.location.y;
      
    }
    
  } else if (deltaY < 0) { //move south
    // Enes100.print("south called: delta ");
    // Enes100.println(deltaY);
    turnAbs(270, TOL);
    while(deltaY < -0.05) {
      if(obstacle()){
        brake();
        smartAvoid();
        return;
      }
      setBothMotors(DEFAULT_SPEED);
      Enes100.updateLocation();
      deltaY = y-Enes100.location.y;
    }
  }
  
  
  if(deltaX > 0) { //move east
    // Enes100.print("east called: delta ");
    // Enes100.println(deltaX);
    turnAbs(0, TOL);
    while(deltaX > 0.05) {
      if(obstacle()){
        brake();
        smartAvoid();
        return;
      }
      setBothMotors(DEFAULT_SPEED);
      Enes100.updateLocation();
      deltaX = x-Enes100.location.x;
      
    }
  } else if (deltaX < 0) { //move west
    // Enes100.print("west called: delta ");
    // Enes100.println(deltaX);
    turnAbs(0, TOL);
    while(deltaX < -0.05) {
      if(obstacle()){
        brake();
        smartAvoid();
        return;
      }
      setBothMotors(DEFAULT_SPEED);
      Enes100.updateLocation();
      deltaX = x-Enes100.location.x;
    }
  }
  brake();
}

void smartAvoid(){
  Enes100.println("Smart Avoid");
  if(Enes100.location.y < 2.0/3) { //bottom third
    Enes100.println("2/3");
    navTo(Enes100.location.x, 1.0); //basically Enes100.location.y+(2.0/3)
  } else if(Enes100.location.y < 4.0/3) { //middle third
    Enes100.println("4/3");
    navTo(Enes100.location.x, Enes100.location.y+(2.0/3));
  } else if(Enes100.location.y < 6.0/3) { //top third
    Enes100.println("6/3");
    navTo(Enes100.location.x, Enes100.location.y-(4.0/3));
  }
  turnAbs(0,TOL);
  if(!obstacle()) {
    navTo(Enes100.location.x+0.7, Enes100.location.y);
  } else {
        //OBSTACLE
  }
  
  return;
  
  
}
//end overhead vision system code

void setBothMotors(int SPEED) {
 digitalWrite(in1, LOW);
  digitalWrite(in2,HIGH);
  analogWrite(ena,SPEED);

  digitalWrite(in3, LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb,SPEED);
}

void revBothMotors(int SPEED) {
 digitalWrite(in1, HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,SPEED);

  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
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
  digitalWrite(in1, HIGH);
  digitalWrite(in2,LOW);
  analogWrite(ena,SPEED);

  digitalWrite(in3, LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enb,SPEED);
}

void pivotCCW(int SPEED) {
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
  //160 smooth constant
    brake();
    for(int i=0;i<2;i++) {
        pivotCW(230);
        delay(410);
    }
    brake();
}

void dumbCCW() {
    brake();
    for(int i=0;i<2;i++) {
        pivotCCW(230);
        delay(410);
    }
    brake();
}

void fixedForward() {
    brake();
    for(int i=0;i<2;i++) {
        setBothMotors(DEFAULT_SPEED);
        delay(600);
    }
    brake();
}

void fixedForward(double ratio) {
    brake();
    for(int i=0;i<2;i++) {
        setBothMotors(DEFAULT_SPEED);
        delay(600 * ratio);
    }
    brake();
}

double theta(double angle) {
  if(angle < 0) {
    return 360.0 + (angle * 180 / M_PI);
  }
  return angle  * 180 / M_PI; 
  
 }

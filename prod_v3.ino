//begin wifi imports
#include "Enes100.h"
//end wifi imports

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

//tolerance in degrees
const double TOL = 6.0;
const double COARSE_PIVOT = 100;
const double FINE_PIVOT = 70;
const double DEFAULT_SPEED = 170;

void setup() {
  pinMode(in1, OUTPUT);  // sets the pin as output
  pinMode(in2, OUTPUT); // sets the pin as output
  pinMode(ena, OUTPUT);
  
  pinMode(in3, OUTPUT);  // sets the pin as output
  pinMode(in4, OUTPUT); // sets the pin as output
  pinMode(enb, OUTPUT);

  Enes100.begin("Ganymede", DATA, 9, 8, 9);
  Enes100.updateLocation();

//   while(true) {
//     setBothMotors(235);
//     delay(1000);
//     pivotCW(230);
//     delay(1000);
//     brake();
//     delay(1000);
//   }

    if(Enes100.updatesLocation() == false) {
        pivotCW(COARSE_PIVOT);
    }
    for(int i=0;i<10;i++) {
        forward();

        brake();
        delay(500);

        dumbAvoid();
        delay(500);
    }
    for(int i=0;i<10;i++) { //obstacles return function, number of times before rover stops
        navTo(3, 2/3);
    }
    brake();
    //mission complete
    



  
}



void loop() {
   
}

void forward(){
    while(!obstacle()) {
     setBothMotors(235);
    } 
}


//begin overhead vision system functions
void turnAbs(double deg, double tol){
	while((fabs(deg-theta()) > tol)) { //rough control
		pivotCW(COARSE_PIVOT);
		Enes100.updateLocation();
		//Enes100.println(theta());
	}
	
	while((fabs(deg-theta()) > tol/5.0)) { //fine adjustment
		pivotCW(FINE_PIVOT);
		Enes100.updateLocation();
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
    dumb90();
    forward();
    delay(300);
    dumb90();
    dumb90();
    dumb90();
    brake();
}

void dumb90() {
    brake();
    for(int i=0;i<3;i++) {
        pivotCW(230);
        delay(200);
    }
    brake();
}


double theta() {
    Enes100.updateLocation();
	if(Enes100.location.theta < 0) {
		return 360.0 + (Enes100.location.theta * 180 / M_PI);
	}
	return Enes100.location.theta  * 180 / M_PI;
}
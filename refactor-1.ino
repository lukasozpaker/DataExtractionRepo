//begin navigation imports
#include "Enes100.h"
#include "Tank.h"
#include <math.h>
//end nagivation imports

//begin ultrasonic imports
#define echoPin 2 
#define trigPin 3 
//end ultrasonic imports


//begin navigation variables 
const double TOL = 6.0;
double finalX = 3.75;
double finalY = 0.4;
//end navigation variables

//begin ultrasonic variables
long duration; 
int distance; 
//end ultrasonic variables

//begin motor variables
    // Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
    // Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;
//end motor variables



void setup() {

    //begin naviation code
	Enes100.begin("The Busters", CRASH_SITE, 3, 8, 9); // Required before you can use any other Enes100 functions.
	Tank.begin(); // Required before you can use any other Tank functions.
	setBothMotors(0);
	Enes100.updateLocation();
	


	//nav to mission location
	if(Enes100.location.y < 2) {
		navTo(Enes100.location.x,1.45); //upper location
	} else {
		navTo(1,0.55); //lower location
	}
	
	//final destination
	for(int i=0;i<16;i++){
        //repeats i times because after obstacle avoidance, function is returned
		navTo(finalX,finalY);
	}
    //end navigation code
		
		

	
	


}

void loop() {  
	//loop not used for project
		
}

void turnAbs(double deg, double tol){
	while((fabs(deg-theta()) > tol)) { //rough control
		pivotCW(255);
		Enes100.updateLocation();
		//Enes100.println(theta());
	}
	
	while((fabs(deg-theta()) > tol/5.0)) { //fine adjustment
		pivotCW(15);
		Enes100.updateLocation();
	}
	brake();
}




void navTo(double x, double y) {
	double deltaX = x-Enes100.location.x;
	double deltaY = y-Enes100.location.y;
	
	if(deltaY > 0) { //move north
		Enes100.print("north called: delta ");
		Enes100.println(deltaY);
		turnAbs(90, TOL);
		while(deltaY > 0.05) {
			if(obstacle()){
				brake();
				smartAvoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaY = y-Enes100.location.y;
			
		}
		
	} else if (deltaY < 0) { //move south
		Enes100.print("south called: delta ");
		Enes100.println(deltaY);
		turnAbs(270, TOL);
		while(deltaY < -0.05) {
			if(obstacle()){
				brake();
				smartAvoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaY = y-Enes100.location.y;
		}
	}
	
	
	if(deltaX > 0) { //move east
		Enes100.print("east called: delta ");
		Enes100.println(deltaX);
		turnAbs(0, TOL);
		while(deltaX > 0.05) {
			if(obstacle()){
				brake();
				smartAvoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaX = x-Enes100.location.x;
			
		}
	} else if (deltaX < 0) { //move west
		Enes100.print("west called: delta ");
		Enes100.println(deltaX);
		turnAbs(0, TOL);
		while(deltaX < -0.05) {
			if(obstacle()){
				brake();
				smartAvoid();
				return;
			}
			setBothMotors(255);
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
		Enes100.println("OBSTACLE");
	}
	return;
}

bool obstacle() {
	if(readUltrasonicSensor() < 20) {
		brake();
		return true;
	}
	return false;
}

//begin ultrasonic functions
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
//end ultrasonic functions

		
void brake() {
	Tank.setLeftMotorPWM(0);
	Tank.setRightMotorPWM(0);
}
void pivotCW(int power) {
	Tank.setLeftMotorPWM(power);
	Tank.setRightMotorPWM(-power);
}

void setBothMotors(int speed) {
	Tank.setLeftMotorPWM(speed);
	Tank.setRightMotorPWM(speed);
}
		  
double theta() {
	if(Enes100.location.theta < 0) {
		return 360.0 + (Enes100.location.theta * 180 / M_PI);
	}
	return Enes100.location.theta  * 180 / M_PI;
}

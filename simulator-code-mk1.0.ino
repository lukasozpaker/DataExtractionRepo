#include "Enes100.h"
#include "Tank.h"
#include <math.h>

const double TOL = 6.0;
double finalX = 3.75;
double finalY = 0.5;
bool blocked[] = {false,false,false,false};
/* The code inside void setup() runs only once, before the code in void loop(). */
void setup() {
	Enes100.begin("The Busters", CRASH_SITE, 3, 8, 9); // Required before you can use any other Enes100 functions.
	Tank.begin(); // Required before you can use any other Tank functions.
	setBothMotors(0);
	
	
	Enes100.updateLocation();
	//initial rotation to crash site
	
	//mission location
	if(Enes100.location.y < 2) {
		navTo(Enes100.location.x,1.45);
	} else {
		navTo(1,0.55);
	}
	
	//final destination
	navTo(finalX,finalY);
	for(int i=0;i<10;i++){
		navTo(finalX,finalY);
	}
		
		

	
	


}

void loop() {  
	//printPi();
	
	Enes100.updateLocation();
	
	Enes100.print("x: ");
	Enes100.println(Enes100.location.x);
	
	Enes100.print("y: ");
	Enes100.println(Enes100.location.y);
	
	Enes100.print("Degree: ");
	Enes100.println(theta());
	
	Enes100.print("Sensor: ");
	Enes100.println(Tank.readDistanceSensor(1));
	
	
		
}

/* This is an example function to make both motors drive
 * at the given power.
 */

void turnRel(double deg){
	pivotCW(255);
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
		Enes100.println("north called");
		turnAbs(90, TOL);
		while(deltaY > 0.05) {
			if(obstacle()){
				brake();
				avoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaY = y-Enes100.location.y;
			
		}
		
	} else { //move south
		Enes100.println("south called");
		turnAbs(270, TOL);
		while(deltaY < -0.05) {
			if(obstacle()){
				brake();
				avoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaY = y-Enes100.location.y;
		}
	}
	
	
	if(deltaX > 0) { //move east
		Enes100.println("east called");
		turnAbs(0, TOL);
		while(deltaX > 0.05) {
			if(obstacle()){
				brake();
				avoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaX = x-Enes100.location.x;
			
		}
	} else { //move west
		Enes100.println("west called");
		turnAbs(0, TOL);
		while(deltaX < -0.05) {
			if(obstacle()){
				brake();
				avoid();
				return;
			}
			setBothMotors(255);
			Enes100.updateLocation();
			deltaX = x-Enes100.location.x;
		}
	}
	brake();
}

void avoid() {
	Enes100.println("avoid");
	
	for(int i=0;i<7;i++) {
		navTo(Enes100.location.x, 1.75-(i*0.2));
		turnAbs(0,TOL);
		if(!obstacle()) {
			Enes100.println("Path Clear");
			navTo(Enes100.location.x+0.5, Enes100.location.y);
			navTo(finalX,finalY);
			return;
		} else {
			Enes100.println("Path Blocked");
		}
	}
	//navTo(Enes100.location.x-0.1, Enes100.location.y+0.3);

	
	
	
}

bool obstacle() {
	if(Tank.readDistanceSensor(1) < 0.25) {
		brake();
		return true;
	}
	return false;
}
		
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

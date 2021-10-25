#include "Enes100.h"
#include "Tank.h"
#include <math.h>

/* The code inside void setup() runs only once, before the code in void loop(). */
void setup() {
	Enes100.begin("Team Name Here", CRASH_SITE, 3, 8, 9); // Required before you can use any other Enes100 functions.
	Tank.begin(); // Required before you can use any other Tank functions.
	setBothMotors(0); // Set both motors to full power.
	turnAbs(40, 1);
	


}

void loop() { 
	//printPi();
	/*
	Enes100.updateLocation();
	Enes100.println(Enes100.location.y);
	Enes100.println(theta());
	if(Enes100.location.y > 0.5 && theta() > 35) {
		Tank.setRightMotorPWM(-255);
		Tank.setLeftMotorPWM(255);
	} else {
		setBothMotors(255);
	}
	if(Enes100.location.x > 3) {
		setBothMotors(-255);
	}
	*/
}

/* This is an example function to make both motors drive
 * at the given power.
 */

void turnRel(double deg){
	pivotCW();
}

void turnAbs(double deg, double tol){
	while((fabs(deg-theta()) > tol)) {
		pivotCW();
		Enes100.updateLocation();
		Enes100.println(theta());
	}
	brake();
}





void brake() {
	Tank.setLeftMotorPWM(0);
	Tank.setRightMotorPWM(0);
}
void pivotCW() {
	Tank.setLeftMotorPWM(50);
	Tank.setRightMotorPWM(-50);
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

#include "Enes100.h"
#include <math.h>

// if ovs.y > 2 {
//     rotate south
// } else {
//     rotate north
// }

// move forward

// complete mission

//once mission complete, rotate east
//coordinates +



//all distances are in centimeters



double xPos;
double yPos;
double thetaPos;
double aruco[] = {-1,-1,-1};
double sensors[] = {-1,-1,-1}; //0: left, 1: front, 2: right

void setup() {
    
    update();
}

void loop() {
    while(!Enes100.updateLocation()) {
        Enes100.println("ERROR: LOCATION NOT FOUND")
    }
    updateCoordinates();
    xPos = aruco[0];
    yPos = aruco[1];
    thetaPos = aruco[2];

    updateSensors();


}

double updateSensors(){
    //TODO: interface with arduino
    //pull data from sensors
    sensors[0] = convertSensor(ARDUINO.PINX);
    sensors[1] = convertSensor(ARDUINO.PINX);
    sensors[2] = convertSensor(ARDUINO.PINX);
}
double convertSensor(double sensor) {
    //sensor conversion factor
}

boolean objectDetectLeft() {
    if(sensors[0] <= 20) return true;
}

boolean objectDetectFront() {
    if(sensors[1] <= 20) return true;
}

boolean objectDetectRight() {
    if(sensors[2] <= 20) return true;
}

double theta() {
	if(Enes100.location.theta < 0) {
		return 360.0 + (Enes100.location.theta * 180 / M_PI);
	}
	return Enes100.location.theta  * 180 / M_PI;
}
void updateCoordinates() {
    Enes100.updateLocation();
    aruco[0] = Enes100.location.x;
    aruco[1] = Enes100.location.y;
    aruco[2] = theta();
}


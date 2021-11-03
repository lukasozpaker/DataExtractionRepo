#include "Enes100.h"

void setup() {
    // Initialize Enes100 Library
    // Team Name, Mission Type, Marker ID, TX Pin, RX Pin
    Enes100.begin("Ganymede", DATA, 9, 8, 9);
    Serial.begin(9600);

    Serial.print("Destination is at (");
    Serial.print(Enes100.destination.x);
    Serial.print(", ");
    Serial.print(Enes100.destination.y);
    Serial.println(")");
    
    // Transmit the message from the arduino
    Serial.println(Enes100.mission('M'));
    Enes100.println('M');
    
}

void loop() {
    // Update the OSV's current location
    while (!Enes100.updateLocation()) {
        // OSV's location was not found
        Serial.println("404 Not Found");
    }
    Serial.print("OSV is at (");
    Serial.print(Enes100.location.x);
    Serial.print(", ");
    Serial.print(Enes100.location.y);
    Serial.print(", ");
    Serial.print(theta(Enes100.location.theta));
    Serial.println(")"); }

double theta(double angle) {
  if(angle < 0) {
    return 360.0 + (angle * 180 / M_PI);
  }
  return angle  * 180 / M_PI; 
  
 }

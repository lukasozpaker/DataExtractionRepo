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
const double TOL = 12.0;
const double COARSE_PIVOT = 230;
const double FINE_PIVOT = 160;
const double DEFAULT_SPEED = 230;

const double UP = 90;
const double DOWN = 270;
const double EAST = 0;
const double WEST = 180;
//constants can be fed into orient() function for readibility

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
  //6.5 tiles by 3.75 tiles : rough arena dimensions
  //fixed forward moves 1.5 tiles default, can take a param that a multiple ie 0.5, 1.2 etc
  //orient(turns to angle (up is 90, east (spawn to endzone) is 0, down is 270, west is 180);
  //Enes100.updateLocation() updates our OVS data
  //obstacle() returns true if obstacle is within 250 mm of OTV

  //building blocks are: orient(degree), fixedForward(multiplier * 1.5 tile length), obstacle(): returns bool if obs, brake(): brakes, 
  
  

    int sw =0;
    //0: navigate to mission, then destination just using turning and moving forward in increments
    //1: navigate to mission, then destination using navto method which takes 2 coordinates to move
    //2: simplified, gets to mission then tries to straightshot to destination, going diagonally
    //3: most simple: get to mission site
    //4: most simple: get to destination zone
    
    //8: only print out aruco position on the control panel
    //9: move forward until obstacle, if obstacle -> function breaks, rover stops

    Enes100.updateLocation();
 

    if(sw ==0) {
      //mission start
      if(Enes100.location.y >=1) { //if at top half nav to bottom for mission
        orient(270); //orient down
        brake();
        fixedForward(0.66);//move 0.8x the fixedforward default length, should get us to mission site
        brake();
      } else { //if at bottom half nav to top for mission
        orient(90); //orient up
        brake();
        fixedForward(0.66);//move 0.8x the fixedforward default length, should get us to mission site
        brake();
      }
      brake();
      delay(2000); //stay at mission site for 2 seconds
      fixedBackward(0.2); //back up from mission site, avoid collsion with site
      
      //mission end

      
      
      
      //get to endzone start
      orient(0); //orient east, towards mission

      //moves rover checks for obstacles then moves rover 0.2*fixedlength, repeats
      for(int i=0;i<10;i++) {
        if(!obstacle()){
          fixedForward(0.2);
        } else {
          defaultAvoid();
          brake();
        }
      }
      //if after those movements not in top third move ot top third for destination
      if(Enes100.location.y <2/3) {
        orient(UP);
        if(!obstacle()) {
          fixedForward(1.2);
        }
      } else if(Enes100.location.y < 4/3) {
        orient(UP);
        if(!obstacle()) {
          fixedForward(0.6);
        }
      }
      brake();
      orient(EAST);

      //moves into destination zone
      for(int i=0;i<10;i++) {
        if(!obstacle()){
          fixedForward(0.2);
        } else {
          defaultAvoid();
          brake();
        }
      }
      brake();

      

      //finshed
    }



    
    if(sw==1){
      if(Enes100.location.y >= 1) {
        navTo(0.5,0.5);
      } else if(Enes100.location.y < 1) {
        navTo(0.5,1.5);
      }

      for(int i=0;i<3;i++) { 
        //navto for this needs to be run in a loop bc the 
        //programmed behavior is to brake and return nothing when
        //obstacle is encountered, so need to repeatedly
        //feed in coordinates
        navTo(3.5, 1.5);
      }
      brake();
      
    }
    if(sw == 2) {
      //mission start
      //program branches based on spawn point
      if(Enes100.location.y >=1) { //if at top half nav to bottom for mission
        orient(270); //orient down
        brake();
        fixedForward(0.8);//move 0.8x the fixeforward default length, should get us to mission site
        brake();
        brake();
        
        delay(2000); //stay at mission site for 2 seconds
        fixedBackward(0.2); //back up from mission site, avoid collsion with site

        orient(40);
        fixedForward(1);
        brake();
        fixedForward(1);
        brake();
        fixedForward(1);
        brake();
        fixedForward(1);

      
      } else { //if at bottom half nav to top for mission
        orient(90); //orient up
        brake();
        fixedForward(0.8);//move 0.8x the fixedforward default length, should get us to mission site
        brake();

        delay(2000); //stay at mission site for 2 seconds
        fixedBackward(0.2); //back up from mission site, avoid collsion with site

        //if at top third, move straight go through mission
        for(int i=0;i<20;i++) {
          if(!obstacle()){
            fixedForward(0.2);
          } else {
            defaultAvoid();
            fixedForward();
            orient(UP);
            fixedForward(0.6);
            orient(EAST);
            brake();
          }
        } 
        brake();
        
        
      }
     
    }
    if(sw == 3) {
       if(Enes100.location.y >=1) { //if at top half nav to bottom for mission
        orient(270); //orient down
        brake();
        fixedForward(0.8);//move 0.8x the fixedforward default length, should get us to mission site
        brake();
     } else { //if at bottom half nav to top for mission
        orient(90); //orient up
        brake();
        fixedForward(0.8);//move 0.8x the fixedforward default length, should get us to mission site
        brake();
      }
       brake();
    }

    if(sw == 4) {
      orient(EAST);
      for(int i=0;i<16;i++) {
          if(Enes100.location.x > 3.0) {
              brake();
              break;
          }
          if(obstacle()) {
              fullAvoid();
          }

      }
      if(Enes100.location.y < 2.0/3 && Enes100.location.x > 3.0) {
              orient(UP);
              fixedForward(1.3);
              orient(EAST);
              fixedForward(1);
              brake();
              //end in destination
          } else if(Enes100.location.y < 4.0/3 && Enes100.location.x > 3.0) {
              orient(UP);
              fixedForward(0.66);
              orient(EAST);
              fixedForward(1);
              brake();
              //end in destination
          } else if(Enes100.location.y >= 4.0/3 && Enes100.location.x > 3.0) {
              orient(EAST);
              fixedForward(1);
              brake();
              //end in destination
          }
    }




    
    if(sw == 8) {
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
    if(sw == 9) {
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
   
    

        
    
    
    
    
    
    //mission complete 
}



void loop() {
   
}




void navPastObstacles() {
  readUltrasonicSensor();
  delay(100);
  for(int i=0;i<4;i++) {
    while(!obstacle()) {
      setBothMotors(150);
      delay(100);
    }
  }



  
}




void finalForward() {
    setBothMotors(DEFAULT_SPEED);
  
}
void forward(){
    readUltrasonicSensor();
    delay(100);
    while(true) {
      setBothMotors(DEFAULT_SPEED);
      if(obstacle()) {
        brake();
        break;
      }
     
    } 
}


//begin overhead vision system functions
//void turnAbs(double deg, double tol){
//

//  while((fabs(deg-theta(Enes100.location.theta)) > tol)) { //rough control
//    pivotCW(COARSE_PIVOT);
//    Enes100.updateLocation();
////    Enes100.println(theta(Enes100.location.theta));
//
//  }
//
//  //fine constant fix
//  
//  brake();
//  dumbCCW();
//  brake();
//}

void orient(double deg) {
  Enes100.updateLocation();
  Enes100.println(Enes100.location.theta);
  delay(10);
  double deltaA = deltaAngle(theta(Enes100.location.theta), deg);
  //7.3 at 90
  //
    brake();
    pivotCW(230);
    delay(7.3 * deltaA * correction(deltaA));
    brake();
  
}

void hardAbs(double our, double deg) {
  double deltaA = deltaAngle(our, deg);
  //3.65 * 2 @ 90
  //
    brake();
        pivotCW(230);
        delay(7.3 * deltaA * correction(deltaA));
    brake();
  
} 


double correction(double angle) {
  return -0.000889 * angle + 1.08;
}

double deltaAngle(double our, double goal) {
  //corrected from v8
  double delta_theta;
  
  if(our >= 180) {
    delta_theta = our - goal;
    delta_theta = fabs(delta_theta);
  } else if (our < 180) {
    delta_theta = 360 - goal + our;
    delta_theta = fabs(delta_theta);
  } 
  if(our < 180 && our > goal) {
    delta_theta = our - goal;
  }
  if(our < goal) {
    delta_theta = 360 - goal + our;
  }
  return delta_theta;
}



void navTo(double x, double y) {
  Enes100.updateLocation();
  delay(10);
  double deltaX = x-Enes100.location.x;
  double deltaY = y-Enes100.location.y;
  
  if(deltaY > 0) { //move north
    Enes100.print("north called: delta ");
    Enes100.println(deltaY);
    orient(UP);
    while(deltaY > 0.1) {
      if(obstacle() && Enes100.location.x > 0.7){
        brake();
        smartAvoid();
        return;
      }
      setBothMotors(DEFAULT_SPEED);
      Enes100.updateLocation();
      deltaY = y-Enes100.location.y;
      
    }
    
  } else if (deltaY < 0) { //move south
    Enes100.print("south called: delta ");
    Enes100.println(deltaY);
    orient(DOWN);
    while(deltaY < -0.1) {
      if(obstacle() && Enes100.location.x > 0.7){
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
    Enes100.print("east called: delta ");
    Enes100.println(deltaX);
    orient(EAST);
    while(deltaX > 0.1) {
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
    Enes100.print("west called: delta ");
    Enes100.println(deltaX);
    orient(WEST);
    while(deltaX < -0.1) {
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
  orient(0);
  if(!obstacle()) {
    navTo(Enes100.location.x+0.7, Enes100.location.y);
  } else {
        //OBSTACLE
  }
  brake();
  
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

void defaultAvoid() {
  //always prefers to be in top third bc that's where the underpass to the destination zone is
  Enes100.println("Smart Avoid");
  if(Enes100.location.y < 2.0/3) { //bottom third
    Enes100.println("2/3");
    orient(UP);
    fixedForward(0.66);
  } else if(Enes100.location.y < 4.0/3) { //middle third
    Enes100.println("4/3");
    orient(UP);
    fixedForward(0.66);
  } else if(Enes100.location.y < 6.0/3) { //top third
    Enes100.println("6/3");
    orient(DOWN);
    fixedForward(0.66);
  }
  orient(EAST);
  
  
  return;
}

void fullAvoid() {

  //always prefers to be in top third bc that's where the underpass to the destination zone is
  Enes100.println("Smart Avoid");
  if(Enes100.location.y < 2.0/3) { //bottom third
    Enes100.println("2/3");
    orient(UP);
    fixedForward(0.66);
  } else if(Enes100.location.y < 4.0/3) { //middle third
    Enes100.println("4/3");
    orient(UP);
    fixedForward(0.66);
  } else if(Enes100.location.y < 6.0/3) { //top third
    Enes100.println("6/3");
    orient(DOWN);
    fixedForward(0.66);
  }
  orient(EAST);
  fixedForward(0.7);
  brake();
  
  
  return;

}

void dumbCW() { //turns 90 cw
  brake();
  pivotCW(230);
  delay(7.3 * 90);
  brake();
}

void dumbCCW() { //turns 90 ccw
    brake();
  pivotCCW(230);
  delay(7.3 * 90);
  brake();
}

//6.5 tiles by 3.75 tiles : rough arena dimensions
//600 = 1.5 tiles
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

void fixedBackward(double ratio) {
  brake();
  for(int i=0;i<2;i++) {
    revBothMotors(DEFAULT_SPEED);
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

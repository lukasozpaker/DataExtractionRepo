#include <Enes100.h>

int i = 0;
short initialStream = 0b1111111111111111;
unsigned char dataStream = 0b11111111;
short code = 0b000000001;
byte inBit;

void setup() {
  Serial.begin(9600);
  Enes100.begin("grrrrrrrrr", DATA, 3, 8, 9);
}

void loop() {
    
    delay(125);

    int sensorValue = analogRead(A0);
    //Serial.println(sensorValue);
    float voltage = sensorValue * (5.17 / 1024.0);
    Serial.println(voltage);
  
    if ((initialStream & 0b111111111) != (code & 0b111111111)) {
      if (voltage < 4.0 && voltage > 0.75) {
        Serial.println("not connected");
      } else if (voltage >= 4.0 && voltage <= 5.0) {
        inBit = 1;
        initialStream = (initialStream << 1) + inBit;
        //Serial.println(voltage);
        //Serial.println(1);
      } else if (voltage >= 0.0 && voltage <= 0.75) {
        inBit = 0;
        initialStream = (initialStream << 1) + inBit;
        //Serial.println(voltage);
        //Serial.println(0);
      }
    } else {
      if (i <= 0) {
        //Serial.println("Activation Match");
      }
      if (voltage < 4.0 && voltage > 0.75 && i < 8) {
        //Serial.println("not connected");
      } else if (voltage >= 4.0 && voltage <= 5.0 && i < 8) {
        inBit = 1;
        dataStream = (dataStream << 1) + inBit;
        //Serial.println(1);
      } else if (voltage >= 0.0 && voltage <= 0.75 && i < 8) {
        inBit = 0;
        dataStream = (dataStream << 1) + inBit;
        //Serial.println(0);
      }
      i++;
    }
    if (i == 8) {
      dataStream = (dataStream >> 1) + 0;
    }
    if (i >= 8) {
      Serial.println("Execution complete");
      char deliverable = dataStream;
      Serial.println(deliverable);
      Enes100.println(deliverable);
      Enes100.mission(deliverable);
    }
}

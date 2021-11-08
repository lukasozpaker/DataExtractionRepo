int i = 0;
short initialStream = 0b1111111111111111;
unsigned char dataStream = 0b11111111;
short code = 0b000000001;
byte inBit;

void setup() {
  Serial.begin(9600);
}

void loop() {
    
    delay(125);

    int sensorValue = analogRead(A0);
    float voltage = sensorValue * (5.0 / 1023.0);
  
    if ((initialStream & 0b111111111) != (code & 0b111111111)) {
      if (voltage < 4.0 && voltage > 1.0) {
        Serial.println("not connected");
      } else if (voltage >= 4.0 && voltage <= 5.0) {
        inBit = 1;
        initialStream = (initialStream << 1) + inBit;
        Serial.println(1);
      } else if (voltage >= 0.0 && voltage <= 1.0) {
        inBit = 0;
        initialStream = (initialStream << 1) + inBit;
        Serial.println(0);
      }
    } else {
      if (i <= 0) {
        Serial.println("Activation Match");
      }
      if (voltage < 4.0 && voltage > 1.0 && i < 8) {
        Serial.println("not connected");
      } else if (voltage >= 4.0 && voltage <= 5.0 && i < 8) {
        inBit = 1;
        dataStream = (dataStream << 1) + inBit;
        Serial.println(1);
      } else if (voltage >= 0.0 && voltage <= 1.0 && i < 8) {
        inBit = 0;
        dataStream = (dataStream << 1) + inBit;
        Serial.println(0);
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
    }
}

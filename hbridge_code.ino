int In1 = 7;      // MotorPin set to pin2
int In2 = 8;   // MotorDirectionPin set to pin3
int ENA = 5;   // MotorDirectionPin set to pin3
int SPEED = 235;
int In3 = 12;      // MotorPin set to pin2
int In4 = 13;   // MotorDirectionPin set to pin3
int ENB = 11;  


void setup()

{
 
  
  pinMode(In1, OUTPUT);  // sets the pin as output
  pinMode(In2, OUTPUT); // sets the pin as output
  pinMode(ENA, OUTPUT);

   pinMode(In3, OUTPUT);  // sets the pin as output
  pinMode(In4, OUTPUT); // sets the pin as output
  pinMode(ENB, OUTPUT);

  digitalWrite(In1, HIGH);
  digitalWrite(In2,LOW);
analogWrite(ENA,SPEED);

  digitalWrite(In3, HIGH);
  digitalWrite(In4,LOW);
analogWrite(ENB,SPEED);
}



void loop() {
  
  digitalWrite(In1, HIGH);
  digitalWrite(In2,LOW);
  analogWrite(ENA,SPEED);

  digitalWrite(In3, HIGH);
  digitalWrite(In4,LOW);
  analogWrite(ENB,SPEED);
}

const int enable1 = 3;
const int enable2 = 11;
const int enable3 = 9;
const int enable4 = 10; // motor right and left
const int motorA_In1 = 4;
const int motorA_In2 = 5;
const int motorB_In3 = 6;
const int motorB_In4 = 7;
void setup() {
  // put your setup code here, to run once:

pinMode(motorB_In3, OUTPUT);
pinMode(motorB_In4, OUTPUT);
pinMode(motorA_In1, OUTPUT);
pinMode(motorA_In2, OUTPUT);

}
void speedset(int speed){
  analogWrite(enable1, speed);
  analogWrite(enable2, speed);
  analogWrite(enable3, speed);
  analogWrite(enable4, speed);
}
void moveForward() {
  speedset(70);
  digitalWrite(motorA_In1, HIGH);
  digitalWrite(motorA_In2, LOW);
  digitalWrite(motorB_In3, HIGH);
  digitalWrite(motorB_In4, LOW);
}


void loop() {
moveForward();
}

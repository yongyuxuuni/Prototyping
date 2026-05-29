
const int trigPin1 = 12;
const int echoPin1 = 13;


const int trigPin2 = 10;
const int echoPin2 = 8;


const int motorA_In1 = 4;
const int motorA_In2 = 5;


const int motorB_In3 = 6;
const int motorB_In4 = 7;
const int safeDistance = 50; 
const int enable3=A4;
const int enable1=A5;


void setup() {

  Serial.begin(9600);

  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(enable3, OUTPUT);  
  pinMode(motorB_In3, OUTPUT);
  pinMode(motorB_In4, OUTPUT);
  
  
  pinMode(enable1, OUTPUT);
  pinMode(motorA_In1, OUTPUT);
  pinMode(motorA_In2, OUTPUT);

}

void loop() {
  
  analogWrite(enable3, 255);
  analogWrite(enable1, 255);
  
  long distance1 = getDistance(trigPin1, echoPin1);
  long distance2 = getDistance(trigPin2, echoPin2);

  
  Serial.print("Sensor 1: ");
  Serial.print(distance1);
  Serial.print(" cm | Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");


  if (distance1 > safeDistance && distance2 > safeDistance) {
  
    moveForward();
  } 
  else if (distance1 <= safeDistance && distance2 > safeDistance) {

    turnRight(); 
    delay(500); 
  } 
  else if (distance1 > safeDistance && distance2 <= safeDistance) {

    turnLeft();  
    delay(500);
  } 
  else {
    
    moveBackward();
    delay(600);
    turnRight(); 
    delay(500);
  }

  delay(50); 
}


long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); 
  long distance = duration * 0.034 / 2;

  if (distance == 0) {
    return 999;
  }
  return distance;
}


void moveForward() {
  digitalWrite(motorA_In1, HIGH);
  digitalWrite(motorA_In2, LOW);
  digitalWrite(motorB_In3, HIGH);
  digitalWrite(motorB_In4, LOW);
}

void moveBackward() {
  digitalWrite(motorA_In1, LOW);
  digitalWrite(motorA_In2, HIGH);
  digitalWrite(motorB_In3, LOW);
  digitalWrite(motorB_In4, HIGH);
}

void turnLeft() {
  
  
  analogWrite(enable1,180);
  digitalWrite(motorA_In1, LOW);
  digitalWrite(motorA_In2, HIGH);
  digitalWrite(motorB_In3, HIGH);
  digitalWrite(motorB_In4, LOW);
}

void turnRight() {
  digitalWrite(motorA_In1, HIGH);
  digitalWrite(motorA_In2, LOW);
  digitalWrite(motorB_In3, LOW);
  digitalWrite(motorB_In4, HIGH);
}

void stopMotors() {
  digitalWrite(motorA_In1, LOW);
  digitalWrite(motorA_In2, LOW);
  digitalWrite(motorB_In3, LOW);
  digitalWrite(motorB_In4, LOW);
}
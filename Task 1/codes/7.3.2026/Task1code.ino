// Written by Baris

// -------- IR Line Sensors --------
#define RIGHT_IR A0
#define LEFT_IR A1

// -------- L293D Motor Driver --------
// Right motor
#define RIGHT_EN1 5
#define RIGHT_IN1 8
#define RIGHT_IN2 7

// Left motor
#define LEFT_EN1 6
#define LEFT_IN1 10
#define LEFT_IN2 9

// -------- Settings --------
int motorSpeed = 85; // def 90
int turnSpeed = 80; // def 110
int correctionTime = 10; // ms

#define BLACK LOW
#define WHITE HIGH

void setup() {
  pinMode(RIGHT_IR, INPUT);
  pinMode(LEFT_IR, INPUT);

  pinMode(RIGHT_EN1, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  pinMode(LEFT_EN1, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int rightIR = digitalRead(RIGHT_IR);
  int leftIR = digitalRead(LEFT_IR);

  Serial.print("Left IR: ");
  Serial.print(leftIR);
  Serial.print(" Right IR: ");
  Serial.println(rightIR);

  if (leftIR == WHITE && rightIR == WHITE) {
    forward();
  }
  else if (leftIR == WHITE && rightIR == BLACK) {
    turnLeft();
  }
  else if (leftIR == BLACK && rightIR == WHITE) {
    turnRight();
  }
  else {
    forward(); // def fwd
  }
}

void forward() {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);

  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, motorSpeed);
  analogWrite(LEFT_EN1, motorSpeed);
}

void turnRight() {
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);

  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, 0);
  analogWrite(LEFT_EN1, turnSpeed);

  delay(correctionTime);
}

void turnLeft() {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);

  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, turnSpeed);
  analogWrite(LEFT_EN1, 0);

  delay(correctionTime);
}

void stopMotors() {
  analogWrite(RIGHT_EN1, 0);
  analogWrite(LEFT_EN1, 0);

  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);

  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);
}

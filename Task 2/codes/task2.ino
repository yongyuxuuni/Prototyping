// Written by Baris

// -------- Ultrasonic Sensors --------
#define RIGHT_TRIG A5
#define RIGHT_ECHO A4
#define LEFT_ECHO A2
#define LEFT_TRIG A3

// -------- IR Line Sensors --------
#define RIGHT_IR A0
#define LEFT_IR A1

// -------- L293D Motor Driver --------
#define RIGHT_EN1 5
#define RIGHT_IN1 8
#define RIGHT_IN2 7

#define LEFT_EN1 6
#define LEFT_IN1 10
#define LEFT_IN2 9

// -------- Settings --------
#define BLACK LOW
#define WHITE HIGH

int motorSpeed = 90;
int turnSpeed = 110;

int arcFast = 115;
int arcSlow = 85;

int obstacleLimit = 20; // cm

unsigned long lastUltrasonicCheck = 0;
const unsigned long ultrasonicInterval = 120;

long leftDistance = 999;
long rightDistance = 999;

void setup() {
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);

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
  updateUltrasonic();

  if (leftDistance < obstacleLimit || rightDistance < obstacleLimit) {
    stopMotors();
    delay(1000);

    if (leftDistance > rightDistance) {
      goAroundLeft();
    } else {
      goAroundRight();
    }
  } else {
    followLine();
  }
}

void followLine() {
  int leftIR = digitalRead(LEFT_IR);
  int rightIR = digitalRead(RIGHT_IR);

  if (leftIR == WHITE && rightIR == WHITE) {
    forward();
  }
  else if (leftIR == WHITE && rightIR == BLACK) {
    smallTurnLeft();
  }
  else if (leftIR == BLACK && rightIR == WHITE) {
    smallTurnRight();
  }
  else {
    forward();
  }
}

void goAroundLeft() {
  turnLeftInPlace();
  delay(250);

  forward();
  delay(500);

  turnRightInPlace();
  delay(250);

  forward();
  delay(500);

  // Counter direction: search right to get back to the line
  searchLineRight();
}

void goAroundRight() {
  turnRightInPlace();
  delay(250);

  forward();
  delay(500);

  turnLeftInPlace();
  delay(250);

  forward();
  delay(500);

  // Counter direction: search left to get back to the line
  searchLineLeft();
}

void searchLineRight() {
  unsigned long startTime = millis();

  while (millis() - startTime < 6000) {
    int leftIR = digitalRead(LEFT_IR);
    int rightIR = digitalRead(RIGHT_IR);

    if (leftIR == BLACK || rightIR == BLACK) {
      stopMotors();
      delay(100);
      return;
    }

    // Arc right
    digitalWrite(RIGHT_IN1, HIGH);
    digitalWrite(RIGHT_IN2, LOW);
    digitalWrite(LEFT_IN1, HIGH);
    digitalWrite(LEFT_IN2, LOW);

    analogWrite(RIGHT_EN1, arcSlow);
    analogWrite(LEFT_EN1, arcFast);
  }

  stopMotors();
}

void searchLineLeft() {
  unsigned long startTime = millis();

  while (millis() - startTime < 6000) {
    int leftIR = digitalRead(LEFT_IR);
    int rightIR = digitalRead(RIGHT_IR);

    if (leftIR == BLACK || rightIR == BLACK) {
      stopMotors();
      delay(100);
      return;
    }

    // Arc left
    digitalWrite(RIGHT_IN1, HIGH);
    digitalWrite(RIGHT_IN2, LOW);
    digitalWrite(LEFT_IN1, HIGH);
    digitalWrite(LEFT_IN2, LOW);

    analogWrite(RIGHT_EN1, arcFast);
    analogWrite(LEFT_EN1, arcSlow);
  }

  stopMotors();
}

void updateUltrasonic() {
  if (millis() - lastUltrasonicCheck >= ultrasonicInterval) {
    lastUltrasonicCheck = millis();

    leftDistance = getDistance(LEFT_TRIG, LEFT_ECHO);
    rightDistance = getDistance(RIGHT_TRIG, RIGHT_ECHO);
  }
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 5000);

  if (duration == 0) {
    return 999;
  }

  return duration * 0.034 / 2;
}

void forward() {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, motorSpeed);
  analogWrite(LEFT_EN1, motorSpeed);
}

void smallTurnLeft() {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, turnSpeed);
  analogWrite(LEFT_EN1, 0);

  delay(10);
}

void smallTurnRight() {
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, 0);
  analogWrite(LEFT_EN1, turnSpeed);

  delay(10);
}

void turnLeftInPlace() {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, HIGH);

  analogWrite(RIGHT_EN1, turnSpeed);
  analogWrite(LEFT_EN1, turnSpeed);
}

void turnRightInPlace() {
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, HIGH);
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, turnSpeed);
  analogWrite(LEFT_EN1, turnSpeed);
}

void stopMotors() {
  analogWrite(RIGHT_EN1, 0);
  analogWrite(LEFT_EN1, 0);

  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);
}

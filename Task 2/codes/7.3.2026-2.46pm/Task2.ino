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

int motorSpeed = 90;
int turnSpeed = 110;
int correctionTime = 10;

int obstacleLimit = 20;

#define BLACK LOW
#define WHITE HIGH

unsigned long lastUltrasonicCheck = 0;
const unsigned long ultrasonicInterval = 150;

long rightDistance = 999;
long leftDistance = 999;

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

  if (rightDistance < obstacleLimit || leftDistance < obstacleLimit) {
    stopMotors();
    return;
  }

  followLine();
}

void updateUltrasonic() {
  if (millis() - lastUltrasonicCheck >= ultrasonicInterval) {
    lastUltrasonicCheck = millis();

    rightDistance = getDistance(RIGHT_TRIG, RIGHT_ECHO);
    leftDistance = getDistance(LEFT_TRIG, LEFT_ECHO);
  }
}

void followLine() {
  int rightIR = digitalRead(RIGHT_IR);
  int leftIR = digitalRead(LEFT_IR);

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
    forward();
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

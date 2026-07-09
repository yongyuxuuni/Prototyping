// Written by Baris
// PID version added

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
#define LEFT true
#define RIGHT false

int motorSpeed = 90;

int arcFast = 115;
int arcSlow = 85;

int obstacleLimit = 20; // cm

// -------- PID Settings --------
float Kp = 35;
float Ki = 0;
float Kd = 18;

float error = 0;
float lastError = 0;
float integral = 0;

int minSpeed = 60;
int maxSpeed = 140;

long sampledDistance;
long targetDistance;
long obstacleTimer;

unsigned long lastPIDTime = 0;

unsigned long lastUltrasonicCheck = 0;
const unsigned long ultrasonicInterval = 120;

long leftDistance = 999;
long rightDistance = 999;

bool LR; // Left = True Right = 0



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

  lastPIDTime = millis();
}

void loop() {
  updateUltrasonic();

  if (leftDistance < obstacleLimit || rightDistance < obstacleLimit) {
    stopMotors();
    resetPID();
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

  /*
    Error values:
    0  = centered / no correction
    -1 = line detected on left, turn right
    1  = line detected on right, turn left
  */

  if (leftIR == WHITE && rightIR == WHITE) {
    error = 0;
  }
  else if (leftIR == BLACK && rightIR == WHITE) {
    error = -1;
  }
  else if (leftIR == WHITE && rightIR == BLACK) {
    error = 1;
  }
  else {
    error = lastError;
  }

  unsigned long now = millis();
  float dt = (now - lastPIDTime) / 1000.0;

  if (dt <= 0) dt = 0.001;

  integral += error * dt;
  integral = constrain(integral, -10, 10);

  float derivative = (error - lastError) / dt;

  float correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

  int leftSpeed = motorSpeed - correction;
  int rightSpeed = motorSpeed + correction;

  leftSpeed = constrain(leftSpeed, minSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, minSpeed, maxSpeed);

  setMotorSpeeds(leftSpeed, rightSpeed);

  lastError = error;
  lastPIDTime = now;
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(LEFT_EN1, leftSpeed);
  analogWrite(RIGHT_EN1, rightSpeed);
}

void resetPID() {
  error = 0;
  lastError = 0;
  integral = 0;
  lastPIDTime = millis();
}

long getAvgDistance() {
  long left = getDistance(LEFT_TRIG, LEFT_ECHO);
  long right = getDistance(RIGHT_TRIG, RIGHT_ECHO);

  if (left != 999 && right != 999) {
    return (left + right) / 2;
  } 
  else {
    return 999;
  }
}


void goAroundLeft() {
  unsigned long timerResult = 0;

  turnLeftInPlace();
  delay(350);

  sampledDistance = getAvgDistance();

  if (sampledDistance == 999 || sampledDistance < 25) {
    forward();
    delay(1000);

    turnRightInPlace();
    delay(350);

    forward();
    delay(1000);

    turnRightInPlace();
    delay(350);

    forward();
    delay(750);
  } 
  else {
    targetDistance = sampledDistance - 20;

    unsigned long obstacleTimer = millis();

    while (getAvgDistance() > targetDistance) {
      forward();

      if (millis() - obstacleTimer > 3000) {
        break;
      }
    }

    timerResult = millis() - obstacleTimer;

    turnRightInPlace();
    delay(350);

    forward();
    delay(1000);

    turnRightInPlace();
    delay(350);

    forward();
    delay(timerResult);
  }

  searchLineRight();
}

void goAroundRight() {
  unsigned long timerResult = 0;

  turnRightInPlace();
  delay(400);

  sampledDistance = getAvgDistance();

  if (sampledDistance == 999 || sampledDistance < 25) {
    forward();
    delay(1000);

    turnLeftInPlace();
    delay(500);

    forward();
    delay(1000);

    turnLeftInPlace();
    delay(500);

    forward();
    delay(750);
  } 
  else {
    targetDistance = sampledDistance - 20;

    unsigned long obstacleTimer = millis();

    while (getAvgDistance() > targetDistance) {
      forward();

      if (millis() - obstacleTimer > 3000) {
        break;
      }
    }

    timerResult = millis() - obstacleTimer;

    turnLeftInPlace();
    delay(500);

    forward();
    delay(1000);

    turnLeftInPlace();
    delay(500);

    forward();
    delay(timerResult);
  }

  searchLineLeft();
}

void searchLineRight() {
  unsigned long startTime = millis();

  while (millis() - startTime < 6000) {
    int leftIR = digitalRead(LEFT_IR);
    int rightIR = digitalRead(RIGHT_IR);

    if (leftIR == BLACK || rightIR == BLACK) {
      stopMotors();
      resetPID();
      delay(100);
      return;
    }

    digitalWrite(RIGHT_IN1, HIGH);
    digitalWrite(RIGHT_IN2, LOW);
    digitalWrite(LEFT_IN1, HIGH);
    digitalWrite(LEFT_IN2, LOW);

    analogWrite(RIGHT_EN1, arcSlow);
    analogWrite(LEFT_EN1, arcFast);
    delay(300)
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
      resetPID();
      delay(100);
      return;
    }

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
  setMotorSpeeds(motorSpeed, motorSpeed);
}

void turnLeftInPlace() {
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, HIGH);

  analogWrite(RIGHT_EN1, 110);
  analogWrite(LEFT_EN1, 110);
}

void turnRightInPlace() {
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, HIGH);
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  analogWrite(RIGHT_EN1, 110);
  analogWrite(LEFT_EN1, 110);
}

void stopMotors() {
  analogWrite(RIGHT_EN1, 0);
  analogWrite(LEFT_EN1, 0);

  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);
}
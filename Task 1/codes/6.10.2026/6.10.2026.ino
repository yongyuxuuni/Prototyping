//Written by Baris
// -------- Ultrasonic Sensors --------
#define RIGHT_TRIG A0
#define RIGHT_ECHO A1
#define LEFT_ECHO A2
#define LEFT_TRIG A3

// -------- IR Line Sensors --------
#define RIGHT_IR A4
#define LEFT_IR A5

// -------- L293D Motor Driver --------
// Right motor
#define RIGHT_EN1 3
#define RIGHT_IN1 4
#define RIGHT_IN2 5

// Left motor
#define LEFT_EN1 10
#define LEFT_IN1 8
#define LEFT_IN2 9

// -------- Settings --------
int motorSpeed = 60;
int turnSpeed = 50;
int obstacleLimit = 15; // cm

// Change these if your IR sensors work opposite
#define BLACK HIGH
#define WHITE LOW

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
int rightIR = digitalRead(RIGHT_IR);
int leftIR = digitalRead(LEFT_IR);

long rightDistance = getDistance(RIGHT_TRIG, RIGHT_ECHO);
delay(30);
long leftDistance = getDistance(LEFT_TRIG, LEFT_ECHO);
delay(30);

Serial.print("Left IR: ");
Serial.print(leftIR);
Serial.print(" Right IR: ");
Serial.print(rightIR);
Serial.print(" Left US: ");
Serial.print(leftDistance);
Serial.print("cm Right US: ");
Serial.print(rightDistance);
Serial.println("cm");

// Stop if obstacle is close
if (rightDistance < obstacleLimit || leftDistance < obstacleLimit) {
stopMotors();
return;
}

// Line following logic
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
stopMotors();
}
}

long getDistance(int trigPin, int echoPin) {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

long duration = pulseIn(echoPin, HIGH, 30000);

if (duration == 0) {
return 999;
}

return duration * 0.034 / 2;
}

void forward() {
analogWrite(RIGHT_EN1, motorSpeed);
analogWrite(LEFT_EN1, motorSpeed);


digitalWrite(RIGHT_IN1, HIGH);
digitalWrite(RIGHT_IN2, LOW);

digitalWrite(LEFT_IN1, HIGH);
digitalWrite(LEFT_IN2, LOW);
}

void turnLeft() {
analogWrite(RIGHT_EN1, turnSpeed);
analogWrite(LEFT_EN1, 0);

digitalWrite(RIGHT_IN1, HIGH);
digitalWrite(RIGHT_IN2, LOW);

digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, LOW);
}

void turnRight() {
analogWrite(RIGHT_EN1, 0);
analogWrite(LEFT_EN1, turnSpeed);

digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, LOW);

digitalWrite(LEFT_IN1, HIGH);
digitalWrite(LEFT_IN2, LOW);
}

void stopMotors() {
analogWrite(RIGHT_EN1, 0);
analogWrite(LEFT_EN1, 0);

digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, LOW);

digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, LOW);
}
//Written by Yongyu XU-6.17.2026.11AM

//fix logic:
//1.when both black, vehicle should stop
//2.when turning left/right, the turning speed shouldn't be 0
//3.when turning left/right, the direction of steering shoudl be reverse



// -------- Ultrasonic Sensors --------
#define LEFT_TRIG A0
#define LEFT_ECHO A1
#define RIGHT_ECHO A2
#define RIGHT_TRIG A3

// -------- IR Line Sensors --------
#define LEFT_IR A4
#define RIGHT_IR A5

// -------- L293D Motor Driver --------
// Left motor
#define LEFT_EN1 3
#define LEFT_IN1 4
#define LEFT_IN2 5

// Right motor
#define RIGHT_EN1 10
#define RIGHT_IN1 8
#define RIGHT_IN2 9
                                            
// -------- Settings --------

int motorSpeedLeft = 65;//initial speed
int motorSpeedRight = 65;//initial speed
int turnSpeedLeft = 30;//turning speed
int turnSpeedRight = 30;//turning speed
int    obstacleLimit = 15; // cm

// Change these if your IR sensors work opposite

#define BLACK LOW
#define WHITE HIGH

long getDistanceNonBlocking(int trigPin, int echoPin);
void runAvoidanceSequence(unsigned long currentMillis);

// -------- Millis() Non-blocking Timers --------
unsigned long lastUSCheckTime = 0;
const unsigned long USCheckInterval = 50; // Measures distance every 50 milliseconds

// Obstacle Avoidance
bool isAvoiding = false;
unsigned long avoidanceStartTime = 0;
int avoidanceStep = 0;

// Global distance variable
long leftDistance = 999;
long rightDistance = 999;

long getDistanceNonBlocking(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 15000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;

}

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
  unsigned long currentMillis = millis();
  // ---- 1. Using `millis()` to trigger ultrasonic distance measurement (non-blocking) ----
  if (currentMillis - lastUSCheckTime >= USCheckInterval) {
    lastUSCheckTime = currentMillis;
    // Alternate between reading the left and right ultrasonic signals to avoid mutual interference
    static bool readLeft = true;
    if (readLeft) {
      leftDistance = getDistanceNonBlocking(LEFT_TRIG, LEFT_ECHO);
      readLeft = false;
    } else {
      rightDistance = getDistanceNonBlocking(RIGHT_TRIG, RIGHT_ECHO);
      readLeft = true;
    }

  }

  // ---- 2. Avoidance Maneuver ----
  if (isAvoiding) {
    runAvoidanceSequence(currentMillis);
    return; // While avoiding obstacles, skip the line-following logic below
  }

  int rightIR = digitalRead(RIGHT_IR);
  int leftIR = digitalRead(LEFT_IR);

  Serial.print("Left IR: ");
  Serial.print(leftIR);
  Serial.print(" Right IR: ");
  Serial.print(rightIR);
  Serial.print(" Left US: ");
  Serial.print(leftDistance);
  Serial.print("cm Right US: ");
  Serial.print(rightDistance);
  Serial.println("cm");

  // Trigger obstacle avoidance conditions
  if (rightDistance < obstacleLimit || leftDistance < obstacleLimit) {
    stopMotors();
    isAvoiding = true;
    avoidanceStartTime = currentMillis;
    avoidanceStep = 1; // step 1 in obstacle avoidance
    return;
  }

  // Line following logic

  if (leftIR == WHITE && rightIR == WHITE) {
    stopMotors();//both white, move forward
    }

    else if (leftIR == WHITE && rightIR == BLACK) {
      turnRight();// right black=>veering to the left=>should turn right
      }

    else if (leftIR == BLACK && rightIR == WHITE) {
      turnLeft();// left black=>veering to the right=>should turn left
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
long duration = pulseIn(echoPin, HIGH, 30000);

if (duration == 0) {
  return 999;
  }
  return duration * 0.034 / 2;
  }

void forward() {
analogWrite(LEFT_EN1, motorSpeedLeft);
analogWrite(RIGHT_EN1, motorSpeedRight);

digitalWrite(LEFT_IN1, HIGH);
digitalWrite(LEFT_IN2, LOW);
digitalWrite(RIGHT_IN1, HIGH);
digitalWrite(RIGHT_IN2, LOW);

Serial.println("forward");
}

void turnLeft() {
analogWrite(LEFT_EN1, 0);
analogWrite(RIGHT_EN1, motorSpeedRight);

digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, LOW);
digitalWrite(RIGHT_IN1, HIGH);
digitalWrite(RIGHT_IN2, LOW);

Serial.println("TURN LEFT");

}

void turnRight() {
analogWrite(LEFT_EN1, motorSpeedLeft);
analogWrite(RIGHT_EN1, 0);
digitalWrite(LEFT_IN1, HIGH);
digitalWrite(LEFT_IN2, LOW);
digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, LOW);

Serial.println("TURN RIGHT");

}

void stopMotors() {
analogWrite(LEFT_EN1, 0);
analogWrite(RIGHT_EN1, 0);
digitalWrite(LEFT_IN1, LOW);
digitalWrite(LEFT_IN2, LOW);
digitalWrite(RIGHT_IN1, LOW);
digitalWrite(RIGHT_IN2, LOW);
Serial.println("stop");
}

// ---- Avoidance Maneuver Sequence ----

void runAvoidanceSequence(unsigned long currentMillis) {
  unsigned long elapsedTime = currentMillis - avoidanceStartTime;
  if (avoidanceStep == 1) {
    // Action 1: Reverse (600 milliseconds)
    if (elapsedTime < 600) {
      analogWrite(LEFT_EN1, motorSpeedLeft);
      analogWrite(RIGHT_EN1, motorSpeedRight);
      digitalWrite(LEFT_IN1, LOW);   digitalWrite(LEFT_IN2, HIGH);
      digitalWrite(RIGHT_IN1, LOW);  digitalWrite(RIGHT_IN2, HIGH);
    } else {
      avoidanceStep = 2;
      avoidanceStartTime = currentMillis; // Reset the timer
    }

  }

  else if (avoidanceStep == 2) {
    // Action 2: Turn right in place to face the opposite direction/go around (lasts 500 milliseconds)
    if (elapsedTime < 500) {
      analogWrite(LEFT_EN1, turnSpeedLeft);
      analogWrite(RIGHT_EN1, turnSpeedRight);
      digitalWrite(LEFT_IN1, HIGH);  digitalWrite(LEFT_IN2, LOW);
      digitalWrite(RIGHT_IN1, LOW);  digitalWrite(RIGHT_IN2, HIGH);
    } else {
      // Once the obstacle avoidance maneuver is complete, reset the system and return control to the line sensor.
      stopMotors();
      isAvoiding = false;

    }

  }

}
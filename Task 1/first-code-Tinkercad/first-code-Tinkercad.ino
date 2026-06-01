// ultrasonic sensors pins
const int trigPin1 = 12;//left
const int echoPin1 = 13;//left
const int trigPin2 = 10;//right
const int echoPin2 = 8;//right

//IR sensors pins
const int irSensorLeft = 3;   
const int irSensorRight = 2;  

//motor pins
const int motorA_In1 = 4;
const int motorA_In2 = 5;
const int motorB_In3 = 6;
const int motorB_In4 = 7;

const int enable3 = A4; // motor left
const int enable1 = A5; // motor right

// 
const int safeDistance = 50; 

void setup() {
  Serial.begin(9600);

  // Initialize ultrasonic sensors pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
  // Initialize IR sensors pins
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);
  
  // Initialize motor pins
  pinMode(enable3, OUTPUT);  
  pinMode(motorB_In3, OUTPUT);
  pinMode(motorB_In4, OUTPUT);
  
  pinMode(enable1, OUTPUT);
  pinMode(motorA_In1, OUTPUT);
  pinMode(motorA_In2, OUTPUT);
}

void loop() {
  // Set the motor to full speed
  analogWrite(enable3, 255);
  analogWrite(enable1, 255);
  
  // Read the distance from the ultrasonic sensor
  long distance1 = getDistance(trigPin1, echoPin1);
  long distance2 = getDistance(trigPin2, echoPin2);

  // Read the status of the IR sensor
 
  int irLeftState = digitalRead(irSensorLeft);
  int irRightState = digitalRead(irSensorRight);

  // Print data via the serial board
  Serial.print("US1: "); Serial.print(distance1);
  Serial.print("cm | US2: "); Serial.print(distance2);
  Serial.print("cm | IR_L: "); Serial.print(irLeftState);
  Serial.print(" | IR_R: "); Serial.println(irRightState);

  // Comprehensive Logic Control (Prioritizes IR signals)
  if (irLeftState == LOW && irRightState == HIGH) {
    turnRight(); // If an object or a black line is detected on the left, turn right
    delay(300);
  } 
  else if (irLeftState == HIGH && irRightState == LOW) {
    turnLeft();  // Object/black line detected on the right; turn left
    delay(300);
  }
  else if (irLeftState == LOW && irRightState == LOW) {
    moveBackward(); // Object/black line detected on the right; turn left
    delay(500);
    turnRight();
    delay(400);
  }
  // If IR sensor is not triggered, follow the existing ultrasonic logic.
  else {
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
  }

  delay(50); 
}

// Function for obtaining ultrasonic distance
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

//otor Control Action Functions
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
  analogWrite(enable1, 180); // Reduce the speed of Motor A when turning left
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
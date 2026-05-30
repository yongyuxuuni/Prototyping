const int trigPin = 9;
const int echoPin = 10;

void setup() {
  Serial.begin(9600); // Start communication with the computer
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration;
  int distance;

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the bounce back time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (Speed of sound is ~340m/s)
  distance = duration * 0.034 / 2;

  // Print to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Wait half a second before next reading
}
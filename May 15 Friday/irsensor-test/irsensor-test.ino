const int trackerPin = 2; // Signal pin
const int ledPin = 13;    // Built-in Arduino LED

void setup() {
  pinMode(trackerPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("KY-033 IR Sensor Test Initialized");
}

void loop() {
  // Read the sensor (LOW usually means white/reflective, HIGH means black/line)
  int detection = digitalRead(trackerPin);

  if (detection == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Line Detected (BLACK)");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("--- Surface (WHITE) ---");
  }

  delay(100); // Small delay for readability
}
// Motor Control Pins
const int motorA_IN1 = 9;
const int motorA_IN2 = 8;
const int motorB_IN3 = 7;
const int motorB_IN4 = 6;

// HC-SR04 Pins
const int trigPin = 12;
const int echoPin = 11;

// Filtering Parameters
const int filterWindowSize = 5;
long distanceBuffer[filterWindowSize];
int bufferIndex = 0;

// Obstacle Threshold Distance (in cm)
const int obstacleThreshold = 50;

void setup() {
  Serial.begin(9600);

  // Motor Pins Setup
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);

  // HC-SR04 Pins Setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize buffer with default large values
  for (int i = 0; i < filterWindowSize; i++) {
    distanceBuffer[i] = 100;
  }
}

void loop() {
  long rawDistance = getDistance();
  updateFilter(rawDistance);
  long filteredDistance = getFilteredDistance();

  // Debugging Output
  Serial.print("Raw: ");
  Serial.print(rawDistance);
  Serial.print(" cm | Filtered: ");
  Serial.print(filteredDistance);
  Serial.print(" cm | ");

  if (filteredDistance > obstacleThreshold) {
    moveForward();
    Serial.println("Moving Forward");
  } else {
    stopMoving();
    delay(300);

    moveBackward();
    Serial.println("Reversing...");
    delay(400);

    turnRight();
    Serial.println("Turning Right");
    delay(400);
  }

  delay(100);
}

// Get Distance from HC-SR04 Sensor
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);  // Timeout after ~25ms
  long distance = duration * 0.034 / 2;

  if (distance == 0 || distance) {
    
  }

  return distance;
}

// Update Moving Average Buffer
void updateFilter(long newReading) {
  distanceBuffer[bufferIndex] = newReading;
  bufferIndex = (bufferIndex + 1) % filterWindowSize;
}

// Calculate Moving Average
long getFilteredDistance() {
  long sum = 0;
  for (int i = 0; i < filterWindowSize; i++) {
    sum += distanceBuffer[i];
  }
  return sum / filterWindowSize;
}

// Motor Control Functions
void moveForward() {
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
}

void moveBackward() {
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
}

void turnRight() {
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
}

void stopMoving() {
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, LOW);
}

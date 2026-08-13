// ====================================================================
// LINE FOLLOWER ROBOT CODE (2-SENSOR setup)
// ====================================================================
// Arduino UNO + L298N Motor Driver + 2x IR Line Sensors

// --- MOTOR 1 (Left Motor) Pins ---
const int m1p1 = 4; // L298N IN1 (Direction)
const int m1p2 = 7; // L298N IN2 (Direction)
const int m1s  = 5; // L298N ENA (Speed - PWM)

// --- MOTOR 2 (Right Motor) Pins ---
const int m2p3 = 8; // L298N IN3 (Direction)
const int m2p4 = 9; // L298N IN4 (Direction)
const int m2s  = 6; // L298N ENB (Speed - PWM)

// --- IR SENSOR Pins ---
const int sensor1 = 2;  // Left IR Sensor (Digital OUT)
const int sensor2 = 10; // Right IR Sensor (Digital OUT)

// --- Sensor Signal Definitions ---
// Note: Standard IR sensors output LOW (0) on WHITE / Reflective surfaces 
// and HIGH (1) on BLACK line / Non-reflective surfaces.
const int BLACK = HIGH;
const int WHITE = LOW;

// --- Speed Settings (0 to 255) ---
const int BASE_SPEED = 120; // Forward speed
const int TURN_SPEED = 140; // Turning outer motor speed

void setup() {
  // Motor pins output mode
  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m1s,  OUTPUT);

  pinMode(m2p3, OUTPUT);
  pinMode(m2p4, OUTPUT);
  pinMode(m2s,  OUTPUT);

  // Sensor pins input mode
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
}

// Move Straight Forward
void forward() {
  // Left Motor Forward
  digitalWrite(m1p1, HIGH);
  digitalWrite(m1p2, LOW);
  analogWrite(m1s, BASE_SPEED);

  // Right Motor Forward
  digitalWrite(m2p3, LOW);
  digitalWrite(m2p4, HIGH);
  analogWrite(m2s, BASE_SPEED);
}

// Stop Both Motors
void stopRobot() {
  digitalWrite(m1p1, LOW);
  digitalWrite(m1p2, LOW);
  analogWrite(m1s, 0);

  digitalWrite(m2p3, LOW);
  digitalWrite(m2p4, LOW);
  analogWrite(m2s, 0);
}

// Gentle Left Turn: Stop Left Motor, Drive Right Motor Forward
void turnLeft() {
  // Left Motor STOPPED (Soft Pivot)
  digitalWrite(m1p1, LOW);
  digitalWrite(m1p2, LOW);
  analogWrite(m1s, 0);

  // Right Motor FORWARD
  digitalWrite(m2p3, LOW);
  digitalWrite(m2p4, HIGH);
  analogWrite(m2s, TURN_SPEED);
}

// Gentle Right Turn: Drive Left Motor Forward, Stop Right Motor
void turnRight() {
  // Left Motor FORWARD
  digitalWrite(m1p1, HIGH);
  digitalWrite(m1p2, LOW);
  analogWrite(m1s, TURN_SPEED);

  // Right Motor STOPPED (Soft Pivot)
  digitalWrite(m2p3, LOW);
  digitalWrite(m2p4, LOW);
  analogWrite(m2s, 0);
}

void loop() {
  int ls = digitalRead(sensor1); // Left Sensor Reading
  int rs = digitalRead(sensor2); // Right Sensor Reading

  // 1. Both sensors on WHITE surface -> Move Forward along the line
  if (ls == WHITE && rs == WHITE) {
    forward();
  }
  // 2. Left sensor hits BLACK line -> Turn Left to stay centered
  else if (ls == BLACK && rs == WHITE) {
    turnLeft();
  }
  // 3. Right sensor hits BLACK line -> Turn Right to stay centered
  else if (rs == BLACK && ls == WHITE) {
    turnRight();
  }
  // 4. Both sensors hit BLACK line (Intersection or T-junction) -> Stop
  else if (ls == BLACK && rs == BLACK) {
    stopRobot();
  }
}
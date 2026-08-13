# Complete Circuit Diagram & Wiring Guide: 2-Wheel Line Follower Robot

Comprehensive documentation for building, wiring, and programming a 2-wheel drive (2WD) Line Follower Robot using **Arduino UNO**, **L298N Dual H-Bridge Motor Driver**, **2x DC Gear Motors**, **2x IR Sensors**, and a **3x 18650 Li-ion Battery Power Unit**.

---

## Table of Contents
1. [Components Overview](#components-overview)
2. [Power Distribution & Battery Setup](#power-distribution--battery-setup)
3. [Master Wiring & Connection Tables](#master-wiring--connection-tables)
4. [Visual Circuit Diagram & Schematics](#visual-circuit-diagram--schematics)
5. [L298N Motor Driver Deep-Dive](#l298n-motor-driver-deep-dive)
6. [IR Sensor Working Principle & Tuning](#ir-sensor-working-principle--tuning)
7. [Robot Steering Logic & Algorithm](#robot-steering-logic--algorithm)
8. [Arduino Code Reference](#arduino-code-reference)
9. [Step-by-Step Testing & Troubleshooting Guide](#step-by-step-testing--troubleshooting-guide)

---

## Components Overview

| Component | Quantity | Purpose in System |
| :--- | :---: | :--- |
| **Arduino UNO R3** | 1 | Microcontroller (Brain of the robot) |
| **L298N Motor Driver** | 1 | Dual H-Bridge module to drive and speed-control motors |
| **BO DC Gear Motors** | 2 | 3V-12V Yellow Geared Motors for left & right wheels |
| **IR Line Sensor Modules** | 2 | TCRT5000 / Optical sensors for black line detection |
| **18650 Li-ion Batteries** | 3 | High-drain 3.7V rechargeable batteries (Series setup = 11.1V - 12.6V) |
| **3-Cell 18650 Battery Holder** | 1 | Holds batteries connected in series |
| **SPST Toggle / Rocker Switch** | 1 | Master ON/OFF power switch |
| **2WD Robot Chassis & Wheels** | 1 set | Mechanical frame, 2 wheels, and 1 caster wheel |
| **Jumper Wires & Screws** | As needed | Male-to-Male, Male-to-Female, Female-to-Female wires |

---

## Power Distribution & Battery Setup

### 1. Battery Configuration
- **Total Voltage:** 3 x 3.7V nominal cells = **11.1V Nominal** (Up to **12.6V Fully Charged**).
- **Power Connection Path:**
  - Battery Pack **(+) RED Wire** $\rightarrow$ **Switch (Terminal 1)**
  - **Switch (Terminal 2)** $\rightarrow$ **L298N Motor Driver `12V` Terminal**
  - Battery Pack **(-) BLACK Wire** $\rightarrow$ **L298N Motor Driver `GND` Terminal**

### 2. Common Ground Requirement (Crucial)
> [!IMPORTANT]
> **Why Common Ground is Mandatory:**
> The Arduino communicates with the L298N Motor Driver using 5V logic signals. For the L298N to register these signals accurately, both the Arduino and the L298N **MUST share the exact same electrical ground reference**.
> 
> Connect a wire from **L298N `GND`** directly to an **Arduino `GND` pin**.

### 3. Microcontroller Power Supply
- The L298N module has an onboard **7805 5V Voltage Regulator**.
- When supplied with 12V at the `12V` terminal, the regulator produces a clean 5V output at the **`5V` terminal**.
- Connect a wire from **L298N `5V` Terminal** to the **Arduino `5V` (or `VIN`) Pin** to power the Arduino without needing a separate USB or 9V battery.

---

## Master Wiring & Connection Tables

### Table 1: Power & Ground Bus

| Source Device | Source Pin | Destination Device | Destination Pin | Wire Color / Notes |
| :--- | :--- | :--- | :--- | :--- |
| **Battery Holder** | POSITIVE (+) Red Wire | **SPST Switch** | Terminal 1 | Power In |
| **SPST Switch** | Terminal 2 | **L298N Driver** | `12V` Screw Terminal | Switched Power |
| **Battery Holder** | NEGATIVE (-) Black Wire | **L298N Driver** | `GND` Screw Terminal | Main Ground |
| **L298N Driver** | `GND` Screw Terminal | **Arduino UNO** | `GND` Pin | **Common Ground** |
| **L298N Driver** | `5V` Screw Terminal | **Arduino UNO** | `5V` Pin | 5V Power to Arduino |

---

### Table 2: Motors to L298N Driver Output Terminals

> [!TIP]
> If a motor turns in reverse during forward motion testing, flip its Red and Black wire positions at the screw terminal.

| Motor | Motor Terminal | L298N Driver Output | Wire Color | Forward Control Pin | Reverse Control Pin |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Motor 1 (Left)** | Terminal A | **OUT1** | Red Wire | `IN1` = HIGH | `IN1` = LOW |
| **Motor 1 (Left)** | Terminal B | **OUT2** | Black Wire | `IN2` = LOW | `IN2` = HIGH |
| **Motor 2 (Right)** | Terminal A | **OUT3** | Red Wire | `IN3` = LOW | `IN3` = HIGH |
| **Motor 2 (Right)** | Terminal B | **OUT4** | Black Wire | `IN4` = HIGH | `IN4` = LOW |

---

### Table 3: L298N Control Pins to Arduino UNO

> [!NOTE]
> **Remove the black jumper caps** on the `ENA` and `ENB` pins on the L298N module to allow Arduino PWM speed control.

| L298N Pin | Arduino Pin | Code Variable | Signal Type | Function |
| :--- | :--- | :--- | :--- | :--- |
| **ENA** | **Pin 5** | `m1s` | PWM Output | Motor 1 (Left) Speed Control (0-255) |
| **IN1** | **Pin 4** | `m1p1` | Digital Output | Motor 1 Direction Pin 1 |
| **IN2** | **Pin 7** | `m1p2` | Digital Output | Motor 1 Direction Pin 2 |
| **IN3** | **Pin 8** | `m2p3` | Digital Output | Motor 2 Direction Pin 1 |
| **IN4** | **Pin 9** | `m2p4` | Digital Output | Motor 2 Direction Pin 2 |
| **ENB** | **Pin 6** | `m2s` | PWM Output | Motor 2 (Right) Speed Control (0-255) |

---

### Table 4: IR Sensors to Arduino UNO

| Sensor | Sensor Pin | Arduino Pin | Code Variable | Signal Description |
| :--- | :--- | :--- | :--- | :--- |
| **Left IR Sensor** | **VCC** | **Arduino 5V** | - | 5V Power Supply |
| **Left IR Sensor** | **GND** | **Arduino GND** | - | Ground Reference |
| **Left IR Sensor** | **OUT** | **Pin 2** | `sensor1` | Digital Signal (LOW = White, HIGH = Black) |
| **Right IR Sensor** | **VCC** | **Arduino 5V** | - | 5V Power Supply |
| **Right IR Sensor** | **GND** | **Arduino GND** | - | Ground Reference |
| **Right IR Sensor** | **OUT** | **Pin 10** | `sensor2` | Digital Signal (LOW = White, HIGH = Black) |

---

## Visual Circuit Diagram & Schematics

```
                          +-------------------------------+
                          |    3x 18650 BATTERY PACK      |
                          |     (11.1V - 12.6V DC)        |
                          +---------------+---------------+
                                          | (+)           | (-)
                                      [ SWITCH ]          |
                                          |               |
                                          v (12V Input)   v (Main GND)
                         +-----------------------------------+
                         |   L298N DUAL H-BRIDGE DRIVER      |
                         |                                   |
                         |  OUT1   OUT2       OUT3   OUT4    |
                         +---|------|-----------|------|-----+
                             |      |           |      |
                       [Red] |      | [Black]   | [Red]| [Black]
                             v      v           v      v
                         +--------------+   +--------------+
                         | LEFT MOTOR 1 |   | RIGHT MOTOR 2|
                         +--------------+   +--------------+

 L298N Control Header:
 ---------------------
 [ ENA ] ----------------------------------------------> Arduino Digital Pin 5  (PWM Speed Motor 1)
 [ IN1 ] ----------------------------------------------> Arduino Digital Pin 4  (Direction)
 [ IN2 ] ----------------------------------------------> Arduino Digital Pin 7  (Direction)
 [ IN3 ] ----------------------------------------------> Arduino Digital Pin 8  (Direction)
 [ IN4 ] ----------------------------------------------> Arduino Digital Pin 9  (Direction)
 [ ENB ] ----------------------------------------------> Arduino Digital Pin 6  (PWM Speed Motor 2)

 L298N Power Header:
 -------------------
 [ GND ] ----------------------------------------------> Arduino GND Pin       (COMMON GROUND)
 [ 5V  ] ----------------------------------------------> Arduino 5V / VIN Pin  (Powers Arduino)

 IR Line Sensors:
 ----------------
 Left Sensor [ VCC ] ----------------------------------> Arduino 5V
 Left Sensor [ GND ] ----------------------------------> Arduino GND
 Left Sensor [ OUT ] ----------------------------------> Arduino Digital Pin 2  (Left Sensor Input)

 Right Sensor [ VCC ] ---------------------------------> Arduino 5V
 Right Sensor [ GND ] ---------------------------------> Arduino GND
 Right Sensor [ OUT ] ---------------------------------> Arduino Digital Pin 10 (Right Sensor Input)
```

---

## L298N Motor Driver Deep-Dive

The L298N module contains a dual H-Bridge driver allowing directional and speed control over two separate DC motors.

### H-Bridge Logic Truth Table

#### Left Motor (Motor 1)
| IN1 (Pin 4) | IN2 (Pin 7) | ENA Pin 5 (PWM) | Left Motor State |
| :---: | :---: | :---: | :--- |
| **HIGH** | **LOW** | 0 to 255 | **FORWARD** (Speed determined by ENA PWM) |
| **LOW** | **HIGH** | 0 to 255 | **REVERSE** (Speed determined by ENA PWM) |
| **LOW** | **LOW** | Any | **STOP** (Motor coasts to stop) |
| **HIGH** | **HIGH** | Any | **BRAKE** (Motor stops immediately) |

#### Right Motor (Motor 2)
| IN3 (Pin 8) | IN4 (Pin 9) | ENB Pin 6 (PWM) | Right Motor State |
| :---: | :---: | :---: | :--- |
| **LOW** | **HIGH** | 0 to 255 | **FORWARD** (Speed determined by ENB PWM) |
| **LOW** | **HIGH** | 0 to 255 | **REVERSE** (Speed determined by ENB PWM) |
| **LOW** | **LOW** | Any | **STOP** (Motor coasts to stop) |
| **HIGH** | **HIGH** | Any | **BRAKE** (Motor stops immediately) |

---

## IR Sensor Working Principle & Tuning

### How standard TCRT5000 IR Modules work:
1. **IR Transmitter (LED):** Emits invisible infrared light downward onto the surface.
2. **IR Receiver (Photodiode):** Detects reflected infrared light.
3. **Surface Absorption vs Reflection:**
   - **White Surface:** High reflection $\rightarrow$ Photodiode conducts $\rightarrow$ Module Output = **LOW (0V)**.
   - **Black Surface/Line:** Absorbs IR light $\rightarrow$ No reflection $\rightarrow$ Module Output = **HIGH (5V)**.

```
       WHITE SURFACE (Reflective)            BLACK SURFACE (Absorptive)
         IR Emission        Reflection          IR Emission        Absorbed
            ||                 ^                  ||                 ||
            ||                 ||                 ||                 \/
            \/                 ||                 \/           ==============
      ==============================           ==============================
            Signal = LOW (0)                        Signal = HIGH (1)
```

### Calibration Steps:
1. Place the robot over the white track surface. Turn the small potentiometer screw on top of each IR sensor until the onboard indicator LED turns OFF.
2. Move the sensor over the black tape. The LED should illuminate brightly.

---

## Robot Steering Logic & Algorithm

### Why Soft Turning (Pivot) is Superior to Reverse Turning
- **Reverse Turning (Spin Turn):** Driving one motor forward and reversing the other causes violent turns. This often causes the line follower to lose track of thin tape lines.
- **Soft Pivot Turning:** Keep the outer wheel turning forward while completely stopping (or slowing down) the inner wheel. This makes smooth, stable turns along curved tracks.

### Truth Table & Action Matrix

| Left Sensor (`sensor1`) | Right Sensor (`sensor2`) | Robot Action | Left Motor | Right Motor |
| :---: | :---: | :--- | :--- | :--- |
| **WHITE (0)** | **WHITE (0)** | **FORWARD** | FORWARD (Speed: 120) | FORWARD (Speed: 120) |
| **BLACK (1)** | **WHITE (0)** | **TURN LEFT** | **STOP (Speed: 0)** | FORWARD (Speed: 140) |
| **WHITE (0)** | **BLACK (1)** | **TURN RIGHT** | FORWARD (Speed: 140) | **STOP (Speed: 0)** |
| **BLACK (1)** | **BLACK (1)** | **STOP** | STOP (Speed: 0) | STOP (Speed: 0) |

---

## Arduino Code Reference

The complete Arduino C++ code is located in [`code.ino`](file:///Users/pranav/PRNV/Programs/LineFollowerCar/code.ino).

```cpp
// ====================================================================
// LINE FOLLOWER ROBOT CODE (2-SENSOR SETUP)
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
const int BLACK = HIGH;
const int WHITE = LOW;

// --- Speed Settings (0 to 255) ---
const int BASE_SPEED = 120; // Forward speed
const int TURN_SPEED = 140; // Turning outer motor speed

void setup() {
  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m1s,  OUTPUT);

  pinMode(m2p3, OUTPUT);
  pinMode(m2p4, OUTPUT);
  pinMode(m2s,  OUTPUT);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
}

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

void stopRobot() {
  digitalWrite(m1p1, LOW);
  digitalWrite(m1p2, LOW);
  analogWrite(m1s, 0);

  digitalWrite(m2p3, LOW);
  digitalWrite(m2p4, LOW);
  analogWrite(m2s, 0);
}

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

  if (ls == WHITE && rs == WHITE) {
    forward();
  }
  else if (ls == BLACK && rs == WHITE) {
    turnLeft();
  }
  else if (rs == BLACK && ls == WHITE) {
    turnRight();
  }
  else if (ls == BLACK && rs == BLACK) {
    stopRobot();
  }
}
```

---

## Step-by-Step Testing & Troubleshooting Guide

### Phase 1: Power & Electrical Safety Verification
1. Before inserting batteries into the holder, double-check that **Battery (+) Red** connects to the Switch, and **Battery (-) Black** connects to `GND`.
2. Turn ON the switch and verify that the red LED on the L298N motor driver turns ON.
3. Verify that the Arduino power LED lights up (receiving 5V from the L298N regulator).

### Phase 2: Motor Spin Direction Verification
1. Lift the robot chassis off the ground so the wheels rotate freely.
2. Turn ON the battery switch.
3. Both wheels should rotate in the **FORWARD** direction.
   - If **Left Wheel** spins backward: Swap `OUT1` and `OUT2` wires on the L298N block.
   - If **Right Wheel** spins backward: Swap `OUT3` and `OUT4` wires on the L298N block.

### Phase 3: Common Issues & Solutions

| Symptom | Probable Cause | Solution |
| :--- | :--- | :--- |
| **Motors do not turn at all** | Missing ENA / ENB speed PWM output or missing ground link. | Ensure ENA is connected to Pin 5, ENB to Pin 6, and L298N GND is connected to Arduino GND. |
| **Car spins continuously in circles** | One IR sensor is miscalibrated or motor polarities reversed. | Calibrate IR sensor potentiometers over black tape vs white floor. Verify turn logic. |
| **Arduino keeps resetting during turns** | Low battery voltage causing voltage sag under motor load. | Recharge 18650 batteries. Ensure 3 cells are delivering at least 11V. |
| **Car moves backward when placed on white paper** | Inverted IR sensor logic. | Change `BLACK = LOW` and `WHITE = HIGH` at top of `code.ino` if your sensors output inverted logic. |

---


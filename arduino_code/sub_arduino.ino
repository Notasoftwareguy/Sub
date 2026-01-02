#include <Servo.h>

// Configuration
const int MAX_VALUES = 4;
const byte START_MARKER = 0xFE;
const unsigned long FAILSAFE_TIMEOUT = 1000; // 3 seconds in milliseconds
const int IMU_UPDATE_RATE_HZ = 500;

// --- Pin mapping ---
const int MOTOR_R_PIN = 9;
const int MOTOR_L_PIN = 10;
const int MOTOR_V_PIN = 11;

// Define Servo Objects
Servo motor_r, motor_l, motor_v;

// Global Variables
byte receivedNumbers[MAX_VALUES];
unsigned long last_motor_update = 0; // Stores the last time we got data
struct repeating_timer timer;



void setup() {
  Serial.begin(115200);
  
  // IMU Setup
  // int err = initializeIMU();

  // Motor Setup
  motor_r.attach(MOTOR_R_PIN);
  motor_l.attach(MOTOR_L_PIN);
  motor_v.attach(MOTOR_V_PIN);
  stopMotors();

  last_motor_update = millis(); 
}

// Update IMU buffer on second cpu
// void setup1() {
//   add_repeating_timer_ms((int) (-1000/((float) IMU_UPDATE_RATE_HZ)), updateStateInterrupt, NULL, &timer); 
// }

void loop() {
  int count = parseBinaryData(receivedNumbers);
  
  if (count >= 1) {
    switch (receivedNumbers[0]) {
      case 0: // Health check
        Serial.println(1);
        
      case 1: // Update motors
        updateServos(&receivedNumbers[1]); // point to second index to skip id number
        last_motor_update = millis();
      
      // case 2: // Get IMU State
      //   transmitState();
    }
  } 
  
  // Stop motors if havent received update in a while
  if ((millis() - last_motor_update) > FAILSAFE_TIMEOUT) {
    stopMotors();
  }
}





// --- Helper Functions ---
void updateServos(byte* values) {
  // Scale 0-254 input to 0-180 degrees
  motor_r.write(map(values[0], 0, 254, 0, 180)); 
  motor_l.write(map(values[1], 0, 254, 0, 180));
  motor_v.write(map(values[2], 0, 254, 0, 180));
}

void stopMotors() {
  motor_r.write(90);
  motor_l.write(90);
  motor_v.write(90);
}
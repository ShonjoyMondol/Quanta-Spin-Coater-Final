//#include <SoftwareSerial.h>  //if you use this the esc signal  cuts on each signals
#include <Servo.h>
#define nextion Serial

//SoftwareSerial nextion(5, 6);  //if you use this the esc signal  cuts on each signals


Servo esc; // Servo object to control the ESC
int coatingDuration = 0; // in seconds
int coatingSpeed = 3000; // Default to 3000 RPM
int remainingTime = 0;
bool isCoating = false; // Flag to track coating process
// Non-blocking timer variables
unsigned long previousMillis = 0;
const long interval = 1000; // 1 second
const int escPin = 9; // PWM pin connected to the ESC
const int minRPM = 3000; // Minimum RPM limit
const int maxRPM = 7000; // Maximum RPM limit
const int minPWM = 875; // Minimum PWM signal in microseconds
const int maxPWM = 2200; // Maximum PWM signal in microseconds
void setup() {
  Serial.begin(9600);
//  nextion.begin(9600);  //if you use this the esc signal  cuts on each signals
  esc.attach(escPin); // Attach ESC to pin 9
 
  // Calibrate the ESC (send max and min throttle to initialize)
  calibrateESC();
  Serial.println("Ready for Spin Coater setup with BLDC motor.");
}
void loop() {
  if (nextion.available()) {
    String command = nextion.readString();
    processCommand(command);
  }
  if (isCoating) {
    updateCoating();
  }
}
void processCommand(String command) {
  // Handle page 0 (coating setup)
  if (command == "b") {
    coatingDuration = max(0, coatingDuration - 1);
    updateNextionNumber("n0", coatingDuration); // Update coating duration on Nextion
  }
  else if (command == "c") {
    coatingDuration += 10;
    updateNextionNumber("n0", coatingDuration);
  }
  else if (command == "d") {
    // Ensure coating speed doesn't drop below 3000 RPM
    coatingSpeed = max(minRPM, coatingSpeed - 10);
    updateNextionNumber("n1", coatingSpeed); // Update coating speed on Nextion
  }
  else if (command == "e") {
    // Ensure coating speed doesn't exceed 7000 RPM
    coatingSpeed = min(maxRPM, coatingSpeed + 100);
    updateNextionNumber("n1", coatingSpeed);
  }
  else if (command == "a") {
    // Move to Page 2
    nextion.print("page 2");
    nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
    updateNextionNumber("p2n0", coatingDuration); // Show remaining time on page 2
    updateNextionNumber("p2n1", coatingSpeed); // Show coating speed on page 2
    remainingTime = coatingDuration;
  }
  else if (command == "f") {
    // Cancel button, return to page 0
    nextion.print("page 0");
    nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
    isCoating = false;
    stopMotor();
  }
  else if (command == "g") {
    // Start coating process
    Serial.println("Coating started.");
    startCoating();
  }
}
void startCoating() {
  isCoating = true;
  setMotorSpeed(coatingSpeed); // Set motor speed based on coatingSpeed
  remainingTime = coatingDuration;
  previousMillis = millis(); // Initialize the timer
 
  // Set text to "Coating . . ." and change color to black
  nextion.print("t4.txt=\"Coating . . .\"");
  nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
 
  // Change text color to black (black color code is typically 0)
  nextion.print("t4.pco=0");
  nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
}
void updateCoating() {
  unsigned long currentMillis = millis(); // Get current time
 
  if (remainingTime > 0) {
    if (currentMillis - previousMillis >= interval) { // Check if 1 second has passed
      previousMillis = currentMillis; // Reset timer
      remainingTime--; // Decrease countdown
     
      // Update reverse countdown on page 2
      updateNextionNumber("p2n0", remainingTime); // Update remaining time countdown
     
      updateProgressBar(); // Update progress bar
    }
    // No delay here — loop keeps running to maintain PWM!
  } else {
    stopMotor();
    isCoating = false;
   
    // Set text to "Coating Done" and change color to green
    nextion.print("t4.txt=\"Coating Done !\"");
    nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
   
    // Change text color to green (green color code in Nextion is 2016)
    nextion.print("t4.pco=2016");
    nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
  }
}
void setMotorSpeed(int rpm) {
  // Constrain RPM to be within the set limits
  rpm = constrain(rpm, minRPM, maxRPM);
 
  // Convert the RPM to a corresponding PWM value between 875 and 2200 microseconds
  int pwmSignal = map(rpm, minRPM, maxRPM, minPWM, maxPWM);
 
  esc.writeMicroseconds(pwmSignal); // Send the PWM signal to ESC
  Serial.print("Motor speed set to: ");
  Serial.print(rpm);
  Serial.print(" RPM (PWM: ");
  Serial.print(pwmSignal);
  Serial.println(" us)");
}
void updateProgressBar() {
  int progress = map(remainingTime, 0, coatingDuration, 0, 100);
  String command = "j0.val=" + String(progress);
  nextion.print(command);
  nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
}
void updateNextionNumber(String component, int value) {
  String command = component + ".val=" + String(value);
  nextion.print(command);
  nextion.write(0xFF); nextion.write(0xFF); nextion.write(0xFF);
}
void stopMotor() {
  esc.writeMicroseconds(minPWM); // Stop the motor (minimum throttle)
  Serial.println("Motor stopped.");
}
void calibrateESC() {
  Serial.println("Calibrating ESC...");
 
  // Send max throttle
  esc.writeMicroseconds(maxPWM);
  delay(2000); // Wait for ESC to register max throttle
 
  // Send min throttle (875 microseconds)
  esc.writeMicroseconds(minPWM);
  delay(2000); // Wait for ESC to register min throttle
 
  Serial.println("ESC Calibration complete.");
}

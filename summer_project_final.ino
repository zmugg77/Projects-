#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// const int motorPin1 = 6;
// const int motorPin2 = 7;

int pwmPin = 3;

const int baseMotorSpeed = 100;  // Set the base motor speed here


void setup() {
  Wire.begin();
  Serial.begin(9600);

  mpu.initialize();
  mpu.setFullScaleAccelRange(0);  // Replace MPU6050_ACCEL_FS_2 with 0

  pinMode (pwmPin, OUTPUT); 

  // Rest of the code remains the same
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calculate tilt angles
  float pitch = atan(-ax / sqrt(pow(ay, 2) + pow(az, 2))) * RAD_TO_DEG;
  float roll = atan(ay / sqrt(pow(ax, 2) + pow(az, 2))) * RAD_TO_DEG;

  // Adjust motor speed based on tilt angles
  int motorSpeed1 = map(abs(roll), 0, 25, 0, 255);
  int motorSpeed2 = map(abs(pitch), 0, 25, 0, 255);

  // Find the maximum motor speed among all axes
  int maxMotorSpeed = max(motorSpeed1, motorSpeed2);
  // Calculate the final motor speed by adding the base speed
  int finalMotorSpeed = baseMotorSpeed + maxMotorSpeed;

  // Set motor speed for all axes
  analogWrite(pwmPin, finalMotorSpeed);

  // Send motor speed and moment data over serial
  Serial.print(finalMotorSpeed);
  Serial.print(",");
  Serial.println(pitch);

  delay(100);
}
#include "FastIMU.h"
#include <Wire.h>
#define CIRCULAR_BUFFER_INT_SAFE
#include <CircularBuffer.hpp>

#define IMU_ADDRESS 0x68    //Change to the address of the IMU

// Configuration
const unsigned long AVERAGE_DURATION = 10; // max milliseconds of imu data to avg
// const int IMU_UPDATE_RATE_HZ = 500;
calData calib = { 0 };  //Calibration data

// Buffer initialization
CircularBuffer<AccelData, 100> accel_buffer;
CircularBuffer<GyroData, 100> gyro_buffer;
CircularBuffer<unsigned long, 100> time_buffer;
AccelData current_accel;
GyroData current_gyro;

MPU6050 IMU(Wire1);

void transmitState() {
  // Calculate mean of usable samples
  float ax = 0;
  float ay = 0;
  float az = 0;
  float gx = 0;
  float gy = 0;
  float gz = 0;

  int buff_index = 0;
  for (buff_index; buff_index < time_buffer.size() ; buff_index++) {
      if ((millis() - time_buffer[buff_index]) > AVERAGE_DURATION) {
        break;
      }
      
      ax += accel_buffer[buff_index].accelX;
      ay += accel_buffer[buff_index].accelY;
      az += accel_buffer[buff_index].accelZ;
      gx += gyro_buffer[buff_index].gyroX;
      gy += gyro_buffer[buff_index].gyroY;
      gz += gyro_buffer[buff_index].gyroZ;
  }
  
  if (buff_index == 0) {
    Serial.println("");
    return;
  } 

  // Calculate means
  ax = ax / buff_index;
  ay = ay / buff_index;
  az = az / buff_index;
  gx = gx / buff_index;
  gy = gy / buff_index;
  gz = gz / buff_index;

  // Print to serial
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.print(gx);
  Serial.print(",");
  Serial.print(gy);
  Serial.print(",");
  Serial.println(gz);
}

// Update all of the buffers
void updateState() {
  IMU.update();
  IMU.getAccel(&current_accel);
  IMU.getGyro(&current_gyro);

  time_buffer.unshift(millis());
  accel_buffer.unshift(current_accel);
  gyro_buffer.unshift(current_gyro);
}

int initializeIMU() {
  Wire1.begin();
  Wire1.setClock(400000); //400khz clock

  int failed_initialization = IMU.init(calib, IMU_ADDRESS) &&
                              IMU.setAccelRange(2) &&
                              IMU.setGyroRange(500);

  return failed_initialization;
}

bool updateStateInterrupt(struct repeating_timer *t) {
  updateState();
  return true;
}



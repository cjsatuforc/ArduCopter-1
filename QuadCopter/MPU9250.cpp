#include "MPU9250.h"


#define BUFFER_SIZE 6 // number of bytes to capture for each sensor

// Initialize the MPU registers
IMU::IMU(){
  XaValue = 0;
  YaValue = 0;
  ZaValue = 0;
  XgValue = 0;
  YgValue = 0;
  ZgValue = 0;
}
void IMU::MPUInit(uint8_t slave_addr, uint8_t* reg_addrs, uint8_t* initialValues) {

  
  for(int i = 0; i < sizeof(initialValues)-1; i++) {

      i2c_writeTo(slave_addr, reg_addrs[i], initialValues[i]);
  }
}
//reading Accelerometer data from MPU
void IMU::readAccelData(uint8_t slave_addr, uint8_t reg_start_addr) {

  byte Buffer[BUFFER_SIZE];
  i2c_readFrom(slave_addr, reg_start_addr, Buffer);
  XaValue = ((int16_t)Buffer[0] << 8) + ((int16_t) Buffer[1]); // First two byte are the X values
  YaValue = ((int16_t)Buffer[2] << 8) + ((int16_t) Buffer[3]); // Next two byte are the Y values
  ZaValue = ((int16_t)Buffer[4] << 8) + ((int16_t) Buffer[5]); // Last two byte are the Z values

}

//reading gyro data from MPU
void IMU::readGyroData(uint8_t slave_addr, uint8_t reg_start_addr) {
  
  byte Buffer[BUFFER_SIZE];
  i2c_readFrom(slave_addr, reg_start_addr, Buffer);
  XgValue = ((int16_t)Buffer[0] << 8) + ((int16_t) Buffer[1]); // First two byte are the X values
  YgValue = ((int16_t)Buffer[2] << 8) + ((int16_t) Buffer[3]); // Next two byte are the Y values
  ZgValue = ((int16_t)Buffer[4] << 8) + ((int16_t) Buffer[5]); // Last two byte are the Z values
}

// wrapper function for writing values to MPU registers
void IMU::writeMPUDATA(uint8_t slave_addr, uint8_t reg_addr, int value) {
  
  i2c_writeTo(slave_addr,reg_addr,value);
}

int* IMU::getGyroValues(int* gyroValues) {

  gyroValues[ROLL_VALUE] = XgValue;
  gyroValues[PITCH_VALUE] = YgValue;
  gyroValues[YAW_VALUE] = ZgValue;
}
int* IMU::getAccelValues(int* accelValues) {

  accelValues[ROLL_VALUE] = XaValue;
  accelValues[PITCH_VALUE] = YaValue;
  accelValues[YAW_VALUE] = ZaValue;
}


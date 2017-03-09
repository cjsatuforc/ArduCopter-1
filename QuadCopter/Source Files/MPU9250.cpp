#include "MPU9250.h"


#define BUFFER_SIZE 6 // number of bytes to capture for each sensor

// Initialize the MPU registers
IMU::IMU(uint8_t slave_addr) {
  XaValue = 0;
  YaValue = 0;
  ZaValue = 0;
  XgValue = 0;
  YgValue = 0;
  ZgValue = 0;
  slaveAddress = slave_addr;
}

//Initialize the motion processing unit by setting the appropriate registers
void IMU::IMUInit(uint8_t* reg_addrs, uint8_t* initialValues) {

  for (int i = 0; i < sizeof(initialValues) - 1; i++) {

    i2c_writeTo(slaveAddress, reg_addrs[i], initialValues[i]);
  }
}

//reading Accelerometer data from MPU
void IMU::readAccelData(uint8_t reg_start_addr) {

  byte Buffer[BUFFER_SIZE];
  i2c_readFrom(slaveAddress, reg_start_addr, Buffer);
  XaValue = ((int16_t)Buffer[0] << 8) + ((int16_t) Buffer[1]); // First two byte are the X values
  YaValue = ((int16_t)Buffer[2] << 8) + ((int16_t) Buffer[3]); // Next two byte are the Y values
  ZaValue = ((int16_t)Buffer[4] << 8) + ((int16_t) Buffer[5]); // Last two byte are the Z values

}

//reading gyro data from MPU
void IMU::readGyroData(uint8_t reg_start_addr) {

  byte Buffer[BUFFER_SIZE];
  i2c_readFrom(slaveAddress, reg_start_addr, Buffer);
  XgValue = ((int16_t)Buffer[0] << 8) + ((int16_t) Buffer[1]); // First two byte are the X values
  YgValue = ((int16_t)Buffer[2] << 8) + ((int16_t) Buffer[3]); // Next two byte are the Y values
  ZgValue = ((int16_t)Buffer[4] << 8) + ((int16_t) Buffer[5]); // Last two byte are the Z values
}

// wrapper function for writing values to MPU registers
void IMU::writeIMUData(uint8_t reg_addr, int value) {

  i2c_writeTo(slaveAddress, reg_addr, value);
}

// getting acceleromter and gyro values from IMU
void IMU::getGyroValues(int* gyroValues) {

  gyroValues[ROLL_VALUE] = XgValue;
  gyroValues[PITCH_VALUE] = YgValue;
  gyroValues[YAW_VALUE] = ZgValue;
}
void IMU::getAccelValues(int* accelValues) {

  accelValues[ROLL_VALUE] = XaValue;
  accelValues[PITCH_VALUE] = YaValue;
  accelValues[YAW_VALUE] = ZaValue;
}

// calculate calibration values and store offsets in IMU
void IMU::calibrateIMU(uint8_t gyroOffsetStartAddress, uint8_t accelOffsetStartAddress) {

  // TODO if needed: perform accelerometer calibration first
  for (int i = 0; i < ACCEL_CALIBRATION_READINGS; i++) {

  }

  // next perform gyro calibration
  int gyroOffset[3] = {0};
  for (int i = 0; i < GYRO_CALIBRATION_READINGS; i++) {
    readGyroData(gyroOffsetStartAddress);
    gyroOffset[0] += XgValue;
    gyroOffset[1] += YgValue;
    gyroOffset[2] += ZgValue;
  }
  for (int i = 0; i < 3; i++) {
    gyroOffset[i] /= GYRO_CALIBRATION_READINGS;
    int highByteOffset = (gyroOffset[i] >> 8) & 0xFF;
    int lowByteOffset = gyroOffset[i] & 0xFF;
    writeIMUData(i + GYRO_X_OFFSET_H, highByteOffset);
    writeIMUData(i + 1 + GYRO_X_OFFSET_H, lowByteOffset);
  }
}

void IMU::calculateAccelMagnitude() {
	
	accelMagnitude = sqrt((XaValue*XaValue) + (YaValue*YaValue) + (ZaValue*ZaValue));
	
}

void IMU::calculateGyroMagnitude() {
	
	gyroMagnitude = sqrt((XgValue*XgValue) + (YgValue*YgValue) + (ZgValue*ZgValue));
}

void IMU::calculateAngleDeviations() {
	
	float accelAnglePitch = asin((float)YaValue/accelMagnitude) * RAD_TO_DEG_CONVERSION;
	float accelAngleRoll =  asin((float)XaValue/accelMagnitude) * -RAD_TO_DEG_CONVERSION
	
	pitchAngle += YgValue*(ESC_PULSE_PERIOD/1000000); // calculate travelled pitch and roll and add this to the pitch and roll angle variables.
	rollAngle +=  XgValue*(ESC_PULSE_PERIOD/1000000); // ESC_PULSE_PERIOD is devided by 10^6 to convert it from microseconds to seconds
	
	//sin takes in radian values so convert from degrees to radians 
	pitchAngle-= rollAngle* sin(ZgValue * (PI/180) * ESC_PULSE_PERIOD/1000000); //If the IMU has yawed transfer the roll angle to the pitch angle.
	rollAngle+= pitchAngle* sin(ZgValue * (PI/180) * ESC_PULSE_PERIOD/1000000); //If the IMU has yawed transfer the pitch angle to the roll angle
	
	// perform gyro drift correction by using a complimetary filter 
	
	pitchAngle = pitchAngle*0.9996 + accelAnglePitch*0.0004;  
  	rollAngle = rollAngle*0.9996 + accelAngleRoll*0.0004; 
  	 
}

float* IMU::getAngleAdjustments() {
	
	float angleAdjustments[2] = {rollAngle, pitchAngle};
	return angleAdjustments;
}

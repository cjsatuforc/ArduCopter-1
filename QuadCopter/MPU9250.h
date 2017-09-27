/*
   MPU9250.h

    Created on: Mar 8, 2017
        Author: tdawson
*/

#ifndef MPU9250_H_
#define MPU9250_H_

#include "Common.h"
#include "I2CHelper.h"


class IMU {

int XaValue, YaValue, ZaValue;
int XgValue, YgValue, ZgValue;
int lastXg, lastYg, lastZg;
uint8_t slaveAddress;
float pitchAngle, rollAngle, yawAngle;
float gyroMagnitude, accelMagnitude;

void calculateAccelMagnitude();
void calculateGyroMagnitude();
void calculateAngleDeviations();

public:

IMU(uint8_t slave_addr);

void getAngleAdjustments(float* angleAdjustments);
void IMUInit(const uint8_t* reg_addrs, const uint8_t* initialValues);
void readAccelData(uint8_t reg_start_addr);
void readGyroData(uint8_t reg_start_addr);
void writeIMUData(uint8_t reg_addr, int value);
void getGyroValues(int* gyroValues);
void getAccelValues(int* accelValues);
void calibrateIMU(uint8_t gyroOffsetStartAddress, uint8_t accelOffsetStartAddress);
};


#endif /* MPU9250_H_ */


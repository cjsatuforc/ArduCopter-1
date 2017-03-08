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

public:

IMU();

void MPUInit(uint8_t slave_addr, uint8_t* reg_addrs, uint8_t* initialValues);
void readAccelData(uint8_t slave_addr, uint8_t reg_start_addr);
void readGyroData(uint8_t slave_addr, uint8_t reg_start_addr);
void writeMPUDATA(uint8_t slave_addr, uint8_t reg_addr, int value);
int* getGyroValues(int* gyroValues);
int* getAccelValues(int* accelValues);
};


#endif /* MPU9250_H_ */


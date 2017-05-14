/*
 * PID.h
 *
 *  Created on: Mar 8, 2017
 *      Author: tdawson
 */

#ifndef PID_H_
#define PID_H_

#include "MPU9250.h"

void calculatePIDOutput(float* pidRoll, float* pidPitch, float* pidYaw, IMU& gyroValues);
void setPIDValues(float* pValues, float* iValues, float* dValues);

#endif /* PID_H_ */


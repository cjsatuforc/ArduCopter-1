
#include "PID.h"
#include "RCControl.h"


static float pidValues[3][3];


void calculatePIDOutput(float* pidRoll, float* pidPitch, float* pidYaw, IMU& gyroValues) {

  // array to store the gyro values
  int gyroValues[3];
  float* pidSetPoints = calculatePIDSetPoints(gyroValues);
  
  // local static variables hold the previous errors and the total I components
  static int pidITotal[3] = {0};
  static int lastRollError = 0, lastPitchError = 0, lastYawError = 0;
 
   gyro.getGyroValues(gyroValues);
  //------------------------------------------------ Roll Calculations------------------------------------------------------------------------------------
  int rollError = gyroValues[ROLL_VALUE] - pidSetPoints[ROLL_VALUE];
  pidITotal[ROLL_VALUE] += (pidValues[ROLL_VALUE][INTEGRAL_VALUE] * rollError);

  *pidRoll = (pidValues[ROLL_VALUE][PROP_VALUE] * rollError) + pidITotal[ROLL_VALUE] + (pidValues[ROLL_VALUE][DERIVATIVE_VALUE] * (rollError - lastRollError));

  //-------------------------------------------------Pitch Calculations-----------------------------------------------------------------------------------
  int pitchError = gyroValues[PITCH_VALUE] - pidSetPoints[PITCH_VALUE];
  pidITotal[PITCH_VALUE] += (pidValues[PITCH_VALUE][INTEGRAL_VALUE] * pitchError);

  *pidPitch = (pidValues[PITCH_VALUE][PROP_VALUE] * rollError) + pidITotal[PITCH_VALUE] + (pidValues[PITCH_VALUE][DERIVATIVE_VALUE] * (pitchError - lastPitchError));


  //-------------------------------------------------Yaw Calculations-------------------------------------------------------------------------------------
  int yawError = gyroValues[YAW_VALUE] - pidSetPoints[YAW_VALUE];
  pidITotal[2] += (pidValues[YAW_VALUE][INTEGRAL_VALUE] * yawError);

  *pidRoll = (pidValues[YAW_VALUE][PROP_VALUE] * yawError) + pidITotal[YAW_VALUE] + (pidValues[YAW_VALUE][DERIVATIVE_VALUE] * (yawError - lastYawError));
}

// calculate PID set points using the receiver input and the angle correction from the imu. returns an array of set points.

static float* calculatePIDSetPoints(IMU &imuData) {
	
	float pidSetPoints[3] = {0};
	float receiverInput[4];
	float* angleAdjustments = imuData.getAngleAdjustments();
	for(int i = 0; i< NUM_CHANNELS; i++) {
		
		receiverInput[i] = convertReceiverInput(i+1);
	
	}
	
	for(int i =0; i < NUM_CHANNEL-1; i++) {
		
		if(i == YAW_VALUE) {
			
			if(receiverInput[i] > ESC_MOTORS_OFF_PULSE) {
				
				if(receiverInput[i] > UPPER_PID_CENTER_LIMIT) pidSetPoints[i] = receiverInput[i+1] - UPPER_PID_CENTER_LIMIT ;
				else if(receiverInput[i] < LOWER_PID_CENTER_LIMIT) pidSetPoints[i] = receiverInput[i+1] - LOWER_PID_CENTER_LIMIT;
				break;
			}
		}
		
		if(receiverInput[i] > UPPER_PID_CENTER_LIMIT) pidSetPoints[i] = receiverInput[i] - UPPER_PID_CENTER_LIMIT - angleAdjustments[i];
		else if(receiverInput[i] < LOWER_PID_CENTER_LIMIT) pidSetPoints[i] = receiverInput[i] - LOWER_PID_CENTER_LIMIT - angleAdjustments[i];
	}
	return pidSetPoints;
}

// set the pid gains
void setPIDValues(const float* pValues, const float* iValues, const float* dValues){

  for(int i = 0; i < 3; i++) {

    pidValues[i][0] = pValues[i];
    pidValues[i][1] = iValues[i];
    pidValues[i][2] = dValues[i];
  }
}

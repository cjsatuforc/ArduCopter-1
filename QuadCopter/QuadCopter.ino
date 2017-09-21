 #include "MPU9250.h"
 #include "RCControl.h"
 #include "PID.h"
 #include "ESC.h"
 #include <Wire.h>

 
  // Declare the IMU variable:
  IMU imu(IMU_SLAVE_ADDRESS);
  int batteryVoltage;
  unsigned long loopTimer;
  
  void setup() {
    
  Wire.begin();

 // Atmega pins default to inputs, so they don't need to be explicitly declared as inputs.
  DDRD |= 0xF0;                                                        //Configure digital poort 4, 5, 6 and 7 as output.
  DDRB |= 0x30;                                                        //Configure digital poort 12 as output.
  digitalWrite(12,HIGH); // Turn on led to indicate startup procedure
    
  imu.IMUInit(registerAddresses, MPUInitValues);
  imu.calibrateIMU(GYRO_X_OFFSET_H, ACCEL_X_OFFSET_H);
  
  PCICR |= (1 << PCIE0);                 //Set PCIE0 to enable PCMSK0 scan.
  PCMSK0 |= (1 << PCINT0);               //Set PCINT0 (digital input 8) to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT1);              //Set PCINT1 (digital input 9)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT2);             //Set PCINT2 (digital input 10)to trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT3);           //Set PCINT3 (digital input 11)to trigger an interrupt on state change.

  calibrateRC();                    // perform RC calibration routire with sticks
  
  //The variable battery_voltage holds 1050 if the battery voltage is 10.5V. (TODO: REMOVE MAGIC NUMBERS)
  batteryVoltage = (analogRead(0) + 65) * 1.2317; 
  loopTimer = micros();

  digitalWrite(12, LOW); //indicate that startup has completed
}

void loop() {

  static bool start = false;
  if(!start) 
    start = checkForStartCond();
   else  
   start = checkForStopCond();
   
  if(start) {

    static float pidRoll, pidPitch, pidYaw;
    
    digitalWrite(13,HIGH); //turn on LED to indicate start of quadcopter

  //read data from the IMU:
    imu.readAccelData(ACCEL_XOUT_H);
    imu.readGyroData(GYRO_XOUT_H);

   //calculate pid output:
   calculatePIDOutput(pidRoll, pidPitch, pidYaw, imu);

   //calcuate ESC Pulse and activate ESCs:
   calculateEscPulses(batteryVoltage, pidRoll, pidPitch, pidYaw);    
  }

  else {

      //set 1000us pulse to keep the ESCs from beeping
      int escPulses[NUM_ESC];
      for(int i = 0; i < NUM_ESC; i++)
        escPulses[i] = 1000;

      setEscValues(escPulses);
  }
     //activate the ESCs
     activateESCs(loopTimer);
     
  //a compilmentary filter is used to reduce noise
  batteryVoltage = batteryVoltage * 0.92 + (analogRead(0) + 65) * 0.09853;

  //turn on LED 13 if battery is too low
  if(batteryVoltage < 1000 && batteryVoltage > 600) digitalWrite(13, HIGH);
  
  }
  

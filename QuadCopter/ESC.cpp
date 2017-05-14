#include "ESC.h"
#include "Arduino.h"
#include "Common.h"
#include "ESC_driver.h"

static int escPulseValues[NUM_ESC];
static unsigned long escChannelTimers[NUM_ESC];
static unsigned long escLoopTimer;

void calculateEscPulses(int& throttleValue, int batteryVoltage, float pidRoll, float pidPitch, float pidYaw) {

  if (throttleValue > MAX_THROTTLE) throttleValue = MAX_THROTTLE;

  for (int i = 0; i < NUM_ESC; i++) {

    switch (i) {

      case ESC_FRONT_RIGHT: escPulseValues[i] = throttleValue - pidPitch + pidRoll - pidYaw;

      case ESC_FRONT_LEFT: escPulseValues[i] = throttleValue + pidPitch + pidRoll + pidYaw;

      case ESC_REAR_RIGHT: escPulseValues[i] = throttleValue + pidPitch - pidRoll - pidYaw;

      case ESC_REAR_LEFT: escPulseValues[i] = throttleValue - pidPitch - pidRoll + pidYaw;
    }

    // if battery is connected and active, compensate for voltage drop
    if (batteryVoltage < MAX_BATTERY_VOLTAGE && batteryVoltage > MIN_BATTERY_VOLTAGE) escPulseValues[i] += escPulseValues[i] * (MAX_BATTERY_VOLTAGE - batteryVoltage) / (float)3500;

    if (escPulseValues[i] < ESC_MOTORS_ON_PULSE) escPulseValues[i] = ESC_MOTORS_ON_PULSE;
    else if (escPulseValues[i] > MAX_ESC_PULSE) escPulseValues[i] = MAX_ESC_PULSE;
  }
}

void disableESCs() {

  for (int i = 0; i < NUM_ESC; i++) escPulseValues[i] = ESC_MOTORS_OFF_PULSE;

}

const int* getEscPulses() {
  return escPulseValues;
}

void activateESCs(unsigned long &programLoopTimer) {

  while ((micros() - *programLoopTimer) > ESC_PULSE_PERIOD); // wait for 4000us to pass before applying the pulse
  programLoopTimer = micros();
  for (int i = 0; i < NUM_ESC; i++) {
    driveESC(i);
    escChannelTimers[i] = escPulseValues[i] + programLoopTimer;
  }

  while (escEnabled() != ALL_ESC_DISABLED) {

    escLoopTimer = micros();
    for (int i = 0; i < NUM_ESC; i++) {

      if ( escChannelTimers[i] <= escLoopTimer) stopEsc(i);
    }
  }
}

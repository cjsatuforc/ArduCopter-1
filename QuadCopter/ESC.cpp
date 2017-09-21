#include "ESC.h"
#include "Arduino.h"
#include "Common.h"
#include "RCControl.h"

static int escPulseValues[NUM_ESC];
static unsigned long escChannelTimers[NUM_ESC];
static unsigned long escLoopTimer;
static int escEnabled();

void calculateEscPulses(int batteryVoltage, float pidRoll, float pidPitch, float pidYaw) {

  int throttleValue = convertRecieverInput(THROTTLE_CHANNEL);
  
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

void setEscValues(int (&escValues)[NUM_ESC]) {

  for (int i = 0; i < NUM_ESC; i++)
    escPulseValues[i] = escValues[i]; 
}

void activateESCs(unsigned long& programLoopTimer) {

  while ((micros() - programLoopTimer) > ESC_PULSE_PERIOD); // wait for 4000us to pass before applying the pulse
  programLoopTimer = micros();
  for (int i = 0; i < NUM_ESC; i++) {
    PORTD |= ( 1 << (i+NUM_ESC));
    escChannelTimers[i] = escPulseValues[i] + programLoopTimer;
  }

  while (escEnabled() != ALL_ESC_DISABLED) {

    escLoopTimer = micros();
    for (int i = 0; i < NUM_ESC; i++) {

      if ( escChannelTimers[i] <= escLoopTimer) PORTD &= ~(1 << (i+NUM_ESC));
    }
  }
}

static int escEnabled() {

  const int* escPulseValues = getEscPulses();
    int enabledEscs = 0;
    
    for(int i = 0; i < NUM_ESC; i++) {

      if(escPulseValues[i] != ESC_MOTORS_OFF_PULSE)
        enabledEscs |= (1 << i);
    }

  return enabledEscs;
}


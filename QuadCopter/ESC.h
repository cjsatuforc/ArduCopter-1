#ifndef ESC_H_
#define ESC_H_

void calculateEscPulses(int& throttleValue, int batteryVoltage, float pidRoll, float pidPitch, float pidYaw);
const int* getEscPulses();
void disableESCs();
void setEscValues(int (&escValues)[NUM_ESC]);
void activateESCs(unsigned long &programLoopTimer);



#endif // ESC_H_

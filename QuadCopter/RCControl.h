#ifndef RCCONTROL_H_
#define RCCONTROL_H_

int convertRecieverInput(int channel);
void calibrateRC();
bool checkForStartCond();
bool checkForStopCond();

#endif // RCCONTROL_H_

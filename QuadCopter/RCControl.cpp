#include "RCControl.h"
#include "Arduino.h"

static int receiverInput[4];
static int receiverCenterValues[4];
static int receiverLowValues[4];
static int receiverHighValues[4];

void calibrateRC() {


  
}

int convertReceiverInput(int channel) {
  //First we declare some local variables
  int actual, difference;


  actual = receiverInput[channel - 1];                                         
  if (actual < receiverCenterValues[channel -1]) {                                                       //The actual receiver value is lower than the center value
    if (actual < receiverLowValues[channel -1])actual = receiverLowValues[channel -1];                                             //Limit the lowest value to the value that was detected during setup
    difference = ((long)(receiverCenterValues[channel -1] - actual) * (long)500) / (receiverCenterValues[channel -1] - receiverLowValues[channel-1]);       //Calculate and scale the actual value to a 1000 - 2000us value
    return 1500 - difference;
  }
  else if (actual > receiverCenterValues[channel-1]) {                                                                      //The actual receiver value is higher than the center value
    if (actual > receiverHighValues[channel-1])actual = receiverHighValues[channel-1];                                           //Limit the lowest value to the value that was detected during setup
    difference = ((long)(actual - receiverCenterValues[channel-1]) * (long)500) / (receiverHighValues[channel-1] - receiverCenterValues[channel-1]);      //Calculate and scale the actual value to a 1000 - 2000us value
    return 1500 + difference;
  }
  return 1500;
}

ISR(PCINT0_vect){
static long timer_1, timer_2, timer_3, timer_4, current_time;
static unsigned char last_channel_1, last_channel_2, last_channel_3, last_channel_4;

current_time = micros();
  //Channel 1=========================================
  if(PINB & B00000001){                                        //Is input 8 high?
    if(last_channel_1 == 0){                                   //Input 8 changed from 0 to 1
      last_channel_1 = 1;                                      //Remember current input state
      timer_1 = current_time;                                  //Set timer_1 to current_time
    }
  }
  else if(last_channel_1 == 1){                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    receiverInput[0] = current_time - timer_1;                //Channel 1 is current_time - timer_1
  }
  //Channel 2=========================================
  if(PINB & B00000010 ){                                       //Is input 9 high?
    if(last_channel_2 == 0){                                   //Input 9 changed from 0 to 1
      last_channel_2 = 1;                                      //Remember current input state
      timer_2 = current_time;                                  //Set timer_2 to current_time
    }
  }
  else if(last_channel_2 == 1){                                //Input 9 is not high and changed from 1 to 0
    last_channel_2 = 0;                                        //Remember current input state
    receiverInput[1] = current_time - timer_2;                //Channel 2 is current_time - timer_2
  }
  //Channel 3=========================================
  if(PINB & B00000100 ){                                       //Is input 10 high?
    if(last_channel_3 == 0){                                   //Input 10 changed from 0 to 1
      last_channel_3 = 1;                                      //Remember current input state
      timer_3 = current_time;                                  //Set timer_3 to current_time
    }
  }
  else if(last_channel_3 == 1){                                //Input 10 is not high and changed from 1 to 0
    last_channel_3 = 0;                                        //Remember current input state
    receiverInput[2] = current_time - timer_3;                //Channel 3 is current_time - timer_3

  }
  //Channel 4=========================================
  if(PINB & B00001000 ){                                       //Is input 11 high?
    if(last_channel_4 == 0){                                   //Input 11 changed from 0 to 1
      last_channel_4 = 1;                                      //Remember current input state
      timer_4 = current_time;                                  //Set timer_4 to current_time
    }
  }
  else if(last_channel_4 == 1){                                //Input 11 is not high and changed from 1 to 0
    last_channel_4 = 0;                                        //Remember current input state
    receiverInput[3] = current_time - timer_4;                //Channel 4 is current_time - timer_4
  }
}


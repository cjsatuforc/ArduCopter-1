#include "SerialLogger.h"
#include "Arduino.h"


SerialLogger* SerialLogger::instance = NULL;

SerialLogger::SerialLogger(uint32_t BaudRate) {

  enabled = true;
  Serial.begin(BaudRate);
}

SerialLogger* SerialLogger::getInstance(uint32_t BaudRate) {

  if (!instance) instance = new SerialLogger(BaudRate);
    return instance; 
}

void SerialLogger::enable() {
   enabled = true;
}

void SerialLogger::disable() {

  enabled = false;
}

void SerialLogger::logToConsole(const char* logString) {

  if(enabled) {
    Serial.print(logString);
  }
}


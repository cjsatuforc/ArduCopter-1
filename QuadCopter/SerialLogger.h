
#ifndef SerialLogger_H_
#define SerialLogger_H_

#include <stdint.h>

class SerialLogger {

    static SerialLogger* instance;
    bool enabled;
    SerialLogger(uint32_t BaudRate);

  public:

    static SerialLogger* getInstance(uint32_t BaudRate);
    void enable();
    void disable();
    void logToConsole(const char* logString);
    static void deleteInstance();
};

#endif // SerialLogger_H_

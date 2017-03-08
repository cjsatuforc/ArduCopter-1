#include "I2CHelper.h"

#include <Wire.h>

void I2CInit() {

  Wire.begin();

}
void i2c_readFrom(uint8_t slave_addr, uint8_t reg_addr, byte* Buffer) {

  int returnFromSlave;
  Wire.beginTransmission(slave_addr);
  Wire.write(byte(reg_addr));
  Wire.endTransmission();
  returnFromSlave = Wire.requestFrom(slave_addr, sizeof(Buffer));
  memset(Buffer, 0, sizeof(Buffer));
         int i = 0;
  while (Wire.available()) {
  Buffer[i] = Wire.read();
    //    Serial.print((int)Buffer[i]);
    //    Serial.print("--");
    i++;
  }
}

void i2c_writeTo(uint8_t slave_addr, uint8_t reg_addr, int val) {

  Wire.beginTransmission(slave_addr);
  Wire.write((byte)reg_addr);
  Wire.write(val);
  Wire.endTransmission();
}

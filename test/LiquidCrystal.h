#ifndef LYQUIDCRYSTAL_H_
#define LYQUIDCRYSTAL_H_

#include "arduino_stubs.h"

// TODO da fare

class LiquidCrystal {
public:

  LiquidCrystal(const int16_t& RS, const int16_t& E, const int16_t& DB4, const int16_t& DB5, const int16_t& DB6, const int16_t& DB7) {
  }
  
  void begin(int col, int row) { }
  
  void createChar (byte b, byte data[]) { }
  
  void clear() { }
    
  void setCursor(int col, int row) { }
  
  void print(std::string str) { }
  
  void print(int d) { }

  void write(byte c) { }

};

#endif // LYQUIDCRYSTAL_H_

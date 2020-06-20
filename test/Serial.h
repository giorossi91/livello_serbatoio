#ifndef SERIAL_H_
#define SERIAL_H_

#include "arduino_types.h"
#include "arduino_stubs.h"

class SerialClass {
public:

  void begin ( int32_t baudrate ) {
    ( void ) baudrate;
  }
  
  void print ( String str ) {
    std::cout << str;
  }
  
  void print ( double n ) {
    std::cout << std::to_string(n);
  }
  
  void println ( double n ) {
    print ( n );
    std::cout << std::endl;
  }
  
  void println ( String str ) {
    print ( str );
    std::cout << std::endl;
  }
};

SerialClass Serial;

#endif // SERIAL_H_

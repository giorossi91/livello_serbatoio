#include "Serial.h"

SerialClass Serial;

void SerialClass::begin ( int32_t baudrate ) {
  ( void ) baudrate;
}

void SerialClass::print ( String str ) {
  emit printSerialText(str);

  std::cout << str << std::flush;
}

void SerialClass::print ( double n ) {
  emit printSerialText(std::to_string(n));

  std::cout << std::to_string(n) << std::flush;
}

void SerialClass::println ( double n ) {
  print ( n );

  emit printSerialText("\n");

  std::cout << std::endl << std::flush;
}

void SerialClass::println ( String str ) {
  print ( str );

  emit printSerialText("\n");

  std::cout << std::endl << std::flush;
}

#ifndef SERIAL_H_
#define SERIAL_H_

#include "arduino_types.h"
#include "arduino_stubs.h"

#include <QObject>

class SerialClass : public QObject {
    Q_OBJECT

public:

  void begin ( int32_t baudrate ) {
    ( void ) baudrate;
  }
  
  void print ( String str ) {
    emit printSerialText(str);

    std::cout << str << std::flush;
  }
  
  void print ( double n ) {
    emit printSerialText(std::to_string(n));

    std::cout << std::to_string(n) << std::flush;
  }
  
  void println ( double n ) {
    print ( n );

    emit printSerialText("\n");

    std::cout << std::endl << std::flush;
  }
  
  void println ( String str ) {
    print ( str );

    emit printSerialText("\n");

    std::cout << std::endl << std::flush;
  }

signals:
  void printSerialText(std::string text);
};

extern SerialClass Serial;

#endif // SERIAL_H_

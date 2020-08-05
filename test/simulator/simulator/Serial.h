#ifndef SERIAL_H_
#define SERIAL_H_

#include "arduino_types.h"
#include "arduino_stubs.h"

#ifdef QT_CORE_LIB
#include <QObject>
#endif

class SerialClass
#ifdef QT_CORE_LIB
        : public QObject {
    Q_OBJECT
#else
{
#endif

public:

  void begin ( int32_t baudrate ) {
    ( void ) baudrate;
  }
  
  void print ( String str ) {
#ifdef QT_CORE_LIB
    emit printSerialText(str);
#else
    std::cout << str;
#endif
  }
  
  void print ( double n ) {
#ifdef QT_CORE_LIB
    emit printSerialText(std::to_string(n));
#else
    std::cout << std::to_string(n);
#endif
  }
  
  void println ( double n ) {
    print ( n );

#ifdef QT_CORE_LIB
    emit printSerialText("\n");
#else
    std::cout << std::endl;
#endif
  }
  
  void println ( String str ) {
    print ( str );
#ifdef QT_CORE_LIB
    emit printSerialText("\n");
#else
    std::cout << std::endl;
#endif
  }

#ifdef QT_CORE_LIB
signals:
  void printSerialText(std::string text);
#endif
};

extern SerialClass Serial;

#endif // SERIAL_H_

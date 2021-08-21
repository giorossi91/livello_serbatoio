#ifndef SERIAL_H_
#define SERIAL_H_

#include "arduino_types.h"
#include "arduino_stubs.h"

#include <QObject>

class SerialClass : public QObject {
    Q_OBJECT

public:

  void begin ( int32_t baudrate );
  
  void print ( String str );
  
  void print ( double n );
  
  void println ( double n );
  
  void println ( String str );

signals:
  void printSerialText(std::string text);
};

extern SerialClass Serial;

#endif // SERIAL_H_

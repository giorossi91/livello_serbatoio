#ifndef LYQUIDCRYSTAL_H_
#define LYQUIDCRYSTAL_H_

#include <QObject>
#include <QString>
#include <QMap>

#include "arduino_types.h"
#include "arduino_stubs.h"

#include <sstream>

class LiquidCrystal : public QObject {
    Q_OBJECT

public:

  LiquidCrystal ( int16_t RS, int16_t E, int16_t DB4, int16_t DB5, int16_t DB6, int16_t DB7 );

  void begin ( int32_t col, int32_t row );

  void createChar ( byte b, byte data[] );

  void clear ( void );

  void setCursor ( int32_t col, int32_t row );

  void print ( const String &str, bool pout = true );

  void print ( const char *pStr, bool pout = true );

  void print ( int32_t d, bool pout = true );

  void write ( byte c );

  static void harness_defaultPrintFail ( const char * str );

  std::string harness_getLcdText ( void );

  void harness_printLcd ( void );

  void harness_setPrintFailFunction ( printFailFunc func = harness_defaultPrintFail );

  void harness_mapSymbol(const byte index, const String& c);


private:
  static constexpr int32_t LCD_COLS_MAX_SIZE = 50;
  static constexpr int32_t LCD_ROWS_MAX_SIZE = 10;

  char pLcdMatrix[LCD_ROWS_MAX_SIZE][LCD_COLS_MAX_SIZE];

  int32_t cols;
  int32_t rows;

  int32_t cursor_col;
  int32_t cursor_row;

  printFailFunc printFailFunction;

  QMap<byte, String> specialSymbolsMap;

signals:
  void printTextOnLcd(std::string text);

};

#endif // LYQUIDCRYSTAL_H_

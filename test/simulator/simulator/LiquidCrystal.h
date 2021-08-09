#ifndef LYQUIDCRYSTAL_H_
#define LYQUIDCRYSTAL_H_

#ifdef QT_CORE_LIB
#include <QObject>
#include <QString>
#undef CPPUNIT_FAIL
#define CPPUNIT_FAIL(s) qFatal(s);
#else
#include <cppunit/extensions/HelperMacros.h>
#endif


#include "arduino_types.h"
#include "arduino_stubs.h"

#include <sstream>

class LiquidCrystal
#ifdef QT_CORE_LIB
: public QObject {
    Q_OBJECT
#else
{
#endif

public:

  LiquidCrystal ( int16_t RS, int16_t E, int16_t DB4, int16_t DB5, int16_t DB6, int16_t DB7 ) {
    ( void ) RS;
    ( void ) E;
    ( void ) DB4;
    ( void ) DB5;
    ( void ) DB6;
    ( void ) DB7;
    
    memset(pLcdMatrix, 0, LCD_COLS_MAX_SIZE * LCD_ROWS_MAX_SIZE);
    
    cols = LCD_COLS_MAX_SIZE;
    rows = LCD_ROWS_MAX_SIZE;
    cursor_col = 0;
    cursor_row = 0;
  }
  
  void begin ( int32_t col, int32_t row ) {
    if( ( ( col >= 0 ) && ( col <= LCD_COLS_MAX_SIZE ) ) && ( ( row >= 0 ) && ( row <= LCD_ROWS_MAX_SIZE ) ) ) {  
  
      cols = col;
      rows = row;
    
      for (int32_t i = 0; i < rows; i++) {
        for (int32_t j = 0; j < cols; j++) {
          pLcdMatrix[i][j] = ' ';      
        }
      }
      
      cursor_col = 0;
      cursor_row = 0;
    } else {
      CPPUNIT_FAIL("Exceeded maximum LCD sizes");
    }
  }
  
  void createChar ( byte b, byte data[] ) {
    ( void ) b;
    ( void ) data;
  }
  
  void clear ( void ) {
    for (int32_t i = 0; i < rows; i++) {
      for (int32_t j = 0; j < cols; j++) {
        pLcdMatrix[i][j] = ' ';      
      }
    }
      
    cursor_col = 0;
    cursor_row = 0;
  }
    
  void setCursor ( int32_t col, int32_t row ) {
    if ( ( ( col < 0 ) || ( col >= cols ) ) || ( ( row < 0 ) || ( row >= rows ) ) ) {
      std::stringstream ss_fail;
    
      ss_fail << "Setting LCD cursor out of bounds " << "[ r=" << row << ", c=" << col << "]." << 
        " Maximum: [ r=" << rows << ", c=" << cols << "]";

      CPPUNIT_FAIL(ss_fail.str().c_str())
    }   
  
    cursor_col = col;
    cursor_row = row;
  }
  
  void print ( String &str, bool pout = true ) {
    print(str.c_str(), pout);
  }
  
  void print ( const char *pStr, bool pout = true ) {
    if ( pStr != nullptr ) {
      uint32_t i = 0U;
      while ( pStr[i] != '\0' ) {
      
        if( cursor_col >= cols ) {
          CPPUNIT_FAIL ( "LCD cols limit exceeded.")
        } 
      
        pLcdMatrix[cursor_row][cursor_col] = pStr[i];
        
        cursor_col++;
        i++;
      }
      
      if( pout == true ) {
        printLcd();
      }
      
    }
  }
  
  void print ( int32_t d, bool pout = true ) {
    print ( std::to_string ( d ).c_str(), pout );
  }

  void write ( byte c ) {
    if ( c == 1 ) {
      print("#", true);
    } else if ( c == 0 ) {
      print("^", true);
     } else {
      print("$", true);
    }
  }
  
  std::string harness_getLcdText ( void ) {
    std::string lcdText = "";
    for(int32_t i = 0; i < rows; i++) {
      for(int32_t j = 0; j < cols; j++) {
        lcdText.push_back( pLcdMatrix[i][j] );
      }
      
      if ( i != ( rows - 1 ) ) {
        lcdText.push_back( '\n' );
      }
    }
    
    return lcdText;
  }
  
  void printLcd ( void ) {
    std::stringstream ss;
#ifndef QT_CORE_LIB
    ss << std::endl;
#endif

    for (int32_t i = 0; i < cols; i++) {
      ss << "=";
    }
    ss << std::endl;
    ss << harness_getLcdText();
    ss << std::endl;
    for (int32_t i = 0; i < cols; i++) {
      ss << "=";
    }

#ifdef QT_CORE_LIB
    emit printTextOnLcd(ss.str());
#else
    ss << std::endl;
    std::cout << ss.str();
#endif
  }
  
private:
  static const int32_t LCD_COLS_MAX_SIZE = 50;
  static const int32_t LCD_ROWS_MAX_SIZE = 10;
  
  char pLcdMatrix[LCD_ROWS_MAX_SIZE][LCD_COLS_MAX_SIZE];
  
  int32_t cols;
  int32_t rows;
  
  int32_t cursor_col;
  int32_t cursor_row;

#ifdef QT_CORE_LIB
signals:
  void printTextOnLcd(std::string text);
#endif
};

#endif // LYQUIDCRYSTAL_H_

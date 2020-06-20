#ifndef LYQUIDCRYSTAL_H_
#define LYQUIDCRYSTAL_H_

#include "arduino_types.h"
#include "arduino_stubs.h"


class LiquidCrystal {
public:

  LiquidCrystal(const int16_t& RS, const int16_t& E, const int16_t& DB4, const int16_t& DB5, const int16_t& DB6, const int16_t& DB7) {
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
    
  void setCursor(int col, int row) {
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
        pLcdMatrix[cursor_row][cursor_col++] = pStr[i++];
      }
      
      if( pout == true ) {
        printLcd();
      }
      
    }
  }
  
  void print(int d, bool pout = true) {
    print ( std::to_string ( d ).c_str(), pout );
  }

  void write(byte c) {
    if ( c == 1 ) {
      print("#", true);
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
    std::cout << std::endl;
    for (int32_t i = 0; i < cols; i++) {
      std::cout << "=";
    }
    std::cout << std::endl;
    std::cout << harness_getLcdText();
    std::cout << std::endl;
    for (int32_t i = 0; i < cols; i++) {
      std::cout << "=";
    }
    std::cout << std::endl;
    
  }
  
private:
  static const int32_t LCD_COLS_MAX_SIZE = 50U;
  static const int32_t LCD_ROWS_MAX_SIZE = 10U;
  
  char pLcdMatrix[LCD_ROWS_MAX_SIZE][LCD_COLS_MAX_SIZE];
  
  int32_t cols;
  int32_t rows;
  
  int32_t cursor_col;
  int32_t cursor_row;
  
  
};

#endif // LYQUIDCRYSTAL_H_

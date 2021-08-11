#include "LiquidCrystal.h"

LiquidCrystal::LiquidCrystal ( int16_t RS, int16_t E, int16_t DB4, int16_t DB5, int16_t DB6, int16_t DB7 ) {
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

  printFailFunction = harness_defaultPrintFail;
}

void LiquidCrystal::harness_defaultPrintFail ( const char * str ) {
    std::cerr << str << std::endl;
}

void LiquidCrystal::begin ( int32_t col, int32_t row ) {
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
    printFailFunction("Exceeded maximum LCD sizes");
  }
}

void LiquidCrystal::createChar ( byte b, byte data[] ) {
  ( void ) b;
  ( void ) data;
}

void LiquidCrystal::clear ( void ) {
  for (int32_t i = 0; i < rows; i++) {
    for (int32_t j = 0; j < cols; j++) {
      pLcdMatrix[i][j] = ' ';
    }
  }

  cursor_col = 0;
  cursor_row = 0;
}

void LiquidCrystal::setCursor ( int32_t col, int32_t row ) {
  if ( ( ( col < 0 ) || ( col >= cols ) ) || ( ( row < 0 ) || ( row >= rows ) ) ) {
    std::stringstream ss_fail;

    ss_fail << "Setting LCD cursor out of bounds " << "[ r=" << row << ", c=" << col << "]." <<
      " Maximum: [ r=" << rows << ", c=" << cols << "]";

    printFailFunction(ss_fail.str().c_str());
  }

  cursor_col = col;
  cursor_row = row;
}

void LiquidCrystal::print ( String &str, bool pout ) {
  print(str.c_str(), pout);
}

void LiquidCrystal::print ( const char *pStr, bool pout ) {
  if ( pStr != nullptr ) {
    uint32_t i = 0U;
    while ( pStr[i] != '\0' ) {

      if( cursor_col >= cols ) {
        printFailFunction ( "LCD cols limit exceeded.");
      }

      pLcdMatrix[cursor_row][cursor_col] = pStr[i];

      cursor_col++;
      i++;
    }

    if( pout == true ) {
      harness_printLcd();
    }

  }
}

void LiquidCrystal::print ( int32_t d, bool pout ) {
  print ( std::to_string ( d ).c_str(), pout );
}

void LiquidCrystal::write ( byte c ) {
  if ( c == 1 ) {
    print("#", true);
  } else {
    print("$", true);
  }
}

std::string LiquidCrystal::harness_getLcdText ( void ) {
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

void LiquidCrystal::harness_printLcd ( void ) {
  std::stringstream ss;

  for (int32_t i = 0; i < cols; i++) {
    ss << "=";
  }

  ss << std::endl;
  ss << harness_getLcdText();
  ss << std::endl;

  for (int32_t i = 0; i < cols; i++) {
    ss << "=";
  }

  emit printTextOnLcd(ss.str());

  ss << std::endl;

  std::cout << ss.str() << std::flush;
}

void LiquidCrystal::harness_setPrintFailFunction ( printFailFunc func ) {
    this->printFailFunction = func;
}

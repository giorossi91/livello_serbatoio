#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int set_interface_attribs ( int fd, int speed ) {
  int            retCode = 0;
  struct termios tty     = { 0 };

  if ( tcgetattr(fd, &tty) < 0 ) {
    fprintf(stderr, "Error from tcgetattr: %s\n", strerror(errno));
    retCode = -1;
  } else {

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         // 8-bit characters
    tty.c_cflag &= ~PARENB;     // no parity bit
    tty.c_cflag &= ~CSTOPB;     // only need 1 stop bit
    tty.c_cflag &= ~CRTSCTS;    // no hardware flowcontrol

    // setup for non-canonical mode
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    // fetch bytes as they become available
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if ( tcsetattr(fd, TCSANOW, &tty) != 0 ) {
      fprintf(stderr, "Error from tcsetattr: %s\n", strerror(errno));
      retCode = -2;
    }
  }
  return retCode;
}

void set_mincount ( int fd, int mcount ) {
  struct termios tty = { 0 };

  if ( tcgetattr(fd, &tty) < 0 ) {
    fprintf(stderr, "Error tcgetattr: %s\n", strerror(errno));
  } else {
    tty.c_cc[VMIN]  = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;  // half second timer

    if ( tcsetattr(fd, TCSANOW, &tty) < 0 ) {
      fprintf(stderr, "Error tcsetattr: %s\n", strerror(errno));
    }
  }
}

char getCharToSend ( void ) {
  static unsigned char i = 16;
  
  if (i == 125) {
    i = 16;
  }
  
  return (i++);
}

int testBlockW ( int fd, unsigned int nbytes ) {
  int           status = 0;
  unsigned int  i      = 0U;
  unsigned char *bytesW = malloc ( nbytes );
  
  ( void ) memset ( bytesW, 0, nbytes );
   
  while ( i < nbytes ) {
      bytesW [ i++ ] = getCharToSend(); 
  }
   
  ssize_t wlen = write ( fd, bytesW, nbytes );
  if ( wlen != nbytes ) {
      fprintf ( stderr, "Error from write: %ld, %d\n", wlen, errno );
      status = 1;
  }
   
  tcdrain ( fd ); // delay for output
  
  free ( bytesW );
   
  return status;
}

int testBlockR ( int fd, unsigned int nbytes ) {
  int           status = 0;
  unsigned int  i      = 0U;
  unsigned char *bytesR = malloc ( nbytes );
  
  ( void ) memset ( bytesR, 0, nbytes );
  
  ssize_t rlen = read ( fd, bytesR, nbytes );
  
  while ( i < rlen ) {
    fprintf ( stdout, "0x%02x ", ( unsigned char ) bytesR [ i ] );
  }
  
  free ( bytesR );
   
  return status;
}


int main ( int argc, char **argv ) {
  unsigned int       test_index   = 1U;
  const unsigned int test_loops   = 2048U;
  const unsigned int block_size   = 32U;
  unsigned int       errors_count = 0U;
  char               *portname    = NULL;
  int                fd           = 0;
  
  if( argc != 3 ) {
    fprintf(stderr, "Expected port name '/dev/ttyXXX' r|w\n");
    return -1;
  }
  
  portname = argv[1];

  fd = open ( portname, O_RDWR | O_NOCTTY | O_SYNC );
  if ( fd < 0 ) {
    fprintf(stderr, "Error opening %s: %s\n", portname, strerror(errno));
    return -1;
  }
  // baudrate 115200, 8 bits, no parity, 1 stop bit
  set_interface_attribs(fd, B115200);
  //set_mincount(fd, 0);  // set to pure timed read
  
  
  printf("Test Started:\n");
  
  while ( ( test_index++ ) <= test_loops ) {
    if ( argv[2][0] == 'w' ) {
      errors_count += testBlockW ( fd, block_size );
    } else if ( argv[2][0] == 'r' ) {
      errors_count += testBlockR ( fd, block_size );
    } else {
      fprintf ( stderr, "unknown test: %c\n", argv[2][0] );
      errors_count = 1;
      break;
    }
    printf("B%04d \n", test_index++);
  };
  
  printf("\n");
  
  if ( errors_count != 0U ) {
    printf("Test FAILED\nThere was %d errors in Serial communication\n Check your setup\n", errors_count);
  } else {
    printf("Test PASSED\n Written: %d B\n", ( block_size * test_loops ) );
  }
  
  return 0;
}

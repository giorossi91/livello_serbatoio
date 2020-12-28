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
  static unsigned char i = 0;
  
  if (i == 255) {
    i = 0;
  }
  
  return (i++);
}


int main ( int argc, char **argv ) {
  unsigned int       test_index   = 0U;
  const unsigned int test_loops   = 10000U;
  unsigned int       errors_count = 0U;
  char               *portname    = NULL;
  int                fd           = 0;
  
  if( argc != 2 ) {
    fprintf(stderr, "Expected port name '/dev/ttyXXX'\n");
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
  
  // simple noncanonical input
  char charToSend = 0;
  char charRecv   = 0;
  int  wlen       = 0;
  int  rlen       = 0;
  
  unsigned int row = 0;
  
  printf("Test Started:\n%d: ", row++);
  
  do {
    charToSend = getCharToSend();
    
    wlen = write(fd, &charToSend, 1);
    if (wlen != 1) {
      fprintf(stderr, "Error from write: %d, %d\n", wlen, errno);
    }
    tcdrain(fd);    // delay for output
    
    rlen = read(fd, &charRecv, 1);
    if (rlen != 1) {
      fprintf(stderr, "Error from read: %d, %d\n", rlen, errno);
    }
    
    if ( charRecv != charToSend ) {
      errors_count ++;
      printf("X");
    } else {
      printf("%c", charToSend);
    }
    
    if( ( test_index % 10 ) == 0 ) {
      printf("\n%d: ", row++);
    } 
  } while ((++test_index) < test_loops);
  
  printf("\n");
  
  if ( errors_count != 0U ) {
    printf("Test FAILED\nThere was %d errors in Serial communication\n Check your setup\n", errors_count);
  } else {
    printf("Test PASSED\n");
  }
  
  return 0;
}

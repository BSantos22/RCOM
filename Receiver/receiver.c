#include "linklayer.h"
#include "datalayer.h"
#include "stats.h"

int main(int argc, char** argv)
{
    int fd,c, res;
    struct termios oldtio,newtio;
    char rstring[255];
    int nrstring;
    nrstring = 0;
char* buffer;

    if ( (argc < 2) || 
  	     ((strcmp("/dev/ttyS0", argv[1])!=0) && 
  	      (strcmp("/dev/ttyS1", argv[1])!=0) )) {
      printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
      exit(1);
    }
	
	srand(time(NULL));	

	if(argc == 3){
		updateError();	
	}


  /*
    Open serial port device for reading and writing and not as controlling tty
    because we don't want to get killed if linenoise sends CTRL-C.
  */
  
    
    fd = open(argv[1], O_RDWR | O_NOCTTY );
    if (fd <0) {perror(argv[1]); exit(-1); }

    if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
      perror("tcgetattr");
      exit(-1);
    }



    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
    newtio.c_cc[VMIN]     = 1;   /* blocking read until 5 chars received */



  /* 
    VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
    leitura do(s) próximo(s) caracter(es)
  */



    tcflush(fd, TCIOFLUSH);

    if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
      perror("tcsetattr");
      exit(-1);
    }

	int file;

	llopen(fd);

	while(llread(fd, buffer, &file) != 0){
	
	}

   	close(file);
	

	llclose(fd);

	printStats();
	file = open(getFileName(), O_RDONLY);
	struct stat fileStat;
	if(fstat(file, &fileStat) == -1){
		perror("fstat");
		return -1;
	}

	int final_size = fileStat.st_size;

	printf("Received Size:%d bytes\n", final_size);
	printf("Control packet size:%d bytes \n", getCPsize());

    tcsetattr(fd,TCSANOW,&oldtio);
	close(file);
    return 0;
}

#include "my_serial.h"
#include "packet_data_type.h"
#include "packet_comms.h"

char *packet_type_strings[] =
    { "UNINITIALISED",
      "HEARTBEAT",
      "MOTOR_VALUES",
      "PARAMETERS",
      "CONFIGUARTION",
    };


int open_port(void)
{
	struct termios tio;	
  	int tty_fd; /* File descriptor for the port */
	tio.c_iflag=0;
	tio.c_oflag=0;
	tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
	tio.c_lflag=0;
	tio.c_cc[VMIN]=1;
	tio.c_cc[VTIME]=5;

	//tty_fd=open(UART4, O_RDWR | O_NONBLOCK);        // O_NONBLOCK might override VMIN and VTIME, so read() may return immediately.
	tty_fd = open(UART1, O_RDWR );// | O_NOCTTY | O_NDELAY);
	cfsetospeed(&tio,B115200);            // 115200 baud
	//cfsetispeed(&tio,B115200);            // 115200 baud

	tcsetattr(tty_fd,TCSANOW,&tio);
	//fcntl(tty_fd, F_SETFL, 0);
  	//fd = open(, O_RDWR | O_NOCTTY | O_NDELAY);
  	/*if (fd == -1)
	{
		perror("open_port: Unable to open /dev/ttyf1 - ");
		return -1;
	}
	else
		fcntl(fd, F_SETFL, 0);
	*/
	return (tty_fd);
}


int main()
{
	int port=open_port();

	char a,b=0xFE;
	size_t length;
	int m;
	packet_data_t data;

	printf("Starting\n");
	//int n = write(port, "Hello World\r\n", 13);
	//if (n < 0) perror("write failed");


	printf("Start reading\n");
	while(1)
	{
		length=get_heartbeat(&data);
		//printf("read try\n");
		//m=read(port,&length,4);
		//read(port,((&data)+sizeof(size_t)),length-sizeof(size_t));
		print_data_packet(&data);
		//if(m>0) printf("%c",a);
		//else perror("read error");
		//write(port,&b, 1);b++;write(port,"\n\r", 2);
		write(port,&b, 1);
		write(port,&data, length);
		sleep(1);
	}
	close(port);
	printf("Port Closed \n");
}

/*
int main(int argc,char** argv)
{
        
        struct termios stdio;
        int tty_fd;
        fd_set rdset;

        unsigned char c='D';

        printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
        memset(&stdio,0,sizeof(stdio));
        stdio.c_iflag=0;
        stdio.c_oflag=0;
        stdio.c_cflag=0;
        stdio.c_lflag=0;
        stdio.c_cc[VMIN]=1;
        stdio.c_cc[VTIME]=0;
        tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking




        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;

        tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);        // O_NONBLOCK might override VMIN and VTIME, so read() may return immediately.
        cfsetospeed(&tio,B115200);            // 115200 baud
        cfsetispeed(&tio,B115200);            // 115200 baud

        tcsetattr(tty_fd,TCSANOW,&tio);
        while (c!='q')
        {
                if (read(tty_fd,&c,1)>0)        write(STDOUT_FILENO,&c,1);              // if new data is available on the serial port, print it out
                if (read(STDIN_FILENO,&c,1)>0)  write(tty_fd,&c,1);                     // if new data is available on the console, send it to the serial port
        }

        close(tty_fd);
}
*/
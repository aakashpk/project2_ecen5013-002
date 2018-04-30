#include "serial.h"
//#include "packet_data_type.h"
//#include "packet_comms.h"


int open_port(char* port_name)
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
	tty_fd = open(port_name, O_RDWR );// | O_NOCTTY | O_NDELAY);
	if (tty_fd < 0)
	{
		perror("Unable to open ");
		return -1;
	}
	cfsetospeed(&tio,B115200);            // 115200 baud
	//cfsetispeed(&tio,B115200);            // 115200 baud

	tcsetattr(tty_fd,TCSANOW,&tio);
	//fcntl(tty_fd, F_SETFL, 0);
  	//fd = open(, O_RDWR | O_NOCTTY | O_NDELAY);

	//else
		//fcntl(fd, F_SETFL, 0);

	return (tty_fd);
}

/*
void packet_receive(int port)
{
	char magic;
	size_t length;
	packet_data_t my_packet;

	while(magic!=0xFE)
	{
		read(port,&magic,1);
		printf(".");
		// Turn on looking for lead character LED here
	}
	read(port,&length,4);
    read(port,&my_packet,(length-4));
    print_data_packet(&my_packet);
}
*/

int main()
{
	int port=open_port(UART4);

	unsigned char c;
	while(1)
	{
		if (read(port, &c, 1))
		{
			printf("0x%x\n", c);
		}
		else
		{
			printf(".");
		}
	}

#if 0
	char a=0xab,b=0xFE;
	uint32_t length;
	int m;
	packet_data_t data;
	char magic=123;

	printf("Starting\n");
	//int n = write(port, "Hello World\r\n", 13);
	//if (n < 0) perror("write failed");
	int ret = 0;

	printf("Start reading\n");
	while(1)
	{

		//packet_receive(port);

		/*
		read(port,&magic,1);
		printf("%x ",magic);
		if(magic==0xFE) printf("---");
		ret=tcflush(port,TCIOFLUSH);
		if(ret<0) perror("Not flushed");
		*/


		length=get_heartbeat(&data);
		//printf("read try\n");
		//m=read(port,&length,4);
		//read(port,((&data)+sizeof(size_t)),length-sizeof(size_t));


		//if(m>0) printf("%c",a);
		//else perror("read error");
		//write(port,&b, 1);b++;write(port,"\n\r", 2);
		write(port,&b, 1);
		write(port,&length, 4);
		write(port,&data, length);
		print_data_packet(&data);
		sleep(2);

}
#endif

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
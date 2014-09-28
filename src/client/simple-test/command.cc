#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <string.h>
#include <strings.h>


using namespace std;	
int _serialDescriptor;

void user_Input(){
	char byte;
	char velX;
	char velY;
	int input_velX;
	int input_velY;
    printf("--------------------------------------------\n");
	printf("Please input X\n");
	if (scanf("%d", &input_velX) != EOF){
		if(input_velX < -100)
			input_velX = -100;
		else if(input_velX >100)
			input_velX = 100;

		velX = (char)input_velX;
	}
	else return;
	
	printf("Please input Y\n");
	if (scanf("%d", &input_velY) != EOF){
		
		if(input_velY < -100)
			input_velY = -100;
		else if(input_velY >100)
			input_velY = 100;

        velY = (char)input_velY;
	}

	if (write(_serialDescriptor,&velX,sizeof(char)) < 0){
		printf("PROTOCOL - CRITICAL ERROR = impossible to write in Serial link\n");
	}
	if (write(_serialDescriptor,&velY,sizeof(char)) < 0){
		printf("PROTOCOL - CRITICAL ERROR = impossible to write in Serial link\n");
	}

    read(_serialDescriptor, &byte, sizeof(char));
    printf("receive right : %u\n",(unsigned char) byte);
	read(_serialDescriptor, &byte, sizeof(char));
    printf("receive left:%u\n",(unsigned char) byte);
    //read(_serialDescriptor, &byte, sizeof(char));
    //printf("receive %u\n",(unsigned char) byte);

}


int main(int argc, char * argv []){
	char byte;	
	if (argc < 2){
		printf("NEED A device id !! \n");
		exit(0);
	}
	string dev = argv [1];
	_serialDescriptor = open(dev.c_str(), O_RDWR | O_NOCTTY); 
	struct termios _newtio;	
	bzero(&_newtio, sizeof(_newtio));
	
	//parameterizing control
	_newtio.c_cflag |= B9600;//default baudrate is initialbaudrate set by the user, can be changed by user with parameterize function
	_newtio.c_cflag &= ~CRTSCTS;//controlling the RTS/CTS flow
	
	//parity setting
	_newtio.c_cflag &= ~PARENB;//no parity
	_newtio.c_cflag &= ~CSTOPB;//no second stop bit
	//character size
	_newtio.c_cflag &= ~CSIZE;//mask the character size bits
	_newtio.c_cflag |= CS8;//8 data bits
	
	_newtio.c_cflag |= CLOCAL;//ignore modem control lines
	_newtio.c_cflag |= CREAD;//enable receiver
	
	//parameterizing inputs and outputs
	_newtio.c_iflag = 0;//ignore parity
	_newtio.c_oflag = 0;
	_newtio.c_lflag = 0;/* set input mode (non-canonical, no echo,...) */
	
	_newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
	_newtio.c_cc[VMIN]     = 1;   /* blocking read until 1 char received */
	if (tcflush(_serialDescriptor, TCIOFLUSH) == -1) {printf("PROTOCOL - ERROR : flush serialinputs and outputs failed\n");
	exit(0);}//flushing received but not readed characters
	if (tcsetattr(_serialDescriptor,TCSANOW,&_newtio) == -1) {printf("PROTOCOL - ERROR : setting attributes of serial link failed\n"); 
	exit(0);}//setting new attributes
	//now reading/writing
//	do{
//		read(_serialDescriptor, &byte, sizeof(char));
//		printf("receive %c\n",byte);		
//	}while(byte != 'E');	
	
	for(;;){	
		user_Input();	
	}

	//close serial port
	tcflush(_serialDescriptor, TCIOFLUSH);//flushing unreaded characters
	close(_serialDescriptor);//closing port
}

#include "send_to_arduino.h"
 
 
static const char *devName = "/dev/i2c-1";
using namespace std;

void send_to_arduino(const char* cmd) 
{
	int file;
	if ((file = open(devName, O_RDWR)) < 0) {
		fprintf(stderr, "I2C: Failed to access %s\n", devName);
		exit(1);
	}
 
//	printf("I2C: acquiring buss to 0x%x\n", ADDRESS);
 
	if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
		fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", ADDRESS);
		exit(1);
	}

	cout << "sending: "; 
	for(int i=0; i<strlen(cmd); i++ ){
		cout<< *(cmd+i);
	}
	cout << endl;

	if (write(file, cmd, strlen(cmd)) == strlen(cmd)) {
		usleep(100);
	}
	close(file);

}

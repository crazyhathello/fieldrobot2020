#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "send_to_arduino.h"

using namespace std;

int main()
{
    	struct termios old_tio, new_tio;
    	unsigned char c;
	char forward[] = "move0";
	char turn_left[] = "move-30";
	char turn_right[] = "move30";
	char stop[] = "stop";

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    do {
	c=getchar();
	switch(c)
	{
		case 'w':
			send_to_arduino(forward);
			printf("forward\n");
			break;
		case 'a':
			send_to_arduino(turn_left);
			printf("turn left\n");
			break;
		case 'd':
			send_to_arduino(turn_right);
			printf("turn right\n");
			break;
		case 's':
			send_to_arduino(stop);
			printf("stop\n");
			break;
	}
    } while(c!='q');

    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

    return 0;
}

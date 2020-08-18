#include "send_to_arduino.h"
#include <unistd.h>

using namespace std;

int main()
{
	char command[] = "move0"; 
	send_to_arduino(command);
	sleep(2);
	
	char command2 [] = "stop";
	send_to_arduino(command2);
	return 0;
}

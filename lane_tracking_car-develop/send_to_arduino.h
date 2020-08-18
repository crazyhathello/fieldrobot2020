#ifndef send_to_arduino_h
#define send_to_arduino_h


#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
 
#define ADDRESS 0x2A
void send_to_arduino(const char *);

#endif


/**
  ******************************************************************************
  * @file	uArmPin.h
  * @author	David.Long
  * @email	xiaokun.long@ufactory.cc
  * @date	2016-10-17
  ******************************************************************************
  */

#ifndef _UARMPIN_H_
#define _UARMPIN_H_

#include "uArmConfig.h"

#define EXTERNAL_EEPROM_SYS_ADDRESS 0xA2
#define EXTERNAL_EEPROM_USER_ADDRESS  0xA0

#ifdef MKII
    #define BUZZER          		38    // HIGH = ON
	#define BT_DETECT_PIN			39

	#define LIMIT_SW                40    // LIMIT Switch Button

	#define BTN_D4                  38    // LOW = Pressed
	#define BTN_D7                  38   // LOW = Pressed

	#define SYS_LED					38

	#define PUMP_EN                38
	#define GRIPPER                 38
	#define GRIPPER_FEEDBACK        38
	#define PUMP_FEEDBACK			38

	#define POWER_DETECT			38


	#define SERVO_ROT_PIN           1
	#define SERVO_LEFT_PIN          9
	#define SERVO_RIGHT_PIN         11
	#define SERVO_HAND_ROT_PIN      6

	#define SERVO_ROT_ANALOG_PIN 		2
	#define SERVO_LEFT_ANALOG_PIN 		0
	#define SERVO_RIGHT_ANALOG_PIN 		1
	#define SERVO_HAND_ROT_ANALOG_PIN 	3

#elif defined(METAL)

	#define BUZZER                  38    // HIGH = ON
	#define LIMIT_SW                38    // LIMIT Switch Button

	#define BTN_D4                  38    // LOW = Pressed
	#define BTN_D7                  38    // LOW = Pressed

	#define PUMP_EN                 38    // HIGH = Valve OPEN
	#define VALVE_EN                38    // HIGH = Pump ON
	#define GRIPPER                 38    // LOW = Catch
	#define GRIPPER_FEEDBACK        38

	#define SERVO_ROT_PIN           2
	#define SERVO_LEFT_PIN          3
	#define SERVO_RIGHT_PIN         6
	#define SERVO_HAND_ROT_PIN      38

	#define SERVO_ROT_ANALOG_PIN 		2
	#define SERVO_LEFT_ANALOG_PIN 		3
	#define SERVO_RIGHT_ANALOG_PIN 		6
	#define SERVO_HAND_ROT_ANALOG_PIN 	38

#endif	// MKII

#endif // _UARMPIN_H_

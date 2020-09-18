#ifndef _APPLE_GRIPPING_H_
#define _APPLE_GRIPPING_H_
#include "armright_cam_config.h"
#include "arm_control.h"

enum claw_mode
{
	FETCH,
	RELEASE
};

//determine the apple location and control the arm to the target location
void apple_gripping(int, claw_mode mode);

#endif // !_APPLE_GRIPPING_H_


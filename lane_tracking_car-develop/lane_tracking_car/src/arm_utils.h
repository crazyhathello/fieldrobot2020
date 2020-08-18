#ifndef _ARM_UTILS_H_
#define _ARM_UTILS_H_
#include "uArm.h"


#define USE_SERIAL_CMD  0 // 1: use serial for control  0: just use arduino to control(release ROM and RAM space)
  void Init();
  void arm_init();
  void tickTimeOut();
  void run();
  void tickTaskRun();
  void manage_inactivity(void);


#endif

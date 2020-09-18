#include "arm_utils.h"


  unsigned long tickStartTime = millis(); // get timestamp;
  void Init()
{
  uArmInit(); // Don't remove
  service.init();

  #if USE_SERIAL_CMD == 1
  serialCmdInit();
  

  #endif
}




void arm_init(){
    Init(); 
 service.setButtonService(false);  // disable build in button service
  double angleB,angleL,angleR;
  double x,y,z;
  x = -150;
  y = 0;
  z = 140;
   controller.xyzToAngle(x,y,z,angleB,angleL,angleR);
 controller.writeServoAngle(angleB,angleL,angleR);

  }

// time out every TICK_INTERVAL(50 ms default)
void tickTimeOut()
{
  
}

  void run()
{
  #if USE_SERIAL_CMD == 1
  handleSerialCmd();
  #endif

  manage_inactivity(); // Don't remove
}

void tickTaskRun()
{
  tickTimeOut();

    buttonPlay.tick();
    buttonMenu.tick();
#ifdef MKII
    ledRed.tick();
    service.btDetect();
#endif    
}
void manage_inactivity(void)
{
#if USE_SERIAL_CMD == 1
  getSerialCmd(); // for serial communication
#endif
  service.run();  // for led, button, bt etc.

  // because there is no other hardware timer available in UNO, so use a soft timer
  // it's necessary for button,led, bt
  // so Don't remove it if you need them
  if(millis() - tickStartTime >= TICK_INTERVAL)
  {
    tickStartTime = millis();
    tickTaskRun();
  }   
}



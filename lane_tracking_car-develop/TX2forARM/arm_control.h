#ifndef _ARM_CONTROL_H_
#define _ARM_CONTROL_H_

#include "unistd.h"
#include "armright_cam_config.h"
#include "../send_to_arduino.h"

#define Y_CLAW_OFFSET 6      //costumized offset (related to image center) for claw is needed
#define Y_PUMP_OFFSET 6     //costumized offset (related to image center) for pump is needed
#define RETURN_HOME_TIME 2 
#define TO_PREPARED_POS1_TIME 2
#define TO_PREPARED_POS2_TIME 3
#define CLAW_OPEN_TIME 8.5
#define CLAW_CLOSE_TIME 8.5
#define CLAW_DESCEND_TIME 2

#define SEARCH_LMAX -100
#define SEARCH_RMAX 100
#define SEARCH_FMAX 350
#define SEARCH_BMAX 100
#define SEARCH_X_STRIDE 5

#define SEARCH_X_FAR_DEADLINE -150
#define SEARCH_X_CLOSE_DEADLINE 150



// Arm constroller determines how the arm should move and send commands to mega
// The constructor of armController set default value of the buffer area of the target coordinate

enum MoveMode{  //select the mode( gripping  or pump)
	PUMP,
	GRIP
};


class armController
{
public:


	float mBuf_x_precent = 0.0 ;
	float mBuf_y_precent = 0.0 ;

	int mStride_x ;
	int mStride_y ;

	int target_x_l = 0;
	int target_x_h = 0;
	int target_y_l = 0;
	int target_y_h = 0;

	armController(int, int);
	armController(float buf_x_precent, float buf_y_precent, int stride_x, int stride_y);
	~armController();

	//initalize the target buffer area 
	void set_target_y_offset(MoveMode mode);
	void search();

	//send moving command to mega, return True if target is aimed
	bool move_to_dst(int& dest_x, int& dest_y);

	//send decending command to mega, return True if ACK is receiced
	bool descend_arm(int height);

	//send open claw command to mega, return True if ACK is receiced
	bool open_claw( );

	//send close pump command to mega, return True if ACK is receiced
	bool close_claw();
	bool descend_pump();
//send open pump command to mega, return True if ACK is receiced
	bool open_pump(int times);

	//send close pump command to mega, return True if ACK is receiced
	bool close_pump();

	//send ascending command to mega, return True if ACK is receiced
	bool ascend_arm(int height);

	//send moving to home command, return True if ACK is received
	bool move_to_home();
	bool move_to_prepared_pos1();
	bool move_to_prepared_pos2();
	bool searchX(const int arm_cur_x);
	bool searchY(const int arm_cur_y);	
};


extern armController arm_Controller;

#endif // !ARM_CONTROL_H

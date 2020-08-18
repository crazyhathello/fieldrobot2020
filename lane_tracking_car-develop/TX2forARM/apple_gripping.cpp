#include "apple_gripping.h"
#include <unistd.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

#define MOVE_CAR_GRIP_DELAY 1.5

void apple_search(armController&, Camera&);

void apple_gripping(int cam_index, claw_mode mode)
{
	int  apple_x, apple_y;
	int  plate_x, plate_y;

///////////////  after the sign is detected, move forward a little /////////
	string cmd = "move0";  
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar); 	
	sleep(MOVE_CAR_GRIP_DELAY);  // sleep a while to let car move to the terget center
	
	cmd = "stop";  // stop the car after moving
	char const *pchar1 = cmd.c_str();
	send_to_arduino(pchar1);
///////////////  after the sign is detected, move forward a little /////////

	Camera armCam(cam_index, 480, 640);

	while(!armCam.mCap.isOpened())
	{
		cout << "cannot open arm camera" << endl;
		sleep(1);
	}

	armController arm(armCam.getWidth(), armCam.getHeight());
	
	arm.move_to_prepared_pos2();  //move from home to prepared pos2 
	sleep(TO_PREPARED_POS2_TIME);
	arm.move_to_prepared_pos1();  //move from prepared pos2 to prepared pos1 (ready for gripping)
	sleep(TO_PREPARED_POS1_TIME);
	

	if (mode == FETCH) { 
		// loop will not break until the destination is found 
		armCam.set_parameter(APPLE);
		arm.set_target_y_offset(GRIP);
		apple_search(arm, armCam);
		cout << " complete search!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		while (true) {  
			//module_t.start();
			if (armCam.Detect(apple_x, apple_y) > 0) {
				//module_t.stop();
				//	module_t.display("arm_cam");
				if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
					cout << "esc key is pressed by user" << endl;
					break;
				}

			}
			else
			{
				cout << "exit function apple_gripping()..." << endl;
				break;
			}
		
			//std::cout << "apple x:  " << apple_x << "       apple y:  " << apple_y << std::endl;
			if (arm.move_to_dst(apple_x, apple_y) > 0) {
				cout << "TARGET AIMED!" << endl;
				//set_times("gripping"); //set timeout
					break;   // Practically, a break will be inserted
			
			}
			
		}//end while. Now the arm should be at the destination

		//////////////////open claw////////////////////////
		if (arm.open_claw() > 0) {
			sleep(CLAW_OPEN_TIME);   //wait for claw to open, this delay is the same as arduino but need to synchronize them both manually
			cout << "claw opened" << endl;
		}
		else
		{
			cout << "arm openning error.." << endl;
		}
		//////////////////open claw////////////////////////


		//////////////////descend arm////////////////////////
		if (arm.descend_arm(30) > 0) {
			sleep(CLAW_DESCEND_TIME);   //wait for arm to descend
			cout <<"arm descended"<< endl;
		}
		else
		{
			cout << "descend arm error.." << endl;
		}
		//////////////////descend arm////////////////////////


		//////////////////close claw////////////////////////
		if (arm.close_claw() > 0) {
			sleep(CLAW_CLOSE_TIME);   //wait for claw to open
		}
		else
		{
			cout << "arm closing error..." << endl;
		}
		//////////////////close claw////////////////////////

		//////////////////return prepared_pos2////////////////////////
		if (arm.move_to_prepared_pos2() > 0) {
			sleep(TO_PREPARED_POS2_TIME);   //wait for arm to move to prepared pos
		}
		else
		{
			cout << "returning prepared pos error..." << endl;
		}
		//////////////////return prepared_pos2////////////////////////
		//////////////////return home////////////////////////
		if (arm.move_to_home() > 0) {
			sleep(RETURN_HOME_TIME);   //wait for claw to open
		}
		else
		{
			cout << "returning home error, exit function apple_gripping()..." << endl;
		}
		//////////////////return home////////////////////////

	}
	else if (mode == RELEASE) {

		armCam.set_parameter(PLATE);
		arm.set_target_y_offset(GRIP);
		apple_search(arm, armCam);
		cout<<" complete search!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

		while (true) {
			//module_t.start();
			if (armCam.Detect(plate_x, plate_y) > 0) {
				//module_t.stop();
				//	module_t.display("arm_cam");
				if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
					cout << "esc key is pressed by user" << endl;
					break;
				}

			}
			else
			{
				cout << "exit function apple_gripping()..." << endl;
				break;
			}
			if (arm.move_to_dst(apple_x, apple_y)> 0) {
				cout << "TARGET AIMED!" << endl;
				//set_times("gripping"); //set timeout
					break;   // Practically, a break will be inserted
			
			}
		}//end while

		//////////////////descend arm////////////////////////
		if (arm.descend_arm(30) > 0) {
			sleep(CLAW_DESCEND_TIME);   //wait for arm to descend
		}
		else
		{
			cout << "descend arm error." << endl;
		}
		//////////////////descend arm////////////////////////
		
		
		//////////////////open claw////////////////////////
		if (arm.open_claw() > 0) {
			sleep(CLAW_OPEN_TIME);   //wait for claw to open
		}
		else
		{
			cout << "arm openning error..." << endl;
		}
		//////////////////open claw////////////////////////
		
		//////////////////return prepared pos2//////////////////////
		if (arm.move_to_prepared_pos2() > 0) {
			sleep(TO_PREPARED_POS2_TIME);   //wait for arm to move to prepared pos
		}
		else
		{
			cout << "returning home error ..." << endl;
		}
		////////////////////return prepared pos2////////////////////


		//////////////////close claw////////////////////////
		if (arm.close_claw() > 0) {
			sleep(CLAW_CLOSE_TIME);   //wait for claw to open
		}
		else
		{
			cout << "arm closing error..." << endl;
		}
		//////////////////close claw////////////////////////

		//////////////////return home///////////////////////
		if (arm.move_to_home() > 0) {
			sleep(RETURN_HOME_TIME);   //wait for claw to open
		}
		else
		{
			cout << "returning home error..." << endl;
		}
		////////////////////return home//////////////////////


	}
	armCam.mCap.release();
}

void apple_search(armController& arm, Camera& armCam)
{
	bool mode = 0;   // mode 0 for left searching , mode 1 for right searching
	int arm_cur_x=0;
	int arm_cur_y=100;
	int confirm_target=0;
	while(true){
		int target_x=-1;
		int target_y=-1;
		armCam.Detect(target_x,target_y);     // if no target found, then target_x, target_y remains neg value
		if(target_x < 0 || target_y < 0){   //target not found  , S shaped search, start from left search
			switch(mode){
				case 0:  // Left search
					if( arm_cur_x <= SEARCH_LMAX){
						mode = 1;
						arm_cur_y+=25;
						if(arm_cur_y >= SEARCH_FMAX) arm_cur_y = SEARCH_BMAX;
					}

					arm_cur_x-=SEARCH_X_STRIDE;
					break;
				case 1: // Right search
					if( arm_cur_x >= SEARCH_RMAX){
						mode = 0;
						arm_cur_y+=25;
						if(arm_cur_y >= SEARCH_FMAX) arm_cur_y = SEARCH_BMAX;
					}

					arm_cur_x+=SEARCH_X_STRIDE;
					break;
			}

			if( arm.searchX(arm_cur_x)==0) cout << "failed sendind searchX command";
			if( arm.searchY(arm_cur_y)==0) cout << "failed sendind searchY command";
		}else{  // target found, not sure if we need to move car again if the target is still too far
			/*if(target_x <= SEARCH_X_FAR_DEADLINE)   // the car is not close enough to the apple , so move car forward
				;//movecar(forward);
				else if(target_x >= SEARCH_X_CLOSE_DEADLINE  ) // the car is too far away from apple , so move car backward
				;//movecar(backward);
			else {
				//stop car
				break;  // Now the arm suppose to be at a suitable position
				}*/
			confirm_target++;
			if(confirm_target > 10) break;
		}
	}  //end while
}


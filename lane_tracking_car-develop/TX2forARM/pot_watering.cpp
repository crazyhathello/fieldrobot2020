#include "pot_watering.h"
#include "opencv2/opencv.hpp"
#include "unistd.h"

#define MOVE_CAR_WATER_DELAY 1

using namespace cv;
using namespace std;

void pot_search(armController&, Camera&);

void pot_watering(int cam_index, int water_times)
{
	Camera armCam(cam_index, 480, 640);
	while(!armCam.mCap.isOpened())
	{
		cout << "cannot open arm camera" << endl;
		sleep(1);
	}

	armCam.set_parameter(POT);

	armController arm(armCam.getWidth(), armCam.getHeight());

	
	int  pot_x, pot_y;
//////////// after the sign is detected, move forward a little ///////////
/*
	string cmd = "move0"; 
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar); 	
	sleep(MOVE_CAR_WATER_DELAY);  // sleep a while to let car move to the terget center
	
	cmd = "stop";  // stop the car after moving
	char const *pchar1 = cmd.c_str();
	send_to_arduino(pchar1);
//////////// after the sign is detected, move forward a little ///////////
*/
	string cmd = "stop";  // stop the car after moving
	char const *pchar1 = cmd.c_str();
	send_to_arduino(pchar1);

	arm.move_to_prepared_pos2();
	sleep(TO_PREPARED_POS2_TIME);
	//arm.move_to_prepared_pos1();
	//sleep(TO_PREPARED_POS1_TIME);


	arm.set_target_y_offset(PUMP);
//	pot_search(arm, armCam);     //if  the arm is high enough, no need to search
	
//	cout << "end pot searching" << endl;

	while (true) {
		//module_t.start();
		if (armCam.Detect(pot_x, pot_y) > 0) {
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

////////////////////////////// move to the top of chosen pot /////////////
		if (arm.move_to_dst(pot_x, pot_y) > 0) {
			cout << "TARGET(POT) AIMED!" << endl;
			break;   // Practically, a break will be inserted

		}
////////////////////////////// move to the top of chosen pot /////////////

	} //end while

	arm.descend_pump(); 
	sleep(1);
	//////////////////open pump////////////////////////
	
	if(water_times==2){
		 if(arm.open_pump(2)>0){
			 sleep(WATERING_TIME2);   //wait for claw to open
		 }else std::cout<<"open pump error..."<<std::endl;
	}
	else{
		if(arm.open_pump(1)>0){
		 	sleep(WATERING_TIME1);
		}else std::cout<<"open pump error..."<<std::endl;

	}
	//////////////////open pump////////////////////////
	
	//////////////////close pump////////////////////////
	if (arm.close_pump() > 0) {
	;   //wait for claw to open
	}
	else
	{
		cout << "close pump error!" << endl;
	}
	//////////////////close pump////////////////////////


	//////////////////return prepared pos////////////////////////
	if (arm.move_to_prepared_pos2() > 0) {
		sleep(TO_PREPARED_POS2_TIME);   //wait for claw to open
	}
	else
	{
		cout << "returning prepared pos error..." << endl;
	}

	//////////////////return prepared pos////////////////////////

	//////////////////return home////////////////////////
	if (arm.move_to_home() > 0) {
		sleep(RETURN_HOME_TIME);   //wait for claw to open
	}
	else
	{
		cout << "returning home error..." << endl;
	}

	//////////////////return home////////////////////////


	armCam.mCap.release();
}

void pot_search(armController& arm, Camera& armCam)
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







#include "arm_control.h"

armController::armController(int armCam_width, int armCam_height)
{
	std::cout<<"armCam width: "<<armCam_width<<"  armCam_height: "<<armCam_height<<std::endl;
	mBuf_x_precent = 0.03;   // need to calibrate the target x,y buffer size 
	mBuf_y_precent = 0.02;
	mStride_x = 30; 
	mStride_y = 30;
	
	float target_x_l_precent = 0.5 - mBuf_x_precent;
	float target_x_h_precent = 0.5 + mBuf_x_precent;
	float target_y_h_precent = 0.5 - mBuf_y_precent;
	float target_y_l_precent = 0.5 + mBuf_y_precent;

	target_x_l = armCam_width * target_x_l_precent;
	target_x_h = armCam_width * target_x_h_precent;
	target_y_l = armCam_height * target_y_l_precent;
	target_y_h = armCam_height * target_y_h_precent;

}

armController::armController(float buf_x_precent, float buf_y_precent, int stride_x, int stride_y){
	mBuf_x_precent = buf_x_precent;
	mBuf_y_precent = buf_y_precent; 
	mStride_x = stride_x;
	mStride_y=  stride_y;
}

armController::~armController()
{
}

void armController::set_target_y_offset(MoveMode mode){

	if(mode == PUMP){
		target_y_l-=Y_PUMP_OFFSET;
		target_y_h-=Y_PUMP_OFFSET;
	}
	else if(mode == GRIP){
		target_y_l+=Y_CLAW_OFFSET;
		target_y_h+=Y_CLAW_OFFSET;
	}

}
/*
void armController::search(){

	bool mode=0;   // mode 0 for left searching , mode 1 for right searching
	int arm_cur_x=0;
	int arm_cur_y=100;
	int confirm_target=0;
	while(true){
		int target_x=-1;   
		int target_y=-1;
		armCam.Detect(target_x,target_y);     // if no target found, then target_x, target_y remains neg value
		if(target_x<0 || target_y<0){   //target not found  , S shaped search, start from left search
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

			if( armController::searchX(arm_cur_x)==0) cout<<"failed sendind searchX command";
			if( armController::searchY(arm_cur_y)==0) cout<<"failed sendind searchY command";
		}else{  // target found, not sure if we need to move car again if the target is still too far
			if(target_x <= SEARCH_X_FAR_DEADLINE)   // the car is not close enough to the apple , so move car forward
				;//movecar(forward);
				else if(target_x >= SEARCH_X_CLOSE_DEADLINE  ) // the car is too far away from apple , so move car backward
				;//movecar(backward);
			else {
				//stop car
				break;  // Now the arm suppose to be at a suitable position
				}
			confirm_target++;
			if(confirm_target>10) break;
		}
	}  //end while
}
*/
//send moving command to mega, return True if target is aimed
bool armController::move_to_dst(int& dest_x, int& dest_y) {

	int error=0;
	bool  xAimed = false;
	bool  yAimed = false;

	//////////// X direction control /////////////
	if (dest_x <= target_x_l) {     //if target is at the left of the Camera screen
		//error = abs(dest_x - target_x_l);
		//int xstride = ((float)error / (armCam.mWidth/2)) * mStride_x;
		//std::cout << "   Move Left, stride: " << xstride ;
		std::cout<< "   Move Left "<<std::endl;
		std::string cmd = "armL";
		char const *pchar = cmd.c_str();
		send_to_arduino(pchar); //send MRIGHT + xSTRIDE
	}
	else if (dest_x >= target_x_h) { //if target is at the right of the Camera screen
		//error = abs(dest_x - target_x_h);
		//int xstride = ((float)error / (armCam.mWidth / 2)) * mStride_x;
		
		std::string cmd = "armR";
		char const *pchar = cmd.c_str();
		//std::cout << "   Move Right, stride: " << xstride ;
		std::cout << "   Move Right "<<std::endl;
		send_to_arduino(pchar); 	
	}
	else {   //if target is correctly located
		std::cout << "   X Target Aimed ";
		xAimed = true;
	}

	//////////// Y direction control /////////////
	if (dest_y >= target_y_l) {     //if target is at the left of the Camera screen
		//error = abs(dest_y - target_y_l);
		//int ystride = ((float)error / armCam.mHeight) * mStride_y;
		//std::cout << "   Move Forward,  stride: " << ystride << std::endl;
		std::cout << "   Move Borward "<<std::endl;
		std::string cmd = "armB";
		char const *pchar = cmd.c_str();
		send_to_arduino(pchar); 
	}
	else if (dest_y <= target_y_h) { //if target is at the right of the Camera screen
		//error = abs(dest_y - target_y_h);
		//int ystride = ((float)error / armCam.mHeight) * mStride_y;
		//std::cout << "   Move Backward, stride: " << ystride << std::endl;
		std::cout << "   Move Fackward "<<std::endl;
		std::string cmd = "armF";
		char const *pchar = cmd.c_str();
		send_to_arduino(pchar); 

	}
	else {   //if target is correctly located
		std::cout << "   Y Target Aimed " << std::endl;
		yAimed = true;
	}

	/////////////send command to mega///////

	/////////////send command to mega///////
	std::cout<<"dest x: "<<dest_x<<"  dest y: "<<dest_y<<"  target_y_h: "<<target_y_h<<" target_y_l: "<<target_y_l<<std::endl;
	return (xAimed && yAimed);
}

//send decending command to mega, return True if target is reached 
bool armController::descend_arm(int height) {
	bool complete = false;
	std::string cmd ="armDescend"+ std::to_string(height);
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar); 
	//if()     there should be an ACK from mega

	complete = true;

	return complete;
}

//send decending command to mega, return True if claw is opened
bool armController::open_claw() {
	bool complete = false;
	std::string cmd = "clawOpen";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}

bool armController::close_claw()  {
	bool complete = false;
	std::string cmd = "clawClose";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}

bool armController::descend_pump() {
	bool complete = false;

	std::string cmd = "armpumpDescend";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}

bool armController::open_pump(int water_times) {
	bool complete = false;
	if(water_times==2){
		std::string cmd = "pumpOpen2";
		char const *pchar = cmd.c_str();
		send_to_arduino(pchar);
	}
	else{
		std::string cmd = "pumpOpen1";
		char const *pchar = cmd.c_str();
		send_to_arduino(pchar);

	}
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}

bool armController::close_pump() {
	bool complete = false;

	std::string cmd = "pumpClose";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}

bool armController::ascend_arm(int height) {
	bool complete = false;
	std::string cmd = "armAscend" + std::to_string(height);
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}



bool armController::move_to_home() {

	bool complete = false;
	std::string cmd = "armHome";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}


bool armController::move_to_prepared_pos1() {

	bool complete = false;
	std::string cmd = "armPreparedPos1";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}

bool armController::move_to_prepared_pos2() {

	bool complete = false;
	std::string cmd = "armPreparedPos2";
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar);
	//if()     there should be an ACK from mega
	complete = true;

	return complete;
}




//sending command to search in x axis
bool armController::searchX(const int arm_cur_x){

	bool complete = false;
	std::string cmd = "armX"+ std::to_string(arm_cur_x);
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar); 
	//if()     there should be an ACK from mega
	complete = true;

	return complete;

}

//sending command to search in y axis
bool armController::searchY(const int arm_cur_y){
	
	bool complete = false;
	std::string cmd = "armY"+ std::to_string(arm_cur_y);
	char const *pchar = cmd.c_str();
	send_to_arduino(pchar); 
	//if()     there should be an ACK from mega
	complete = true;

	return complete;

}


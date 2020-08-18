These files in directory /TX2forARM are for missions.
The Makefile of these files are written in shell-scipt(build.sh) in the previous directory


1. opencv_pratice.cpp is the old version of main function for debug, it is now merged into the new main function in land_tracking_car.cpp .

2. timer.cpp is for timing debug.


3. All the camera settings are in file 'armright_cam_config.h' and 'armright_cam_config.cpp', encapsulated in class Camera.

4. armright_cam_utils.cpp is for some sorting and filtering functions used by 'armright_cam_config.cpp'.



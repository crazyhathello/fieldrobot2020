# fieldrobot2020
2020 NTU BME fieldrobot

Code used in 2020 fieldrobot competition

這篇文章是2020年 toolman lab 專題生的 Z02隊參加在嘉義大學舉辦的田間機器人競賽的開發紀錄，希望能提供足夠的資訊讓之後挑戰田間機器的學弟妹能有好的headstart，
花少點時間在我們已經遇過的問題上。
## Features

- System consists of NVIDIA Jetson TX2 and Arduino Mega 2560, with transmission via serial

- Implements a lane tracking algorithm for synthetic grass paths using opencv image processing, allowing robot to follow along the appropriate path.

## File structure
Python and Arduino main program: /Main_Program

Fritzing circuit schematic: /Circuit design

Testing and Dev functions: /camera_test_code

segnet model (from https://github.com/divamgupta/image-segmentation-keras): /Segnet_model

reference code from previous years: /lane_tracking_car-develop

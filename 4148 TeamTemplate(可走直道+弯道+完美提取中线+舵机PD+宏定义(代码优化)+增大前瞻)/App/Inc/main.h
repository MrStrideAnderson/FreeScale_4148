#ifndef MAIN_H
#define MAIN_H

#include "VCAN_camera.h"

//#define SENDIMG
//#define CHANGESPEED
//#define PRINTFRE

extern uint8 imgbuff[CAMERA_SIZE];//压缩后的二值化图像
extern uint8 img[CAMERA_H][CAMERA_W];//解压后的灰度图像
extern uint8 img2[CAMERA_H][CAMERA_W];
extern volatile uint32 cnvtime;

void IntToStr(uint8 num, char str[]);

#endif
 /*
******************************************************************************* 
*                    .::::.  
*                  .::::::::.  
*                 :::::::::::  
*             ..:::::::::::'  
*           '::::::::::::'  
*             .::::::::::  
*        '::::::::::::::..  
*             ..::::::::::::.  
*           ``::::::::::::::::  
*            ::::``:::::::::'        .:::.  
*           ::::'   ':::::'       .::::::::.  
*         .::::'      ::::     .:::::::'::::.  
*        .:::'       :::::  .:::::::::' ':::::.  
*       .::'        :::::.:::::::::'      ':::::.  
*      .::'         ::::::::::::::'         ``::::.            @Author: 4148 Team    
*  ...:::           ::::::::::::'              ``::.           @Version: v1.0.0
* ```` ':.          ':::::::::'                  ::::..        @Date: 2018��3��13��
*                    '.:::::'                    ':'````..     @Description:
*
*******************************************************************************
*/

#ifndef IMPROC_H
#define IMPROC_H

#include "include.h"
#include "common.h"


#define   WHITE_VAL   255
#define   BLACK_VAL   0
#define   CAM_MAX_LENGTH_CM  250
#define   LINE_WIDTH 7
#define   AVG_COUNT  5

#define   W_START    0
#define   W_END      (CAMERA_W-1)
#define   W_MIDD     (CAMERA_W/2-1)

#define  H_START  0
#define  H_END    (CAMERA_H-1)

extern imINT16  HBoundL[CAMERA_H];//��߽�
extern imINT16  HBoundR[CAMERA_H];//�ұ߽�
extern imINT16  HBoundM[CAMERA_H];//����
extern imINT16  HBoundM_F[CAMERA_H];//�˲��������
extern imINT16  HBoundM_REAL[CAM_MAX_LENGTH_CM+1];//�˲��󣬷��Ƴ�����������Ϊ���������
extern imINT32  gDir_Near;
extern imINT32  gDir_Mid;
extern imINT32  gDir_Far;
extern int MaxWhite;
extern int Out;
extern int W_MID;
extern void Graph_FindMidLine(void);
extern void Graph_Calculate_Dir(int speed);


#endif
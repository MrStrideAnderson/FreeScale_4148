/*
 *******************************************************************************
 *                                EIT CarLib
 *                             lib FUNCTIONS
 *
 *                     (c) Copyright 2015-2020 Car@EIT 
 *                            All Rights Reserved
 * File      : EIT_PID.h
 * This file is part of EIT Car Project
 * Embedded Innovation Team(EIT) - Car@EIT  
 * ----------------------------------------------------------------------------
 * LICENSING TERMS:
 * 
 *     CarLib is provided in source form for FREE evaluation and  educational 
 * use.
 *    If you plan on using  EITLib  in a commercial product you need to contact 
 * Car@EIT to properly license its use in your product. 
 * 
 * ----------------------------------------------------------------------------
 * Change Logs:
 * Date           Author       Notes
 * 2015-02-06     Xian.Chen    the first version
 * 2015-02-08     Xian.Chen    add PID_SetSpVal and PID_SetSpVal
 *
 *******************************************************************************
 */
#ifndef __EIT_PID_h__
#define __EIT_PID_h__

#include "common.h"

typedef struct _PID
{
    /*In*/
    int32   spVal;				//目标值
    int32   spValRamp;			//限制后的动态目标值
    int32   spUpRate;   		//上升速度
    int32   spDnRate;			//下降速度（应是个负数）
    int32   fbValFilterLast; //四个值滑动平均值滤波后的上次反馈量
    int32   fbValFilter;	 //四个值滑动平均值滤波后的本次反馈量
    int32   fbValFilterDiff; 
	//四个值滑动平均值滤波后的本次 与 四个值滑动平均值滤波后的上次 的差值
    int32   fbVal_k0;     //上0次反馈量
    int32   fbVal_k1;	 //上1次反馈量
    int32   fbVal_k2;	 //上2次反馈量
    int32   fbVal_k3;	 //上3次反馈量
    /*Out*/
    int32   outVal; //pid输出值
    /*Var*/
    int32   err;	//内部误差（误）
    int32   P;	//单独p计算后的量
    float   I;	//单独I计算后的量
    int32   D;	//单独D计算后的量
    
    /*Param*/
    int32   MAX_Val;	//输出限幅(最大值)
    int32   MIN_Val;	//输出限幅(最小值)
	
    float   Kp;		//kp,ki,kd参数
    float   Ki;
    float   Kd;
}PID;
typedef  PID*   PID_t;
extern void   PID_InitFbVal(PID_t tPID,int32 fbVal); //反馈量初始化
extern void   PID_SetFbVal(PID_t tPID,int32 fbVal); //设置反馈量
extern void   PID_Run_STD(PID_t tPID);	//D计算时使用滤波前的值
extern void   PID_Run_PID(PID_t tPID);  //D计算时使用滤波后的值
#endif 

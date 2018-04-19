#include "common.h"
#include "include.h"

double SteerKp=2.41, SteerKd=0.001;
PID SteerPID;
///*************************************************************************
//*  函数名称：GoStraigt
//*  功能说明：直走
//*  参数说明：无
//*  函数返回：无
//*  修改时间：2018年4月17日
//*  修改人  ：CLAY   
//*************************************************************************/
//void GoStraigt()
//{
//    ftm_pwm_duty(SD5_FTM, SD5_CH, STEER_MIDDLE);    
//}

void SteerPID_Init()
{
	SteerPID.Kp = 2.41;
	SteerPID.Ki = 0;
	SteerPID.Kd = 0;
	
	
	SteerPID.MAX_Val = 880; //左右极限均按30°。 左880 中850 右826
	SteerPID.MIN_Val = 826; //左右极限均按30°。 左880 中850 右826
	
	SteerPID.spVal   = 850 ;//打角目标值
	
	SteerPID.spUpRate = 200;   		//最大的上升速度
    SteerPID.spDnRate = -200 ;		//最大的下降速度（应是个负数）
	
	
	PID_InitFbVal(&SteerPID,0);
	SteerPID.I = 0;
    SteerPID.spValRamp=0;
		
}
/*************************************************************************
*  函数名称：Turn
*  功能说明：左转
*  参数说明：angle-对应的打角
*  函数返回：无
*  修改时间：2018年4月17日
*  修改人  ：CLAY   
*************************************************************************/
void Turn(int32 Angle)
{
    PID_SetFbVal(&SteerPID,80 + Angle); //Angle在[-80,80]
    PID_Run_PID(&SteerPID);

    ftm_pwm_duty(SD5_FTM, SD5_CH, SteerPID.outVal);//设置舵机打角 

}

/*************************************************************************
*  函数名称：GetError
*  功能说明：获取偏差
*  参数说明：无
*  函数返回：无
*  修改时间：2018年4月17日
*  修改人  ：CLAY   
*************************************************************************/
void GetError()
{
    uint8 error51, error31;
    int32 error;
//    error51 = (Middle[50]+Middle[51]+Middle[52])/3;//摄像头靠底部
//    error31 = (Middle[30]+Middle[31]+Middle[32])/3;
    error51 = (Middle[57]+Middle[58]+Middle[59])/3;//摄像头增大1.2cm前瞻后
    error31 = (Middle[40]+Middle[41]+Middle[42])/3;
    
    error = error51-error31;
//    if(error == 0)
//    {
//      GoStraigt();
//    }
//    else
//    {
      Turn(error);
//    } 
}

/*************************************************************************
*  函数名称：InitRoad
*  功能说明：初始化道路的相关参数
*  参数说明：无
*  函数返回：无
*  修改时间：2018年4月17日
*  修改人  ：CLAY   
*************************************************************************/
void SteerInit()//舵机初始化
{
   ftm_pwm_init(SD5_FTM, SD5_CH,SD5_HZ,STEER_MIDDLE);//舵机信号PA12 //左右极限均按30°。 左880 中850 右826
   

   SteerPID_Init();
}

/*************************************************************************
*  函数名称：InitRoad
*  功能说明：初始化道路的相关参数
*  参数说明：无
*  函数返回：无
*  修改时间：2018年4月17日
*  修改人  ：CLAY   
*************************************************************************/
void MotorInit()//电机初始化
{
    ftm_pwm_init(MOTOR540_FTM, MOTOR540_CH,MOTOR540_HZ,40);//电机信号PA10 占空比10/1000
}


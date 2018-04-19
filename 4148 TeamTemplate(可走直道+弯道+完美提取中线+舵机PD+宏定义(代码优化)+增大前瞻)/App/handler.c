#include "handler.h"
#include "main.h"


/*************************************************************************
*  函数名称：PORTA_IRQHandler
*  功能说明：摄像头的场中断和行中断
*  参数说明：无
*  函数返回：无
*  修改时间：2018年3月18日 
*  修改人  ：CLAY   
*************************************************************************/
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif


}

/*************************************************************************
*  函数名称：DMA0_IRQHandler()
*  功能说明：DMA图像传输
*  参数说明：无
*  函数返回：无
*  修改时间：2018年3月18日 
*  修改人  ：CLAY   
*************************************************************************/
void DMA0_IRQHandler()
{
    camera_dma();
}

/*************************************************************************
*  函数名称：FTM0_INPUT_IRQHandler()
*  功能说明：编码器M法测速
*  参数说明：无
*  函数返回：无
*  修改时间：2018年4月17日 
*  修改人  ：CLAY   
*************************************************************************/
 void FTM0_INPUT_IRQHandler()    
 {    
     static uint8 flag = 0;    
     uint8 s = FTM0_STATUS;          //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.    
     uint8 CHn;    
    
     FTM0_STATUS = 0x00;    
    
     CHn = 6;    
     if( s & (1 << CHn) )    
     {    
         /*     用户任务       */   
         if(flag == 0)    
         {    
              //第一次进来，开始计时    
              flag++;    
              ftm_input_clean(FTM0);            //清 计数器计数值    
          }    
          else if(flag == 1)    
          {    
              //第二次测到整个周期的时间    
              cnvtime = ftm_input_get(FTM0, FTM_CH6); //保存    
              flag = 0;    
     
              FTM_IRQ_DIS(FTM0,FTM_CH0);      //关闭FTM0_CH0 中断  （避免频繁中断）    
          }    
          /*********************/   
      }    
  }

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
* ```` ':.          ':::::::::'                  ::::..        @Date: 2018年3月13日
*                    '.:::::'                    ':'````..     @Description:
*
*******************************************************************************
*/

#include "common.h"
#include "include.h"
#include "handler.h"

//CAMERA_SIZE，CAMERA_H，CAMERA_H均是在VCAN_OV7725_Eagle.h定义的宏
uint8 imgbuff[CAMERA_SIZE];//压缩后的二值化图像
uint8 img[CAMERA_H][CAMERA_W];//解压后的灰度图像
uint8 img2[CAMERA_H][CAMERA_W];
volatile uint32 cnvtime = 0;                //输入捕捉值 

/*************************************************************************
*  函数名称：IntToStr
*  功能说明：整形数据转换为字符串
*  参数说明：num-传入的整数，str-对应写入的字符串
*  函数返回：无
*  修改时间：2018年4月11日 
*  修改人  ：CLAY   
*************************************************************************/
void IntToStr(uint8 num, char str[])
{
    char i=0;
    char buf[8];
    
    do{
      buf[i++] = num%10;
      num /= 10;
    }while(num > 0);
    
    while(i-- > 0)
    {
      *str++ = buf[i] + '0';
    }
    *str++ = '\r';
    *str++ = '\n';
    *str = '\0';
}

/*************************************************************************
*  函数名称：main
*  功能说明：主函数，自己看去
*  参数说明：无
*  函数返回：无
*  修改时间：2018年3月18日 
*  修改人  ：CLAY   
*************************************************************************/
void  main(void)
{
    //初始化
    SteerInit();//舵机初始化，信号PA12
    MotorInit();//电机初始化，信号PA10
    RoadInit(); //道路初始化
    camera_init(imgbuff);
    ftm_input_init(FTM0, FTM_CH6, FTM_Falling,FTM_PS_2);        //初始化FTM输入捕捉模式，下降沿捕捉(FTM0_CH6 为 PTD6)
    
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
    
#ifdef PRINTFRE
    set_vector_handler(FTM0_VECTORn ,FTM0_INPUT_IRQHandler);//设置FTM0的中断服务函数为 FTM0_INPUT_IRQHandler  
    enable_irq (FTM0_IRQn);  //使能FTM0中断
    port_init_NoALT(PTD6,PULLUP);  
#endif

    
    while(1)
    {
        camera_get_img(); //摄像头获取图像    
        img_extract(img, imgbuff, CAMERA_SIZE);//解压图像到img数组中
        GetCenter();
        GetError();//获取偏差
        
#ifdef SENDIMG
        vcan_sendimg(img, sizeof(img));//发送灰度图像
#endif

#ifdef CHANGESPEED
      
      static char v=50;//注意这里，要定义成static 
      char ch;
      char str[10];
        
       if(uart_querychar (VCAN_PORT, &ch) != 0)//查询接收1个字符    
       {     
         if(ch == '-')
         {
              v-=5;
              ftm_pwm_duty(FTM2, FTM_CH0, v);//其实应该调用的是这个改变duty的函数，而不是初始化函数！
              IntToStr(v, str);
              uart_putstr (VCAN_PORT, (uint8_t *)str); 
         }
         else if(ch == '+')
         {
              v+=5;
              ftm_pwm_duty(FTM2, FTM_CH0, v);
              IntToStr(v, str);
              uart_putstr (VCAN_PORT, (uint8_t *)str); 
         }      
       }
#endif
       
#ifdef PRINTFRE
       if(cnvtime != 0)    
       {    
           //捕捉频率 = bus 时钟 / (分频系数) / cnvtime    
           //分频系数 是 由 初始化时传递进去的分频因子FTM_PS_e 决定，分频系数 = 1<<FTM_PS_e    
           //最大支持频率为 bus 时钟 四分之一    
  
           printf("\n捕捉到频率为：%d",bus_clk_khz*1000/(1<<FTM_PS_2)/cnvtime);    
         
           //等需要采集脉冲的时候，就开中断，过一段时间就可以获取到相应的值    
           cnvtime = 0;    
  
           FTM0_STATUS = 0x00;            //清中断标志位    
           FTM_IRQ_EN(FTM0,FTM_CH0);    
  
       }    
       else   
       {    
            printf("\n没有捕捉到频率");    
       }
#endif
       
    }
}



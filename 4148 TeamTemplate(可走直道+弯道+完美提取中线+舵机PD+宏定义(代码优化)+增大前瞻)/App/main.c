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

#include "common.h"
#include "include.h"
#include "handler.h"

//CAMERA_SIZE��CAMERA_H��CAMERA_H������VCAN_OV7725_Eagle.h����ĺ�
uint8 imgbuff[CAMERA_SIZE];//ѹ����Ķ�ֵ��ͼ��
uint8 img[CAMERA_H][CAMERA_W];//��ѹ��ĻҶ�ͼ��
uint8 img2[CAMERA_H][CAMERA_W];
volatile uint32 cnvtime = 0;                //���벶׽ֵ 

/*************************************************************************
*  �������ƣ�IntToStr
*  ����˵������������ת��Ϊ�ַ���
*  ����˵����num-�����������str-��Ӧд����ַ���
*  �������أ���
*  �޸�ʱ�䣺2018��4��11�� 
*  �޸���  ��CLAY   
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
*  �������ƣ�main
*  ����˵�������������Լ���ȥ
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��3��18�� 
*  �޸���  ��CLAY   
*************************************************************************/
void  main(void)
{
    //��ʼ��
    SteerInit();//�����ʼ�����ź�PA12
    MotorInit();//�����ʼ�����ź�PA10
    RoadInit(); //��·��ʼ��
    camera_init(imgbuff);
    ftm_input_init(FTM0, FTM_CH6, FTM_Falling,FTM_PS_2);        //��ʼ��FTM���벶׽ģʽ���½��ز�׽(FTM0_CH6 Ϊ PTD6)
    
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //���� DMA0 ���жϷ�����Ϊ PORTA_IRQHandler
    
#ifdef PRINTFRE
    set_vector_handler(FTM0_VECTORn ,FTM0_INPUT_IRQHandler);//����FTM0���жϷ�����Ϊ FTM0_INPUT_IRQHandler  
    enable_irq (FTM0_IRQn);  //ʹ��FTM0�ж�
    port_init_NoALT(PTD6,PULLUP);  
#endif

    
    while(1)
    {
        camera_get_img(); //����ͷ��ȡͼ��    
        img_extract(img, imgbuff, CAMERA_SIZE);//��ѹͼ��img������
        GetCenter();
        GetError();//��ȡƫ��
        
#ifdef SENDIMG
        vcan_sendimg(img, sizeof(img));//���ͻҶ�ͼ��
#endif

#ifdef CHANGESPEED
      
      static char v=50;//ע�����Ҫ�����static 
      char ch;
      char str[10];
        
       if(uart_querychar (VCAN_PORT, &ch) != 0)//��ѯ����1���ַ�    
       {     
         if(ch == '-')
         {
              v-=5;
              ftm_pwm_duty(FTM2, FTM_CH0, v);//��ʵӦ�õ��õ�������ı�duty�ĺ����������ǳ�ʼ��������
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
           //��׽Ƶ�� = bus ʱ�� / (��Ƶϵ��) / cnvtime    
           //��Ƶϵ�� �� �� ��ʼ��ʱ���ݽ�ȥ�ķ�Ƶ����FTM_PS_e ��������Ƶϵ�� = 1<<FTM_PS_e    
           //���֧��Ƶ��Ϊ bus ʱ�� �ķ�֮һ    
  
           printf("\n��׽��Ƶ��Ϊ��%d",bus_clk_khz*1000/(1<<FTM_PS_2)/cnvtime);    
         
           //����Ҫ�ɼ������ʱ�򣬾Ϳ��жϣ���һ��ʱ��Ϳ��Ի�ȡ����Ӧ��ֵ    
           cnvtime = 0;    
  
           FTM0_STATUS = 0x00;            //���жϱ�־λ    
           FTM_IRQ_EN(FTM0,FTM_CH0);    
  
       }    
       else   
       {    
            printf("\nû�в�׽��Ƶ��");    
       }
#endif
       
    }
}



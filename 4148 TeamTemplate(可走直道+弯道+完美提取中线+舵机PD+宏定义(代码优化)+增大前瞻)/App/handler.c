#include "handler.h"
#include "main.h"


/*************************************************************************
*  �������ƣ�PORTA_IRQHandler
*  ����˵��������ͷ�ĳ��жϺ����ж�
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��3��18�� 
*  �޸���  ��CLAY   
*************************************************************************/
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif


}

/*************************************************************************
*  �������ƣ�DMA0_IRQHandler()
*  ����˵����DMAͼ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��3��18�� 
*  �޸���  ��CLAY   
*************************************************************************/
void DMA0_IRQHandler()
{
    camera_dma();
}

/*************************************************************************
*  �������ƣ�FTM0_INPUT_IRQHandler()
*  ����˵����������M������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��4��17�� 
*  �޸���  ��CLAY   
*************************************************************************/
 void FTM0_INPUT_IRQHandler()    
 {    
     static uint8 flag = 0;    
     uint8 s = FTM0_STATUS;          //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.    
     uint8 CHn;    
    
     FTM0_STATUS = 0x00;    
    
     CHn = 6;    
     if( s & (1 << CHn) )    
     {    
         /*     �û�����       */   
         if(flag == 0)    
         {    
              //��һ�ν�������ʼ��ʱ    
              flag++;    
              ftm_input_clean(FTM0);            //�� ����������ֵ    
          }    
          else if(flag == 1)    
          {    
              //�ڶ��β⵽�������ڵ�ʱ��    
              cnvtime = ftm_input_get(FTM0, FTM_CH6); //����    
              flag = 0;    
     
              FTM_IRQ_DIS(FTM0,FTM_CH0);      //�ر�FTM0_CH0 �ж�  ������Ƶ���жϣ�    
          }    
          /*********************/   
      }    
  }

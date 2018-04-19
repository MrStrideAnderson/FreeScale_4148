#include "common.h"
#include "include.h"
#include "main.h"
#include "road.h"

uint8 BlackLeft[60];//��ڰ׽�����λ��
uint8 BlackRight[60];//�Һڰ׽�����λ��
uint8 Middle[60];//����λ��
uint8 LGetFlag[60];   //��߻�ȡ���߱�־λ          
uint8 RGetFlag[60];   //�ұ߻�ȡ���߱�־λ

/*************************************************************************
*  �������ƣ�InitRoad
*  ����˵������ʼ����·����ز���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��4��17��
*  �޸���  ��CLAY   
*************************************************************************/
void RoadInit()
{
  uint8 i;
  
  for(i=ROW-1; i>0; i--)
  {
    Middle[i] = MID;
    BlackLeft[i] = 0;
    BlackRight[i] = 0;
  }
}

/*************************************************************************
*  �������ƣ�GetCenter
*  ����˵������ȡ����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��4��11�� 2018��4��17��
*  �޸���  ��CLAY   
*************************************************************************/
void GetCenter()
{ 
   uint8 i, j;
   
   //(ע���ˣ������־λ���ÿ�ε���0)(���Һ��߽߱��Ǹ���������ν����Ϊ�Ḳ��) 
   for(i=ROW-1; i>0; i--)//��ʼ����־λ
   {
     LGetFlag[i] = 0;
     RGetFlag[i] = 0;
   }
   
   for(i=ROW-1; i>0; i--)//�������һ�п�ʼ
   {
     if(i == ROW-1)//����������һ��,��39�п�ʼɨ
     {
       for(j=MID; j>0; j--)//ɨ����һ����߽�
       {
         if(img[i][j]==WhitePoint && img[i][j-1]==BlackPoint && img[i][j-2]==BlackPoint)//��߽�
         {
           BlackLeft[i] = j;//��¼�߽�λ��
           LGetFlag[i] = 1;//��λ��־λ
           break;//������break
         }
       }
       for(j=MID; j<COL; j++)//ɨ����һ���ұ߽�
       {
         if(img[i][j]==WhitePoint && img[i][j+1]==BlackPoint && img[i][j+2]==BlackPoint)//�ұ߽�
         {
           BlackRight[i] = j;
           RGetFlag[i] = 1;
           break;
         }
       }
     }
     
     else//����������
     {
       for(j=Middle[i+1]; j>0; j--)//����һ�е��е㿪ʼɨ������߽�
       {
         if(img[i][j]==WhitePoint && img[i][j-1]==BlackPoint && img[i][j-2]==BlackPoint)//��߽�
         {
           BlackLeft[i] = j;//��¼�߽�λ��
           LGetFlag[i] = 1;//��λ��־λ
           break;//������break
         }
       }
       
       for(j=Middle[i+1]; j<COL; j++)//����һ�е��е㿪ʼɨ�����ұ߽�
       {
         if(img[i][j]==WhitePoint && img[i][j+1]==BlackPoint && img[i][j+2]==BlackPoint)//�ұ߽�
         {
           BlackRight[i] = j;
           RGetFlag[i] = 1;
           break;
         }
       }
     }
   }
   
   for(i=ROW-1; i>0; i--)
   {
     if(LGetFlag[i] && RGetFlag[i])//���ұ߽綼ȡ����
     {       
       Middle[i] = (BlackLeft[i]+BlackRight[i])/2;
     }
     else if(!LGetFlag[i] && RGetFlag[i])//��߽�ûȡ��
     {
       Middle[i] = (0 + BlackRight[i])/2;
     }
     else if(LGetFlag[i] && !RGetFlag[i])//�ұ߽�ûȡ��
     {
       Middle[i] = (BlackLeft[i] + 79)/2;
     }
     else if(!LGetFlag[i] && !RGetFlag[i])//���Ҷ�ûȡ��
     {
       Middle[i] = MID;
     }
   }
   
#ifdef SENDIMG
   for(i=ROW-1; i>0; i--)
   {
     img[i][Middle[i]] = BlackPoint;//��λ����������
   }
#endif
}
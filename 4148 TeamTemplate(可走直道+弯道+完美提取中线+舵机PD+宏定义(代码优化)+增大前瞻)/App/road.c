#include "common.h"
#include "include.h"
#include "main.h"
#include "road.h"

uint8 BlackLeft[60];//左黑白交界线位置
uint8 BlackRight[60];//右黑白交界线位置
uint8 Middle[60];//中线位置
uint8 LGetFlag[60];   //左边获取边线标志位          
uint8 RGetFlag[60];   //右边获取边线标志位

/*************************************************************************
*  函数名称：InitRoad
*  功能说明：初始化道路的相关参数
*  参数说明：无
*  函数返回：无
*  修改时间：2018年4月17日
*  修改人  ：CLAY   
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
*  函数名称：GetCenter
*  功能说明：获取中线
*  参数说明：无
*  函数返回：无
*  修改时间：2018年4月11日 2018年4月17日
*  修改人  ：CLAY   
*************************************************************************/
void GetCenter()
{ 
   uint8 i, j;
   
   //(注意了，这个标志位务必每次得清0)(左右黑线边界那个倒是无所谓，因为会覆盖) 
   for(i=ROW-1; i>0; i--)//初始化标志位
   {
     LGetFlag[i] = 0;
     RGetFlag[i] = 0;
   }
   
   for(i=ROW-1; i>0; i--)//从最底下一行开始
   {
     if(i == ROW-1)//如果是最底下一行,从39列开始扫
     {
       for(j=MID; j>0; j--)//扫底下一行左边界
       {
         if(img[i][j]==WhitePoint && img[i][j-1]==BlackPoint && img[i][j-2]==BlackPoint)//左边界
         {
           BlackLeft[i] = j;//记录边界位置
           LGetFlag[i] = 1;//置位标志位
           break;//别忘了break
         }
       }
       for(j=MID; j<COL; j++)//扫底下一行右边界
       {
         if(img[i][j]==WhitePoint && img[i][j+1]==BlackPoint && img[i][j+2]==BlackPoint)//右边界
         {
           BlackRight[i] = j;
           RGetFlag[i] = 1;
           break;
         }
       }
     }
     
     else//处理其他行
     {
       for(j=Middle[i+1]; j>0; j--)//从上一行的中点开始扫本行左边界
       {
         if(img[i][j]==WhitePoint && img[i][j-1]==BlackPoint && img[i][j-2]==BlackPoint)//左边界
         {
           BlackLeft[i] = j;//记录边界位置
           LGetFlag[i] = 1;//置位标志位
           break;//别忘了break
         }
       }
       
       for(j=Middle[i+1]; j<COL; j++)//从上一行的中点开始扫本行右边界
       {
         if(img[i][j]==WhitePoint && img[i][j+1]==BlackPoint && img[i][j+2]==BlackPoint)//右边界
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
     if(LGetFlag[i] && RGetFlag[i])//左右边界都取到了
     {       
       Middle[i] = (BlackLeft[i]+BlackRight[i])/2;
     }
     else if(!LGetFlag[i] && RGetFlag[i])//左边界没取到
     {
       Middle[i] = (0 + BlackRight[i])/2;
     }
     else if(LGetFlag[i] && !RGetFlag[i])//右边界没取到
     {
       Middle[i] = (BlackLeft[i] + 79)/2;
     }
     else if(!LGetFlag[i] && !RGetFlag[i])//左右都没取到
     {
       Middle[i] = MID;
     }
   }
   
#ifdef SENDIMG
   for(i=ROW-1; i>0; i--)
   {
     img[i][Middle[i]] = BlackPoint;//上位机画出中线
   }
#endif
}
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

#include "improc.h"

extern EIT_PARAM gParam;
extern imUINT8 Image_Data[CAMERA_H][CAMERA_W];
imUINT8 Image_DataF[CAMERA_H][CAMERA_W];
imINT32 gDir_Near=0;
imINT32 gDir_Mid =0;
imINT32 gDir_Far =0;
imINT16 HBoundL[CAMERA_H];
imINT16 HBoundR[CAMERA_H];
imINT16 HBoundM[CAMERA_H];
imINT16 HBoundM_F[CAMERA_H];
imINT16 HBoundM_REAL[CAM_MAX_LENGTH_CM+1];
imINT32 H_Min=H_START;
imINT32 H_MaxL=H_END;
imINT32 H_MaxR=H_END;
imINT32 D_Max =CAM_MAX_LENGTH_CM;
int MaxWhite=0;
int Out=0;
int MaxStart=W_START;
int MaxEnd=W_START;
int S=0; //S ·��־λ
int pixel_change=0; //���ص�ͻ�����
int num=0; //���ڳ����ж��Ƿ���Բ��
int Circle=0;
int judge_startline1(); //��������,�����������ߴ���ʶ��ΪԲ�� ����ʮ��·
void Graph_ExpandImag(unsigned char* src,int width, int height,int strutWidth, int structHeigh);

/*************************************************************************
*  �������ƣ�Graph_JudgeOut
*  ����˵�����жϳ���
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��3��19��
*  �޸���  ��CLAY   
*************************************************************************/
int Graph_JudgeOut(void)
{
	static int LeftOut=0;
	int WhiteCnt;
	int W;
	int wStart,wEnd;
	MaxWhite=0;
	WhiteCnt=0;
	wStart=W_START;
	wEnd=W_START;
	pixel_change=0;
	for(W=W_START;W<W_END;W++)
	{
		if((Image_Data[H_END][W]>Image_Data[H_END][W+1]||Image_Data[H_END][W]<Image_Data[H_END][W+1])&&(W+1<=W_END)) //�ױ�ڻ��ߺ�����
		{
			pixel_change++;
		}
		if(Image_Data[H_END][W]==WHITE_VAL&&Image_Data[H_END][W+1]==WHITE_VAL)
			WhiteCnt++;
		else
		{
			if(WhiteCnt>MaxWhite)
			{
				wEnd=W;
				MaxWhite=WhiteCnt;
				MaxStart=wStart;
				MaxEnd=wEnd;
			}
			WhiteCnt=0;
			wStart=W;
		}
	}
	if(WhiteCnt>MaxWhite)
	{
		wEnd=W;
		MaxWhite=WhiteCnt;
		MaxStart=wStart;
		MaxEnd=wEnd;
	}
	switch(Out)
	{
              case 0:
                if(MaxWhite<=40)
                {
                        if(MaxStart<=W_START)
                        {
                                Out=1;
                                LeftOut=1;
                        }
                        else if (MaxEnd>=W_END)
                        {
                                Out=1;
                                LeftOut=0;
                        }
                }
                break;
              case 1:
                if(MaxWhite>=70)
                {
                        if(LeftOut==1 && (MaxStart<=W_START))
                                Out=0;
                        if(LeftOut==0 && (MaxEnd>=W_END))
                                Out=0;
                }
                break;
	}
	return Out;
}


/*************************************************************************
*  �������ƣ�FindMidLine
*  ����˵����Ѱ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2018��3��18��      2018��3��19�� 
*  �޸���  ��CLAY   
*************************************************************************/
int Graph_FindMidLine()
{
	/********�ұ��߱���********/
	int H,W;
	int findLline=0; //�ҵ����߱�־
	int findRline=0; //�ҵ����߱�־
	imINT16 BoundL[CAMERA_H];
	imINT16 BoundR[CAMERA_H];
	int dDir0,dDir1; //ʮ��·����
	int GetLineCntL=0;
	int GetLineCntR=0;
	int Mid; //ÿ�θ��µ�����
	int H1,H2,H3,H4,H5,H6;
	/**************************/

	/*******�ж��Ƿ����*******/
	if(Graph_JudgeOut()&&(!(pixel_change>6)))
		return 0;
	else
	{
		for (H=0;H<=CAMERA_H;H++)
		{
			HBoundL[H]=MaxStart;
			HBoundR[H]=MaxEnd;
			HBoundM[H]=(MaxStart+MaxEnd)/2;
		}
		// H_Min = H_START; //������û���ã�ֻ��Ϊ�˳������ȷִ�У���Ϊ��� else ��û����䣬����Բ��������
	}
	/**************************/

	for(H=110;H>30;H--)
	{
		if(Image_Data[H][W_MID]==WHITE_VAL&&Image_Data[H-1][W_MID]==BLACK_VAL&&H_circle==0) //��¼ͼ����������û������
		{
			num=1;
			H_circle=120-(H-1);
			//break;
		}
		if(H>10&&H<60)
		{
			if(Image_Data[H][W_MID]==WHITE_VAL&&Image_Data[H-1][W_MID]==BLACK_VAL&&point_A==0)
			{
				point_A=H-1;
				//EnBuzzer();
			}
			if(Image_Data[H][W_MID]==WHITE_VAL&&Image_Data[H+1][W_MID]==BLACK_VAL&&point_B==0)
			{
				point_B=H+1;
				//EnBuzzer();
			}
			if(point_A!=0&&point_B!=0)
				break;
		}
	}
	if(point_A-point_B>15&&(point_A>25&&point_B<25))
	{
		center=(point_A+point_B)/2;
	}
	S=0;
	//DisBuzzer();
	for(H=H_END;H>=H_START;H--)
	{
		/******S ·ʶ��*******/
		if(Image_Data[H][79]==WHITE_VAL)
			White_pixel++;
		/********************/
		
		H1=int_min(H+1,H_END);
		H2=int_min(H+2,H_END);
		H3=int_min(H+3,H_END);
		H4=int_min(H+4,H_END);
		H5=int_min(H+5,H_END);
		H6=int_min(H+6,H_END);
		Mid=HBoundM[H+1];
		
		if(Image_Data[H][Mid]!=WHITE_VAL || Image_Data[H][Mid-1]!=WHITE_VAL || Image_Data[H][Mid+1]!=WHITE_VAL)
		{
			Mid = W_START+(W_END>>2);
			if(Image_Data[H][Mid]!=WHITE_VAL || Image_Data[H][Mid-1]!=WHITE_VAL || Image_Data[H][Mid+1]!=WHITE_VAL)
			{
				Mid = W_END-(W_END>>2);
				if(Image_Data[H][Mid]!=WHITE_VAL || Image_Data[H][Mid-1]!=WHITE_VAL || Image_Data[H][Mid+1]!=WHITE_VAL)
				{
					Mid = W_START+(W_END>>4);
					if(Image_Data[H][Mid]!=WHITE_VAL || Image_Data[H][Mid-1]!=WHITE_VAL || Image_Data[H][Mid+1]!=WHITE_VAL)
					{
						Mid = W_END-(W_END>>4);
						if(Image_Data[H][Mid]!=WHITE_VAL||
						Image_Data[H][Mid-1]!=WHITE_VAL || Image_Data[H][Mid+1]!=WHITE_VAL)
						{
							HBoundL[H] = HBoundL[H+1] ;
							HBoundR[H] = HBoundR[H+1] ;
							HBoundM[H] = HBoundM[H+1] ;
							continue;
						}
					}
				}
			}
		}
		
		/***************������***************/
		for(W=W_START;W<Mid;W++)
		{
			if (Image_Data[H][W]<Image_Data[H][W+1])
			{
				HBoundL[H] = W;
				// if(int_abs(HBoundL[H]-HBoundL[H1])>5)
				// HBoundL[H]=HBoundL[H1];
				GetLineCntL++;
				break;
			}
		}
		if(W==Mid)
		{
			lastL=nowL;
			nowL=1;
			if(((H>121-H_circle)&&(H<121-H_circle+20))&&(lastL==1&&nowL==1)&&num==1)
				no_getLineL++;
			HBoundL[H] =HBoundL[H1];
		}
		/***********************************/
		
		/***************������***************/
		for(W=W_END;W>Mid;W--)
		{
			if (Image_Data[H][W]<Image_Data[H][W-1])
			{
				HBoundR[H] = W;
				// if(int_abs(HBoundR[H]-HBoundR[H1])>5)
				// HBoundR[H]=HBoundR[H1];
				GetLineCntR++;
				break;
			}
		}
		if(W==Mid)
		{
			lastR=nowR;
			nowR=1;
			if(((H>121-H_circle)&&(H<121-H_circle+20))&&(lastR==1&&nowR==1)&&num==1)
				no_getLineR++;
			HBoundR[H] =HBoundR[H1] ;
		}
		if(Image_Data[center][W_MID]==BLACK_VAL&&Image_Data[center][W_MID-1]==BLACK_VAL&&Image_Data[center][W_MID+1]==BLACK_VAL&&Image_Data[center][W_MID-5]==BLACK_VAL&&Image_Data[center][W_MID+5]==BLACK_VAL&&Image_Data[center][W_MID+15]==BLACK_VAL&&Image_Data[center][W_MID-15]==BLACK_VAL&&Image_Data[center][25]==WHITE_VAL&&Image_Data[center][135]==WHITE_VAL&&(no_getLineR>10)&&(no_getLineL>10))
		{
			EnBuzzer();
			Circle=1;
		}
		/***********************************/
		
		/**************ʮ��·***************/
		dDir0=HBoundR[H]-HBoundR[H2];
		dDir1=HBoundR[H2]-HBoundR[H6];
		if((dDir0*dDir1<-4 && dDir0>0)/*&&(!startline) */|| ((dDir0 > 10&& dDir1<=0)/*&&(!startline)*/)) //(!startline):�������ߴ�����ʶ��Ϊʮ��·�����Լ���(!startline)
		{
			//ʮ��·������
			HBoundR[H]=HBoundR[H3]+HBoundR[H3]-HBoundR[H6];
			HBoundR[H1]=HBoundR[H4]+HBoundR[H3]-HBoundR[H6];
		}
		dDir0=HBoundL[H]-HBoundL[H2];
		dDir1=HBoundL[H2]-HBoundL[H6];
		if((dDir0*dDir1<-4 && dDir0<0)/*&&(!startline)*/ || ((dDir0 <-10&& dDir1>=0)/*&&(!startline)*/))
		{
			//ʮ��·������
			HBoundL[H]=HBoundL[H3]+HBoundL[H3]-HBoundL[H6];
			HBoundL[H1]=HBoundL[H4]+HBoundL[H3]-HBoundL[H6];
		}
		HBoundM[H]= (HBoundR[H]+ HBoundL[H])/2;
		if( HBoundR[H] < W_START+LINE_WIDTH || HBoundL[H] > W_END-LINE_WIDTH || (HBoundR[H]-HBoundL[H]) <LINE_WIDTH )
		{
			H_Min = H;
			break;
		}
		/***********************************/
	}
	
	/******S ·ʶ��*******/
	sum=0;
	for(H=H_END;H>60;H--)
	{
		sum+=HBoundM[H];
	}
	S_Dir=sum/(H_END-60)-79;
	if(White_pixel>100&&((S_Dir<25&&S_Dir>1)||(S_Dir>-25&&S_Dir<-1))&&(int_abs((HBoundM[60]+HBoundM[H_END])/2-HBoundM[90])>5))
	{
		S=1; //S ·��־λ
		//EnBuzzer(); //��������
	}
	/********************/
	
	/********************Բ��*********************/
	int countbl=0,countbr=0;
	for(W=W_START;W<W_END;W++)
	{
		if((Image_Data[121-H_circle-2][W]>Image_Data[121-H_circle-2][W+1]||Image_Data[121-H_circle-2][W]<Image_Data[121-H_circle-2][W+1])&&(W+1<=W_END)) //�ױ�ڻ��ߺ�����
		{
			pixel_change1++; //���ص�ͻ�����
		}
	}
	if(num==1&&(pixel_change1<6))
	{
		for(H=120-H_circle;H<=H_END;H++)
		{
			if(Image_Data[H][0]==WHITE_VAL)
			{
				countL++;
				//woxiedelalalallallalla
				if(countL>=H_circle-30)
					break;
			}
			else
			{ 
				countbl++;
				if(countbl>4)
				{
					countL=0;
					countbl=0;
					break;
				}
			}
		}
		for(H=H_END;H>=120-H_circle;H--)
		{
			if(Image_Data[H][159]==WHITE_VAL)
			{
				countR++;
				//woxiedelalalallallalla
				if(countR>=H_circle-30)
				break;
			}
			else
			{
				countbr++;
				if(countbr>4)
				{
					countR=0;
					countbr=0;
					break;
				}
			}
		}
	}
	//Circle=0;
	// int a,b,c,d;
	// for(H=H_END;H>0;H--)
	// {
	//
	// if(Image_Data[H][W_MID]==WHITE_VAL&&Image_Data[H-1][W_MID]==BLACK_VAL) //��¼ͼ����������û������
	// {
	// a=H-1;
	// break;
	// }
	// }
	// for(H=a;H>0;H--)
	// {
	// if(Image_Data[H][W_MID]==BLACK_VAL&&Image_Data[H-1][W_MID]==WHITE_VAL)
	// {
	// b=H;
	// break;
	// }
	// }
	// for(H=b;H>0;H--)
	// {
	// if(Image_Data[H][W_MID]==WHITE_VAL&&Image_Data[H-1][W_MID]==BLACK_VAL)
	// {
	// d=H;
	// break;
	// }
	// }
	// c=a-b;
	//if((countL>=H_circle-30&&(countR>=H_circle-30)/*&&Image_Data[120-H_circle-3][159]==WHITE_VAL*/&&H_circle&&Image_Data[35][40]!=WHITE_VAL&&(GetLineCntR<H_circle))||((d>5)&&(c>40)&&(countL>=30)&&(countR>=30)&&H_circle&&Image_Data[35][40]!=WHITE_VAL&&(GetLineCntR<H_circle))/*&&(!startline)*/)
	//H_circle&&Image_Data[35][40]!=WHITE_VAL:��ʮ��·���������Ƚ�б��ʮ��·����Բ�����ڲ�࣬���Լ�������������

	if(Circle==1||((H>30&&H<=80)&&(countL>=H_circle-30)&&(countR>=H_circle-30)&&H_circle&&Image_Data[35][40]!=WHITE_VAL&&(GetLineCntR<H_circle))||((H>30&&H<=110)&&(countL>=H_circle-5)&&(countR>=H_circle-5)&&H_circle&&Image_Data[35][40]!=WHITE_VAL)||(no_getLineL>10&&no_getLineR>10&&Image_Data[25][55]!=WHITE_VAL))
	//H_circle&&Image_Data[35][40]!=WHITE_VAL:��ʮ��·���������Ƚ�б��ʮ��·����Բ�����ڲ�࣬���Լ�������������
	{
		Circle=1;
		if(direction)
		{
			for(H=H_END;H>=H_START;H--)
			//Բ�������ߣ���Բ����ת
			{
				HBoundR[H]=160-(H-(120-H_circle))*160/(double)(H_circle);
				HBoundL[H]=159;
				if(H<(110-H_circle))
				{
					HBoundR[H]=159;
				}
				HBoundM[H]=(HBoundL[H]+HBoundR[H])/2;
			}
		}
		else
		{
			for(H=H_END;H>=H_START;H--)
			//Բ��������
			{
				HBoundL[H]=(H-(120-H_circle))*160/(double)(H_circle);
				HBoundR[H]=0;
				if(H<(110-H_circle))
				{
					HBoundL[H]=0;
				}
				HBoundM[H]=(HBoundL[H]+HBoundR[H])/2;
			}
		}
	}
	/*********************************************/
	
	/*******************�ϰ���********************/
	for(H=100;H>20;H--) //50��ʼ�� 100 �� 40
	{
		
		/********ʶ�����ϰ���*******/
		Lbarrier_Weight=0;
		Lchange=0;
		//�����,������һ�� Rchange=1;
		for(W=HBoundM[H];W>HBoundL[H];W--) //������ϰ���
		{
			if (Image_Data[H][W]>Image_Data[H][W-1]||Lchange)
			{
				Lchange=1; //
				�а׵��ڵ�����
				if(Lchange==1&&Image_Data[H][W-1]==BLACK_VAL&&Image_Data[H][W-2]==BLACK_VAL)
					Lbarrier_Weight++; //�ϰ�����
				else
				{
					Lbarrier_end=W;
					Lbarrier_H=H;
					Lchange=0;
					break;
				}
			}
		}
		Lbarrierstart=Lbarrier_end+Lbarrier_Weight; //�ϰ��￿���������ĵ�һ��
		/****************************/
		
		/********ʶ�����ϰ���********/
		Rbarrier_Weight=0;
		Rchange=0; //�����,������һ�� Rchange=1;
		for(W=HBoundM[H];W<HBoundR[H];W++) //������ϰ���
		{
			if (Image_Data[H][W]>Image_Data[H][W+1]||Rchange)
			{
				Rchange=1; //�а׵��ڵ�����
				if(Rchange==1&&Image_Data[H][W+1]==BLACK_VAL&&Image_Data[H][W+2]==BLACK_VAL)
					Rbarrier_Weight++; //�ϰ�����
				else
				{
					Rbarrier_end=W;
					Rbarrier_H=H;
					Rchange=0;
					break;
				}
			}
			if(HBoundM[H]-HBoundM[H-1]>10) //���������������ƣ���Բ�������Χ������̫����Բ��������ʶ��Ϊ�ϰ��������� if ��������ų���Χ�����ĸ���
			{
				no=0; //�������ϰ��������Χ��������
			}
		}
		Rbarrierstart=Rbarrier_end-Rbarrier_Weight; //�ϰ��￿���������ĵ�һ��
		/***************************/
		
		if(Lbarrier_Weight>20&&HBoundM[Lbarrier_H]!=BLACK_VAL) //�ų�Զ������ ��HBoundM[Rbarrier_H]!=BLACK_VAL
		{
			Lbarrier=1;
			break;
		}
		if(Rbarrier_Weight>20&&HBoundM[Rbarrier_H]!=BLACK_VAL)
		//Lbarrier_Weight ��Ϊ 10 ʱ�������߻ᵱ���ϰ�����Ը�Ϊ 20
		{
			Rbarrier=1;
			break;
		}
	}
	
	/*******************�ϰ��ﲹ��********************/
	if(Lbarrier==1&&no&&(Lbarrier_H>H_Min))
	//(Lbarrier_H>H_Min&&(H_Min!=H_START)):�ų�������ţ�����ʱ������ʶ��Ϊ�ϰ���
	{
		for(H=Lbarrier_H;H<H_END;H++)
		{
			HBoundL[H]=Lbarrierstart-(H-Lbarrier_H)*Lbarrierstart/(H_END-Lbarrier_H)+10; //�ϰ��ﲹ��
		}
		for(H=Lbarrier_H-1;H>=H_START;H--)
		{
			HBoundL[H]=HBoundL[H+1];
		}
	}
	if(Rbarrier==1&&no&&(Rbarrier_H>H_Min))
	{
		for(H=Rbarrier_H;H<H_END;H++)
		{
			HBoundR[H]=Rbarrierstart+(H-Rbarrier_H)*(160-Rbarrierstart)/(H_END-Rbarrier_H)-10;
		}
		for(H=Rbarrier_H-1;H>=H_START;H--)
		{
			dR[H]=HBoundR[H+1];
		}
	}
	for(H=H_END;H>=H_START;H--) //�ϰ������߹滮
	{
		HBoundM[H]=(HBoundL[H]+HBoundR[H])/2;
	}
	/**********************************************/
	/*for(H=int_min(H_MaxL,H_MaxR);H<=H_END;H++)
	{
	HBoundM[H]=(HBoundL[H]+HBoundR[H])/2;
	}*/
	return S;
}
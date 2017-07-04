#include <ansi_c.h>
#include <userint.h>
#include "supermaster.h" 
#include <utility.h>
#include "global.h" 
#include "XGSPON_OLT.h"

int XGSPON_OLT_ENTRY_Password (int handle)
{
	int error=0; 
	
	error = I2C_4BYTEs_WRITE_DLL (handle, 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target !");  
		return -1;
	}
	
	return 0;
}  

int XGSPON_OLT_GET_FirmVersion(int handle, char *str)
{
	int error=0;    
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "MCU_GET_VERSION()");  
	
	error = SetCommand(handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	return 0;
}

int XGSPON_OLT_Set_10G_TX_ON(int handle)                   //打开10G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 110, &temp);
	if (error)
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	}
	
	g_temp=temp&0xbf;
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 110, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 
	
	return 0;	 
}

int XGSPON_OLT_Set_10G_TX_OFF(int handle)                  //关闭10G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 110, &temp);
	if (error)
	{
		return -1;
	}
	
	g_temp=temp|0x40;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 110, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 

	return 0;
}

int XGSPON_OLT_Set_1G_TX_ON(int handle)                    //打开1G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 111, &temp);
	if (error)
	{
		return -1;
	}
	
	g_temp=temp&0xfe;
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 111, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 

	return 0;	
}

int XGSPON_OLT_Set_1G_TX_OFF(int handle)                   //关闭1G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 111, &temp);
	if (error)
	{
		return -1;
	}
	
	g_temp=temp|0x01;
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 111, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 
	return 0;		
}

int XGSPON_OLT_Update_BASE(int handle, int tableIndex) 			           //更新表BASE
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_UPDATE_FLASH(BASE,%d)", tableIndex);
	
	SetCommand (handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	} 
	return 0; 	
}

int XGSPON_OLT_Update_LUK(int handle, int tableIndex) 			           //更新表LUK
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_UPDATE_FLASH(LUK,%d)", tableIndex);
	
	SetCommand (handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	} 
	return 0; 	
}

/**************************************************XGPON OLT**************************************************/
/***************************************XGPON APC************************************/
int XGSPON_OLT_SET_10G_APC_Mode (int handle, enum LUT_MODE Mode)	//设置XGPON APC模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x)", XGSPON_OLT_10G_APC_MODE_ADD, 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x)", XGSPON_OLT_10G_APC_MODE_ADD, 'A');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int XGSPON_OLT_GET_10G_APC(int handle, unsigned short* GET_APC)	//读取XGPON APC值 
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  APC_LSB=0;
	unsigned char  APC_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_GET_TABLE(BASE,0,%d,2)", XGSPON_OLT_10G_APC_DAC_ADD);    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x", &APC_LSB, &APC_MSB);                   
	
	*GET_APC=(((unsigned short)APC_MSB)<<8)+APC_LSB;
				
	return 0;	
}

int XGSPON_OLT_SET_10G_APC(int handle, unsigned short SET_APC)	//设置XGPON APC值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x)", XGSPON_OLT_10G_APC_DAC_ADD, (unsigned char)(SET_APC&0xFF), (unsigned char)((SET_APC>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_SET_10G_APC_Range(int handle, unsigned short APC_MAX, unsigned short APC_MIN)	//设置XGPON APC最大最小值	  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_RANGE_ADD, (unsigned char)(APC_MAX&0xFF), (unsigned char)((APC_MAX>>8)&0xFF), (unsigned char)(APC_MIN&0xFF), (unsigned char)((APC_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_GET_10G_APC_LUT(int handle, int *Temper, float *Slope, float *Offset)	//读取XGPON APC LUK       
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=0;index<2;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, index*XGSPON_OLT_LUT_BYTE_COUNT, XGSPON_OLT_LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-XGSPON_OLT_CORE_TEMP_LUT_OFFSET;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/XGSPON_OLT_10G_APC_SLOPE_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x); 
	}
	return 0;
}

int XGSPON_OLT_SET_10G_APC_LUT(int handle, float *DAC, float *Temper)	//写XGPON APC查找表           
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_APC_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APC_SLOPE_AMP);
			
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_APC_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APC_SLOPE_AMP);			
			
	offset_m = (120.+ XGSPON_OLT_CORE_TEMP_LUT_OFFSET)* sSlop[1]/XGSPON_OLT_10G_APC_SLOPE_AMP +  sOffset[1];   
	
	//写查找表
	//第一段
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_APC_LUT_INDEX)<0)
	{
		return -1;
	}

	return 0;
}

int XGSPON_OLT_SET_10G_APC_LUT_High(int handle, float *DAC ,float *Temper)	//写XGPON APC查找表High      
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_APC_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APC_SLOPE_AMP);			

	offset_m = (120+50)* sSlop[1]/XGSPON_OLT_10G_APC_SLOPE_AMP +  sOffset[1]; 
	
	
	//写高温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_APC_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
}

int XGSPON_OLT_SET_10G_APC_LUT_Low(int handle, float *DAC, float *Temper)	//写XGPON APC查找表LOW             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_APC_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APC_SLOPE_AMP);
			
	//写低温段查找表
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APC_LUT_NAME, XGSPON_OLT_10G_APC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_APC_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
} 

int XGSPON_OLT_10G_Fault_OFF(int handle)	//设置APC Fault功能关闭  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"gn2010_write(113,0x5F)");    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_10G_Fault_ON(int handle)	//设置APC Fault功能开启  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"gn2010_write(113,0x1F)");    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

/***************************************XGPON APC************************************/

/***************************************XGPON MOD************************************/
int XGSPON_OLT_SET_10G_MOD_Mode (int handle, enum LUT_MODE Mode)	//设置XGPON MOD查找表模式   
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x);", XGSPON_OLT_10G_MOD_MODE_ADD, 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x);", XGSPON_OLT_10G_MOD_MODE_ADD, 'A');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int XGSPON_OLT_GET_10G_MOD(int handle, unsigned short* GET_MOD)	//读取XGPON MOD值   
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  ER_LSB=0;
	unsigned char  ER_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_GET_TABLE(BASE,0,%d,2)", XGSPON_OLT_10G_MOD_DAC_ADD);      
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x", &ER_LSB, &ER_MSB);                   
	
	*GET_MOD=(((unsigned short)ER_MSB)<<8)+ER_LSB;
	
	return 0;		
}

int XGSPON_OLT_SET_10G_MOD(int handle, unsigned short SET_MOD)	//设置XGPON MOD值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x)", XGSPON_OLT_10G_MOD_DAC_ADD, (unsigned char)(SET_MOD&0xFF), (unsigned char)((SET_MOD>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 
	
	return 0;	
}

int XGSPON_OLT_SET_10G_MOD_Range(int handle, unsigned short MOD_MAX, unsigned short MOD_MIN)	//设置XGPON MOD最大最小值
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_RANGE_ADD, (unsigned char)(MOD_MAX&0xFF), (unsigned char)((MOD_MAX>>8)&0xFF), (unsigned char)(MOD_MIN&0xFF), (unsigned char)((MOD_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_GET_10G_MOD_LUT(int handle, int *Temper, float *Slope, float *Offset)	//读取XGPON MOD LUK       
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=0;index<2;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, index*XGSPON_OLT_LUT_BYTE_COUNT, XGSPON_OLT_LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-XGSPON_OLT_CORE_TEMP_LUT_OFFSET;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/XGSPON_OLT_10G_MOD_SLOPE_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x); 
	}
	return 0;
}

int XGSPON_OLT_SET_10G_MOD_LUT(int handle, float *DAC, float *Temper)	//写XGPON MOD查找表        
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_MOD_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_MOD_SLOPE_AMP);
			
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_MOD_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_MOD_SLOPE_AMP);			
			
	offset_m = (120.+ XGSPON_OLT_CORE_TEMP_LUT_OFFSET)* sSlop[1]/XGSPON_OLT_10G_MOD_SLOPE_AMP +  sOffset[1];   
	
	//写查找表
	//第一段
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_MOD_LUT_INDEX)<0)
	{
		return -1;
	}

	return 0;
}

int XGSPON_OLT_SET_10G_MOD_LUT_High(int handle, float *DAC, float *Temper)	//写XGPON MOD查找表High       
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_MOD_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_MOD_SLOPE_AMP);			

	offset_m = (120+50)* sSlop[1]/XGSPON_OLT_10G_MOD_SLOPE_AMP +  sOffset[1]; 
	
	
	//写高温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_MOD_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
}

int XGSPON_OLT_SET_10G_MOD_LUT_Low(int handle, float *DAC, float *Temper)//写XGPON MOD查找表LOW             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_MOD_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_MOD_SLOPE_AMP);
			
	//写低温段查找表
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_MOD_LUT_NAME, XGSPON_OLT_10G_MOD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_MOD_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
} 
/***************************************XGPON MOD************************************/

/***************************************XGPON APD************************************/
int XGSPON_OLT_SET_10G_APD_Mode (int handle, enum LUT_MODE Mode)	//设置APD模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x)", XGSPON_OLT_10G_APD_MODE_ADD, 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x)", XGSPON_OLT_10G_APD_MODE_ADD, 'A');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int XGSPON_OLT_GET_10G_APD(int handle, unsigned short* GET_APD)	//读取APD值 
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  APD_LSB=0;
	unsigned char  APD_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_GET_TABLE(BASE,0,%d,2)", XGSPON_OLT_10G_APD_DAC_ADD);      
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x", &APD_LSB, &APD_MSB);                   
	
	*GET_APD=(((unsigned short)APD_MSB)<<8)+APD_LSB;
				
	return 0;	
}

int XGSPON_OLT_SET_10G_APD(int handle, unsigned short SET_APD)	//设置APD值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x)", XGSPON_OLT_10G_APD_DAC_ADD, (unsigned char)(SET_APD&0xFF), (unsigned char)((SET_APD>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_SET_10G_APD_Range(int handle, unsigned short APD_MAX, unsigned short APD_MIN)	//设置APD最大最小值	  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_RANGE_ADD, (unsigned char)(APD_MAX&0xFF), (unsigned char)((APD_MAX>>8)&0xFF), (unsigned char)(APD_MIN&0xFF), (unsigned char)((APD_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_GET_10G_APD_LUT(int handle, int *Temper, float *Slope, float *Offset)	//读取APD LUK       
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=0;index<2;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, index*XGSPON_OLT_LUT_BYTE_COUNT, XGSPON_OLT_LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-XGSPON_OLT_CORE_TEMP_LUT_OFFSET;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/XGSPON_OLT_10G_APD_SLOPE_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x); 
	}
	return 0;
}

int XGSPON_OLT_SET_10G_APD_LUT(int handle, float* DAC,float* Temper)	//写APD查找表           
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_APD_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APD_SLOPE_AMP);
			
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_APD_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APD_SLOPE_AMP);			
			
	offset_m = (120.+ XGSPON_OLT_CORE_TEMP_LUT_OFFSET)* sSlop[1]/XGSPON_OLT_10G_APD_SLOPE_AMP +  sOffset[1];   
	
	//写查找表
	//第一段
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_APD_LUT_INDEX)<0)
	{
		return -1;
	}

	return 0;
}

int XGSPON_OLT_SET_10G_APD_LUT_High(int handle, float *DAC, float *Temper)	//写APD查找表High      
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_APD_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APD_SLOPE_AMP);			

	offset_m = (120+50)* sSlop[1]/XGSPON_OLT_10G_APD_SLOPE_AMP +  sOffset[1]; 
	
	
	//写高温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_APD_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
}

int XGSPON_OLT_SET_10G_APD_LUT_Low(int handle, float *DAC, float *Temper)	//写APD查找表LOW             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_APD_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_APD_SLOPE_AMP);
			
	//写低温段查找表
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_APD_LUT_NAME, XGSPON_OLT_10G_APD_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_APD_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
} 
/***************************************XGPON APD************************************/

/***************************************XGPON LOS************************************/
int XGSPON_OLT_SET_10G_LOS_Mode (int handle, enum LUT_MODE Mode)	//设置XGPON LOS模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,%d,0x%x);", XGSPON_OLT_10G_LOS_MODE_ADD, 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,%d,0x%x);", XGSPON_OLT_10G_LOS_MODE_ADD, 'A');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int XGSPON_OLT_GET_10G_LOS(int handle, unsigned short* GET_LOS)	//读取XGPON LOS值 
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  LOS_LSB=0;
	unsigned char  LOS_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_GET_TABLE(BASE,0,%d,2)", XGSPON_OLT_10G_LOS_DAC_ADD);      
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}

	sscanf(strOupt,"0x%x,0x%x", &LOS_LSB, &LOS_MSB);                   
	
	*GET_LOS=(((unsigned short)LOS_MSB)<<8)+LOS_LSB;
				
	return 0;	
}

int XGSPON_OLT_SET_10G_LOS(int handle, unsigned short SET_LOS)	//设置XGPON LOS值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x)", XGSPON_OLT_10G_LOS_DAC_ADD, (unsigned char)(SET_LOS&0xFF), (unsigned char)((SET_LOS>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_SET_10G_LOS_Range(int handle, unsigned short LOS_MAX, unsigned short LOS_MIN)	//设置XGPON LOS最大最小值	  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%d,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_RANGE_ADD, (unsigned char)(LOS_MAX&0xFF), (unsigned char)((LOS_MAX>>8)&0xFF), (unsigned char)(LOS_MIN&0xFF), (unsigned char)((LOS_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_GET_10G_LOS_LUT(int handle, int *Temper, float *Slope, float *Offset)	//读XGPON 取LOS LUK       
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=0;index<2;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, index*XGSPON_OLT_LUT_BYTE_COUNT, XGSPON_OLT_LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-XGSPON_OLT_CORE_TEMP_LUT_OFFSET;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/XGSPON_OLT_10G_LOS_SLOPE_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x); 
	}
	return 0;
}

int XGSPON_OLT_SET_10G_LOS_LUT(int handle, float* DAC, float* Temper)	//写XGPON LOS查找表           
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_LOS_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_LOS_SLOPE_AMP);
			
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_LOS_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_LOS_SLOPE_AMP);			
			
	offset_m = (120.+ XGSPON_OLT_CORE_TEMP_LUT_OFFSET)* sSlop[1]/XGSPON_OLT_10G_LOS_SLOPE_AMP +  sOffset[1];   
	
	//写查找表
	//第一段
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_LOS_LUT_INDEX)<0)
	{
		return -1;
	}

	return 0;
}

int XGSPON_OLT_SET_10G_LOS_LUT_High(int handle, float* DAC, float* Temper)	//写XGPON LOS查找表High      
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_10G_LOS_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_LOS_SLOPE_AMP);			

	offset_m = (120+50)* sSlop[1]/XGSPON_OLT_10G_LOS_SLOPE_AMP +  sOffset[1]; 
	
	
	//写高温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_LOS_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
}

int XGSPON_OLT_SET_10G_LOS_LUT_Low(int handle, float* DAC, float* Temper)	//写XGPON LOS查找表LOW             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_10G_LOS_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_10G_LOS_SLOPE_AMP);
			
	//写低温段查找表
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_10G_LOS_LUT_NAME, XGSPON_OLT_10G_LOS_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_10G_LOS_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
} 
/***************************************XGPON LOS************************************/

/***************************************XGPON RSSI************************************/
int XGSPON_OLT_SET_10G_RSSI_Compensation(int handle, float Kh, float Kl) 				          		//写XGPON Rx的高低温校准参数Kh和Kl 
{
	int error=0;

	INT16U tmp_KH;
	INT16U tmp_KL;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	tmp_KH = Kh*XGSPON_OLT_10G_RSSI_K_AMP;
	tmp_KL = Kl*XGSPON_OLT_10G_RSSI_K_AMP;       
	
	sprintf(strInpt, "MCU_SET_TABLE(base,0,%d,0x%02x,0x%02x,0x%02x,0x%02x)",  XGSPON_OLT_10G_RSSI_KL_ADD, (unsigned char)(tmp_KL&0xFF), (unsigned char)((tmp_KL>>8)&0xFF), (unsigned char)(tmp_KH&0xFF), (unsigned char)((tmp_KH>>8)&0xFF));
	error=SetCommand (handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	
	return 0;	
}

int XGSPON_OLT_SET_10G_RXKL(int handle, float Kl) 				                                  		//写XGPON Rx的低温校准参数和Kl     
{
	int error=0;

	INT16U tmp_KH;
	INT16U tmp_KL;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	tmp_KL = Kl*XGSPON_OLT_10G_RSSI_K_AMP;   

	sprintf(strInpt, "MCU_SET_TABLE(base,0,%d,0x%02x,0x%02x)", XGSPON_OLT_10G_RSSI_KL_ADD, (unsigned char)(tmp_KL&0xFF), (unsigned char)((tmp_KL>>8)&0xFF));
	error=SetCommand (handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	
	return 0;	
}

int XGSPON_OLT_SET_10G_RXKH(int handle, float Kh) 				                                  		//写XGPON Rx的高温校准参数和Kh     
{
	int error=0;

	INT16U tmp_KH;
	INT16U tmp_KL;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	tmp_KH = Kh*XGSPON_OLT_10G_RSSI_K_AMP;
	
	sprintf(strInpt, "MCU_SET_TABLE(base,0,%d,0x%02x,0x%02x)", XGSPON_OLT_10G_RSSI_KH_ADD, (unsigned char)(tmp_KH&0xFF), (unsigned char)((tmp_KH>>8)&0xFF));
	error=SetCommand (handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	
	return 0;	
}

int XGSPON_OLT_GET_10G_RX_ADC(int handle, short *ADC)											  		//读取XGPON RSSI ADC    
{
	int error=0;			  
	int liv_adctemp=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	error = SetCommand (handle, "MCU_GET_ADC(3)", strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt, "0x%x", &liv_adctemp);
	*ADC=liv_adctemp;
	
	return 0;
}

int XGSPON_OLT_SET_10G_RXP_MON(int handle, enum LUT_MODE Mode)									  		//设置XGPON RSSI模式     
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_FORMULA:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x);", XGSPON_OLT_10G_RSSI_MODE_ADD, 'F');
			break;
		}
		
		case LUT_MODE_EXT:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x);", XGSPON_OLT_10G_RSSI_MODE_ADD, 'E');
			break;
		}
		
		case LUT_MODE_TABLE:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x);", XGSPON_OLT_10G_RSSI_MODE_ADD, 'T');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int XGSPON_OLT_SET_10G_RXPOW_TABLE(int handle, short *ADC_Array, double *PWR_Array, int CaliNumber)  	//设置XGPON RSSI查找表  
{
	int  error=0;
	int  index;
	
	INT8U address;    
	
	INT8U  lslope, mslope, loffset, moffset,lADC,mADC;
	INT16U wADC, wSlope, wOffset;

	char strOupt[256];
	char strInpt[256];
	
	double Slope[30];
	double Offset[30];
	short  Rx_OP_Arr[30]={0};
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for (index = 0; index < CaliNumber; index++)
	{
		Rx_OP_Arr[index]=(unsigned short)(pow(10, PWR_Array[index] /10.) / 0.1E-3); 
	}
	
	for (index = 0; index < CaliNumber; index++)
	{	 
		Slope[index]=(Rx_OP_Arr[index+1] - Rx_OP_Arr[index]) / ((double)ADC_Array[index+1] - (double)ADC_Array[index]);
		Offset[index]=(Rx_OP_Arr[index] - Slope[index]*(ADC_Array[index]));  
	}
	
	for (index = 0; index < CaliNumber; index++)
	{
		address = index*6;
		
		if (CaliNumber-1 -1 == index)
		{
			wADC = 0x7FFF;
		}
		else
		{
			wADC = ADC_Array[index + 1];   
		}
		
		wSlope = (Slope[index] * XGSPON_OLT_10G_RSSI_SLOPE_AMP)<-65535 ? 65535:(Slope[index] * XGSPON_OLT_10G_RSSI_SLOPE_AMP);   //溢出保护，最大0xFFFF;
		wOffset = Offset[index];   
		
		lslope = wSlope&0x00ff;
		mslope = wSlope>>8&0x00ff;
		loffset = wOffset&0x00ff;
		moffset = wOffset>>8&0x00ff;
		lADC = wADC&0x00ff;
		mADC = wADC>>8&0x00ff;  
		
		memset(strInpt, 0, 256);
		memset(strOupt, 0, 256); 
		sprintf(strInpt, "MCU_SET_TABLE(%s,%d,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X)", XGSPON_OLT_10G_RSSI_LUT_NAME, XGSPON_OLT_10G_RSSI_LUT_INDEX, address, lADC, mADC, lslope, mslope, loffset, moffset);
		SetCommand(handle, strInpt, strOupt);    
		if(0 != strcmp(strOupt, "OK"))
		{
			MessagePopup ("ERROR", "输入命令出错!     "); 
			return -1;
		} 
	}
	
	if(XGSPON_OLT_Update_BASE(handle, XGSPON_OLT_10G_RSSI_LUT_INDEX)<0)
	{
		return -1;
	}
	
	error=XGSPON_OLT_SET_10G_RXP_MON(handle, LUT_MODE_TABLE);  
	if(error!=0)
	{
		return -1;
	}
	
	if(XGSPON_OLT_Update_BASE(handle, 0)<0)
	{
		return -1;
	}
	
	return 0;	
}
/***************************************XGPON RSSI************************************/

/***************************************XGPON TXMON************************************/
int XGSPON_OLT_SET_10G_TxMon(int handle, float MON_L, float MON_H, float Unit)  						//设置XGPON TxMon 门限    
{
	int error=0;
	
	INT16U TX_MON_L;
	INT16U TX_MON_H;   
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	if(10000* pow(10, MON_L/10.)/(Unit/0.1)>65535) 
    { 
		TX_MON_L = (INT16U)(65535);
	}
	else
	{
    	TX_MON_L = (INT16U)(10000* pow(10, MON_L/10.)/(Unit/0.1));   //dBm转换成0.1uw
	}
	
	if(10000* pow(10, MON_H/10.)/(Unit/0.1)>65535) 
    { 
		TX_MON_H = (INT16U)(65535);
	}
	else
	{
    	TX_MON_H = (INT16U)(10000* pow(10, MON_H/10.)/(Unit/0.1));   //dBm转换成0.1uw
	}
	
	sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x, 0x%x, 0x%x, 0x%x)", XGSPON_OLT_10G_TXMON_MIN_ADD, (BYTE)(((INT16S)(TX_MON_L)) & 0xFF), (BYTE)(((INT16S)(TX_MON_L)>> 8) & 0xFF), (BYTE)(((INT16S)(TX_MON_H)) & 0xFF), (BYTE)(((INT16S)(TX_MON_H)>> 8) & 0xFF));
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置自动模式
	if(error!=0)
	{	
		return -1;	
	}
	
	return 0;																				  
}

int XGSPON_OLT_GET_10G_TxPOWER_ADC(int handle, unsigned short *ADC)   									//读取XGPON TX ADC 
{
	int          error = 0;
    unsigned int temp16U[2];
    char         mode;
	unsigned int liv_adctemp; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));

	error = SetCommand (handle, "MCU_GET_MON_INFO(txpower)", strOupt);
	if (error!=0) 
	{
		return -1; 
	}
	
	sscanf(strOupt, "%c,0x%x,0x%x", &mode, &temp16U[0], &temp16U[1]);
	sprintf(strInpt, "MCU_SET_MON_INFO(txpower, E, 0x%x, 0x%x)", temp16U[0], temp16U[1]);
	error = SetCommand (handle, strInpt, strOupt);	
	if (error!=0) 
	{
		return -1; 
	}
	
	error = SetCommand (handle, "MCU_GET_ADC(7)", strOupt);
	if (error!=0) 
	{
		return -1; 
	}
	sscanf(strOupt, "0x%x", &liv_adctemp); 
	*ADC=liv_adctemp;
	if(*ADC>3000)
	{
		Delay(0.5);
		error = SetCommand (handle, "MCU_GET_ADC(7)", strOupt);
		if (error!=0) 
		{
			return -1; 
		}
		sscanf(strOupt, "0x%x", &liv_adctemp); 
		*ADC=liv_adctemp; 
	}
	
	sprintf(strInpt, "MCU_SET_MON_INFO(txpower, F, 0x%x, 0x%x)", temp16U[0], temp16U[1]);
	error = SetCommand (handle, strInpt, strOupt);	
	if (error!=0) 
	{
		return -1; 
	}
	
	return 0;
}

int XGSPON_OLT_SET_10G_TXPOW_CAL(int handle, unsigned short *ADC_Array, float *PWR_Array, float Unit) 	//写XGPON Tx校准数据
{
	float slope;
	float offset;
	int   error;
	
	INT16S   nSlope;
	INT16S   nOffset;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	 
	PWR_Array[0] = pow(10, PWR_Array[0] /10.) / (Unit*1E-3); //1LSB=0.1uW 
	PWR_Array[1] = pow(10, PWR_Array[1] /10.) / (Unit*1E-3); //1LSB=0.1uW 
	 
	slope = (PWR_Array[1] - PWR_Array[0]) / (ADC_Array[1] - ADC_Array[0]);
	nSlope = (INT16S)(slope*XGSPON_OLT_10G_TXMON_SLOPE_AMP);	   				//slope四舍五入取整**Eric.Yao**20130730

	offset= PWR_Array[0] - slope * ADC_Array[0];   	//slope四舍五入取整后计算offset**Eric.Yao**20130730
	nOffset = (INT16S)(offset);	   										//offset四舍五入取整**Eric.Yao**20130730
	
	sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%d,0x%x, 0x%x, 0x%x, 0x%x)", XGSPON_OLT_10G_TXMON_SLOPE_ADD, (BYTE)(((INT16S)(nSlope)) & 0xFF), (BYTE)(((INT16S)(nSlope)>> 8) & 0xFF), (BYTE)(((INT16S)(nOffset)) & 0xFF), (BYTE)(((INT16S)(nOffset)>> 8) & 0xFF));
	error = SetCommand (handle, strInpt, strOupt);	      
	if (error!=0) 
	{
		return -1; 
	}

	return 0;  
}
/***************************************XGPON TXMON************************************/

int XGSPON_OLT_GET_10G_Bias(int handle, float *Bias, float Unit)		//计算XGPON Bias
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*Bias = calculate_biascurrent_8077(local_XFP_A0_Lower.sStr.tx_Bias, 1, 0, Unit);
	
	return 0;
		
}

int XGSPON_OLT_GET_10G_TxMon(int handle, float *TxMon, float Unit)		//计算XGPON TxMon
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*TxMon = calculate_txpower_8077(local_XFP_A0_Lower.sStr.tx_Power, 1, 0, Unit);
	
	return 0;
		
}

int XGSPON_OLT_GET_10G_RxMon(int handle, float *RxMon, float Unit)		//计算XGPON RxMon
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*RxMon = calculate_rxpower_8077(local_XFP_A0_Lower.sStr.rx_Power, 1, 0, Unit);
	
	return 0;
		
}
/**************************************************XGPON OLT**************************************************/

/***************************************TEMP************************************/
int XGSPON_OLT_SET_TOSA_TEMP_Mode (int handle, enum LUT_MODE Mode)	//设置TEMP模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,60,0x%x);", 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,60,0x%x);", 'A');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int XGSPON_OLT_GET_TOSA_TEMP(int handle, unsigned short* GET_TEMP)	//读取TEMP值 
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  TEMP_LSB=0;
	unsigned char  TEMP_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	error = SetCommand (handle, "MCU_GET_TABLE(BASE,0,62,2)", strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x", &TEMP_LSB, &TEMP_MSB);                   
	
	*GET_TEMP=(((unsigned short)TEMP_MSB)<<8)+TEMP_LSB;
				
	return 0;	
}

int XGSPON_OLT_SET_TOSA_TEMP(int handle, unsigned short SET_TEMP)	//设置TEMP值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,62,0x%x,0x%x)", (unsigned char)(SET_TEMP&0xFF), (unsigned char)((SET_TEMP>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_SET_TOSA_TEMP_Range(int handle, unsigned short TEMP_MAX, unsigned short TEMP_MIN)	//设置TEMP最大最小值	  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,64,0x%x,0x%x,0x%x,0x%x)", (unsigned char)(TEMP_MAX&0xFF), (unsigned char)((TEMP_MAX>>8)&0xFF), (unsigned char)(TEMP_MIN&0xFF), (unsigned char)((TEMP_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}
/***************************************TEMP************************************/

/***************************************EA************************************/
int XGSPON_OLT_SET_EA_Mode (int handle, enum LUT_MODE Mode)	//设置EA模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,68,0x%x)", 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,68,0x%x)", 'A');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int XGSPON_OLT_GET_EA(int handle, unsigned short* GET_EA)	//读取EA值 
{
	int            error=0;
	int            liv_EA=0; 
	unsigned char  EA_LSB=0;
	unsigned char  EA_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	error = SetCommand (handle, "MCU_GET_TABLE(BASE,0,70,2)", strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x",&EA_LSB,&EA_MSB);                   
	
	*GET_EA=(((unsigned short)EA_MSB)<<8)+EA_LSB;
				
	return 0;	
}

int XGSPON_OLT_SET_EA(int handle, unsigned short SET_EA)	//设置EA值  
{
	int     error=0;
	int	EA_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,70,0x%x,0x%x)",(unsigned char)(SET_EA&0xFF),(unsigned char)((SET_EA>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_SET_EA_Range(int handle, unsigned short EA_MAX, unsigned short EA_MIN)	//设置EA最大最小值	  
{
	int     error=0;
	int	EA_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,72,0x%x,0x%x,0x%x,0x%x)", (unsigned char)(EA_MAX&0xFF), (unsigned char)((EA_MAX>>8)&0xFF), (unsigned char)(EA_MIN&0xFF), (unsigned char)((EA_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}
/***************************************EA************************************/

/***************************************TEC************************************/
int XGSPON_OLT_SET_TEC_Mode (int handle, enum LUT_MODE Mode)	//设置TEC模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,52,0x%x)", 'M');
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,52,0x%x)", 'A');
			break;
		}
		
		case LUT_MODE_TABLE:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,48,0x%x)", 'T');
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int XGSPON_OLT_GET_TEC(int handle, unsigned short* GET_TEC)	//读取TEC值 
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  TEC_LSB=0;
	unsigned char  TEC_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	error = SetCommand (handle, "MCU_GET_TABLE(BASE,0,54,2)", strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x", &TEC_LSB, &TEC_MSB);                   
	
	*GET_TEC=(((unsigned short)TEC_MSB)<<8)+TEC_LSB;
				
	return 0;	
}

int XGSPON_OLT_SET_TEC(int handle, unsigned short SET_TEC)	//设置TEC值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,54,0x%x,0x%x)", (unsigned char)(SET_TEC&0xFF), (unsigned char)((SET_TEC>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_SET_TEC_Range(int handle, unsigned short TEC_MAX, unsigned short TEC_MIN)	//设置TEC最大最小值	  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,56,0x%x,0x%x,0x%x,0x%x)", (unsigned char)(TEC_MAX&0xFF), (unsigned char)((TEC_MAX>>8)&0xFF), (unsigned char)(TEC_MIN&0xFF), (unsigned char)((TEC_MIN>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_GET_TEC_LUT(int handle, int *Temper, float *Slope, float *Offset)	//读取TEC LUK       
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=0;index<2;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, index*XGSPON_OLT_LUT_BYTE_COUNT, XGSPON_OLT_LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-XGSPON_OLT_CORE_TEMP_LUT_OFFSET;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/XGSPON_OLT_TEC_SLOPE_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x); 
	}
	return 0;
}

int XGSPON_OLT_SET_TEC_LUT(int handle, float* DAC, float* Temper)	//写TEC查找表           
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_TEC_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_TEC_SLOPE_AMP);
			
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_TEC_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_TEC_SLOPE_AMP);			
			
	offset_m = (120.+ XGSPON_OLT_CORE_TEMP_LUT_OFFSET)* sSlop[1]/XGSPON_OLT_TEC_SLOPE_AMP +  sOffset[1];   
	
	//写查找表
	//第一段
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_TEC_LUT_INDEX)<0)
	{
		return -1;
	}

	return 0;
}

int XGSPON_OLT_SET_TEC_LUT_High(int handle, float* DAC, float* Temper)	//写TEC查找表High      
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * XGSPON_OLT_TEC_SLOPE_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	=	(DAC[1] - sSlop[1] * (Temper[1] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_TEC_SLOPE_AMP);			

	offset_m = (120+50)* sSlop[1]/XGSPON_OLT_TEC_SLOPE_AMP +  sOffset[1]; 
	
	
	//写高温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_TEC_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
}

int XGSPON_OLT_SET_TEC_LUT_Low(int handle, float* DAC, float* Temper)	//写TEC查找表LOW             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * XGSPON_OLT_TEC_SLOPE_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	=	(DAC[0] - sSlop[0] * (Temper[0] + XGSPON_OLT_CORE_TEMP_LUT_OFFSET)/XGSPON_OLT_TEC_SLOPE_AMP);
			
	//写低温段查找表
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", XGSPON_OLT_TEC_LUT_NAME, XGSPON_OLT_TEC_LUT_INDEX, XGSPON_OLT_LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+XGSPON_OLT_CORE_TEMP_LUT_OFFSET)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(XGSPON_OLT_Update_LUK(handle, XGSPON_OLT_TEC_LUT_INDEX)<0)
	{
		return -1;	
	}

	return 0;
} 
/***************************************TEC************************************/

int XGSPON_OLT_SET_BIAS_Unit_Ex(int handle, int unit)                //根据数据配置Bias单位来配置Bias校准系数      
{
	char strOupt[256];
	char strInpt[256];
	
    unsigned int config=0; 																								
	int          error=0;
	unsigned int Slop,Offset,Temper; 
	unsigned int dTemper_L,dTemper_H,dSlop_L,dSlop_H,dOffset_L,dOffset_H;     

	if(4==unit)
	{
		sprintf(strInpt, "MCU_SET_MON_INFO(txbias,F,0x003C,0xCE32)");								//10G Bias 4uA  			
		error=SetCommand (handle,strInpt, strOupt);
		if(error!=0) 
		{	 
			return -1; 
		}
	}
	else
	{
		sprintf(strInpt, "MCU_SET_MON_INFO(txbias,F,0x78,0x9C64)");								//10G Bias 2uA  			
		error=SetCommand (handle,strInpt, strOupt);
		if(error!=0) 
		{	 
			return -1; 
		}
	}

	sprintf(strInpt, "MCU_UPDATE_FLASH(BASE,0)");								//10G Bias 2uA  			
	error=SetCommand (handle,strInpt, strOupt);
	if(error!=0) 
	{	 
		return -1; 
	}
		
	return 0;
}

/***************************************CASE TEMP************************************/ 
int XGSPON_OLT_SET_CASE_TEMP_Mode (int handle, enum LUT_MODE Mode)	//设置CASE TEMP模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_FORMULA:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,00,0x%x)", 2);
			break;
		}
		
		case LUT_MODE_TABLE:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,00,0x%x)", 1);
			break;
		}
		
		case LUT_MODE_EXT:
		{
			sprintf (strInpt, "MCU_SET_TABLE(BASE,0,00,0x%x)", 0);
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int XGSPON_OLT_SET_CASE_TEMP_LUT (int handle, float K1, float B1, float K2, float B2, float S)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt, "MCU_SET_TABLE(luk,3,0,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)",    \
																 (BYTE)((int)(S+50) & 0xFF),00,\
																 (BYTE)(((INT16S)(K1*256)) & 0xFF), (BYTE)(((INT16S)(K1*256)>> 8) & 0xFF), \
																 (BYTE)(((INT16S)(B1)) & 0xFF), (BYTE)(((INT16S)(B1) >> 8) & 0xFF));
	SetCommand (handle, strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))     
	{
		MessagePopup ("ERROR", "写温度LUT错误!     "); 
		return -1;
	} 
	Delay(1.);	
	sprintf(strInpt, "MCU_SET_TABLE(luk,3,6,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)",    \
																 240,00,\
																 (BYTE)(((INT16S)(K2*256)) & 0xFF), (BYTE)(((INT16S)(K2*256)>> 8) & 0xFF), \
																 (BYTE)(((INT16S)(B2)) & 0xFF), (BYTE)(((INT16S)(B2) >> 8) & 0xFF));
	SetCommand (handle, strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))    
	{
		MessagePopup ("ERROR", "写温度LUT错误!     "); 
		return -1;
	} 
	
	return 0;
}

int XGSPON_OLT_GET_CASE_Temper(int handle, float *Temper, float Unit)		//计算XGSPON OLT Case Temper
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*Temper = calculate_temperature_8077(local_XFP_A0_Lower.sStr.temperature, 1, 0, Unit);
	   
	return 0;  
}
/***************************************CASE TEMP************************************/

/***************************************CORE TEMP************************************/
int XGSPON_OLT_GET_CORE_Temper(int handle, float* CoreTemper) 	   //模块内核温度监控
{
	int error = 0;
 	int temp16U[2]; 
	char mode;
	int ltemp=0;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
			
	error = SetCommand (handle, "MCU_GET_ADC(1)", strOupt);
	if (error!=0) 
	{
		return -1; 
	}
	sscanf(strOupt, "0x%x", &ltemp);
	*CoreTemper=(double)((short)ltemp)-50; 
			
	return 0;
}
/***************************************CORE TEMP************************************/

/***************************************VCC************************************/
int XGSPON_OLT_Cal_Vcc(int handle, float slope, float offset)          // Cal Vcc
{
	int error=0;   
	
	char strOupt[256];
	char strInpt[256];
	
	INT16U wSlope;
	INT16U wOffset;  
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	if (slope*128.>65535.)
	{
		wSlope = 0xFFFF;
	}
	else
	{
		wSlope = (INT16U)(slope*128.);   
	}
	
	if (offset>65535.)
	{
		wOffset = 0xFFFF;
	}
	else
	{
		wOffset = (INT16U)(offset);   
	}
	
	sprintf (strInpt, "MCU_SET_MON_INFO(voltage,f,0x%x,0x%x)", wSlope, wOffset);
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	return 0;
}
/***************************************VCC************************************/

/***************************************CFG************************************/
int XGSPON_OLT_SET_CFG(int handle, unsigned short DAC)                                                      
{
    unsigned int config=0; 
	int          error=0;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt, "MCU_SET_CFG(0x%x)", DAC); 
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0) 
	{	 
		return -1; 
	}
	
	return 0;
}
/***************************************CFG************************************/

/***************************************DRIVER************************************/
int XGSPON_OLT_Update_DRIVER(int handle)   			      //更新2010驱动寄存器表  
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	
	
	SetCommand (handle, "GN2010_SAVE_REG()", strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	}
	
	SetCommand (handle, "MCU_UPDATE_FLASH(DRIVER, 0)", strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	}
	return 0; 		
}
/***************************************DRIVER************************************/



/***************************************APC 驱动模式************************************/
int XGSPON_OLT_SET_APC_DriverMode(int handle, int DAC)                                                      
{
    unsigned int config=0; 
	int          error=0;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt, "gn2010_write(82,0x%x)", DAC); 
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0) 
	{	 
		return -1; 
	}
	
	return 0;
}
/***************************************CFG************************************/

int XGSPON_OLT_Fault_OFF(int handle)	//设置APC Fault功能关闭  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"gn2010_write(113,0x5F)");    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int XGSPON_OLT_Fault_ON(int handle)	//设置APC Fault功能开启  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"gn2010_write(113,0x1F)");    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

float calculate_temperature_8077(INT16S data, float slope, float offset, float unit)
{
	float result;
	
	result = (slope * data + offset) * (1/256.) * unit;
	
	return result;
}

float calculate_voltage_8077(INT16U data, float slope, float offset, float unit)
{
	float result; 

	result = (slope * data + offset) * (1.E-6) * unit; 
	
	return result;
}

float calculate_biascurrent_8077(INT16U data, float slope, float offset, float unit)
{
	float result;

	result = (slope * data + offset) * (1.E-3) * unit;    
	
	return result;
}

float calculate_txpower_8077(INT16U data, float slope, float offset, float unit)
{
	float result;

	DisableBreakOnLibraryErrors ();										 
	result = 10.*log10((slope * data + offset) * (1.E-3) * unit);
	EnableBreakOnLibraryErrors ();
	
	return result;
}

float calculate_rxpower_8077(INT16U data, float slope, float offset, float unit)
{
	float result;

	DisableBreakOnLibraryErrors ();										 
	result = 10.*log10((slope * data + offset) * (1.E-3) * unit);
	EnableBreakOnLibraryErrors ();

	return result;
}  

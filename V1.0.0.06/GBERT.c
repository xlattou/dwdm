#include <formatio.h>
#include <utility.h>
#include <visa.h>
#include <userint.h>
#include <ansi_c.h>
#include <toolbox.h> 
#include "GBERT_DLL.h"  
#include "GBERT.h"
#include "GBERT10000_dll.h"

BOOL GBERT_Init(int *GBERT_USBHandle, unsigned char *GBERT_SerialNumber, int Pattern, int Rate, int myBERT_SFPTxDisable)
{
	int GBERT_ErrorCode;
	enum emum_BERT_Pattern BERT_Pattern;
	enum emum_BERT_Application BERT_Application;
	enum emum_BERT_CheckerMode BERT_CheckerMode; 
	double BERT_GatingTime;

	int BERT_SFPout_en;
	int BERT_SFPin_en;
	int BERT_SMAout_en;

	int BERT_InputPolarity; 
	int BERT_OutputPolarity; 
	int BERT_SFPTxDisable;
	int BERT_InputEqualization; 
	int BERT_OutputSwing;  
	int BERT_OutputSlewRate;	
	
	//step1, GBERT selftest
	GBERT_ErrorCode = GBERT_Selftest_withSN_DLL (GBERT_SerialNumber, GBERT_USBHandle);
	if (GBERT_ErrorCode<0) return FALSE;
	
	//step2, GBERT initialize
//	BERT_Pattern           = PRBS7;
//	BERT_Application       = R1250Mbps;
	BERT_Pattern           = Pattern;
	BERT_Application       = Rate;
	BERT_CheckerMode       = SingleRun; 
	BERT_SFPin_en          = 0; //SFP input disable, so SMA input enable
	BERT_SFPout_en         = 1; //output enable
	BERT_SMAout_en         = 1; //output enable 
	BERT_SFPTxDisable      = myBERT_SFPTxDisable; //output enable 
	BERT_InputPolarity     = 0; //not invert
	BERT_OutputPolarity    = 0; //not invert
	BERT_InputEqualization = 3; //0 dB
	BERT_OutputSwing       = 0; //small 
	BERT_OutputSlewRate    = 0; //fast
	

	GBERT_ErrorCode = GBERT_Initialize_DLL(*GBERT_USBHandle,
				 BERT_Pattern,  BERT_Application, BERT_CheckerMode,
				 BERT_SFPin_en,  BERT_SFPout_en, BERT_SMAout_en,  BERT_SFPTxDisable,
				 BERT_OutputPolarity,  BERT_InputPolarity,
				 BERT_InputEqualization,  BERT_OutputSwing, BERT_OutputSlewRate);
	if (GBERT_ErrorCode<0) return FALSE; 
	
	//step3, GBERT single run
	//BERT start check
	BERT_GatingTime = 1.0;//1s
	GBERT_ErrorCode = GBERT_StartCheck_DLL(*GBERT_USBHandle, BERT_GatingTime);
	if (GBERT_ErrorCode<0) return FALSE; 
	
	return TRUE;
}

BOOL GBERT_Init_SFPTxDisable(int *GBERT_USBHandle, unsigned char *GBERT_SerialNumber, int myBERT_SFPTxDisable)
{
	//step1, GBERT selftest
	GBERT_ErrorCode = GBERT_Selftest_withSN_DLL (GBERT_SerialNumber, GBERT_USBHandle);
	if (GBERT_ErrorCode<0) return FALSE;
	
	//step2, GBERT initialize
	BERT_Pattern           = PRBS7;
	BERT_Application       = R2488Mbps;
	BERT_CheckerMode       = SingleRun; 
	BERT_SFPin_en          = 0; //SFP input disable, so SMA input enable
	BERT_SFPout_en         = 1; //output enable
	BERT_SMAout_en         = 1; //output enable 
	BERT_SFPTxDisable      = myBERT_SFPTxDisable; //output enable 
	BERT_InputPolarity     = 0; //not invert
	BERT_OutputPolarity    = 0; //not invert
	BERT_InputEqualization = 3; //0 dB
	BERT_OutputSwing       = 0; //small 
	BERT_OutputSlewRate    = 0; //fast
	
	GBERT_ErrorCode = GBERT_Initialize_DLL(*GBERT_USBHandle,
				 BERT_Pattern,  BERT_Application, BERT_CheckerMode,
				 BERT_SFPin_en,  BERT_SFPout_en, BERT_SMAout_en,  BERT_SFPTxDisable,
				 BERT_OutputPolarity,  BERT_InputPolarity,
				 BERT_InputEqualization,  BERT_OutputSwing, BERT_OutputSlewRate);
	if (GBERT_ErrorCode<0) return FALSE; 
	
	//step3, GBERT single run
	//BERT start check
	BERT_GatingTime = 1.0;//1s
	GBERT_ErrorCode = GBERT_StartCheck_DLL(*GBERT_USBHandle, BERT_GatingTime);
	if (GBERT_ErrorCode<0) return FALSE; 
	
	return TRUE;
}

BOOL GBERT_Start(int GBERT_USBHandle, double testTime)
{		
	double BERT_GatingTime;
	int GBERT_ErrorCode;     
	
	BERT_GatingTime=testTime;
	GBERT_ErrorCode = GBERT_StartCheck_DLL(GBERT_USBHandle, BERT_GatingTime);
	if (GBERT_ErrorCode<0) return FALSE;
	
	return TRUE;
}

int GBERT_READ_Status (int inst_GBERT, unsigned int *ErrorBit)
{
	int GBERT_ErrorCode;   
	double BERT_Accumulated_ErrorRatio;
	double BERT_Accumulated_BitCount;
	double BERT_Accumulated_ErrorCount;
	double BERT_Accumulated_ElapsedTime;
	
	GBERT_ErrorCode = GBERT_GetCheckStatus_DLL(inst_GBERT, &BERT_Accumulated_ErrorRatio, 
							&BERT_Accumulated_BitCount, &BERT_Accumulated_ErrorCount, &BERT_Accumulated_ElapsedTime);  
	
	if (GBERT_ErrorCode<0) {*ErrorBit=99;}
	else   				   {*ErrorBit=(unsigned int)BERT_Accumulated_ErrorCount;}

	return 0;
}

BOOL GBERT_Check_Start(int GBERT_USBHandle, unsigned int *ErrorBit)
{
int count, errnum;

	int GBERT_ErrorCode;  
	double BERT_Accumulated_ErrorRatio;
	double BERT_Accumulated_BitCount;
	double BERT_Accumulated_ErrorCount;
	double BERT_Accumulated_ElapsedTime;
	double BERT_GatingTime = 1.0;  

	count=0;*ErrorBit=1;
	do
	{
		GBERT_ErrorCode = GBERT_GetCheckStatus_DLL(GBERT_USBHandle, &BERT_Accumulated_ErrorRatio, 
							&BERT_Accumulated_BitCount, &BERT_Accumulated_ErrorCount, &BERT_Accumulated_ElapsedTime);  
		
		if (GBERT_ErrorCode<0)  Delay(0.5);	//GBERT_ErrorCode<0 ״̬��ȡ�д��� ��ʱ1s��ȴ���һ��ѭ��
		
		count++;
		
		if (count>3) break;			//��ֹ��ѭ��,ǿ��5��ѭ�����˳�
	} while (GBERT_ErrorCode<0);  	
	
	*ErrorBit=(unsigned int)BERT_Accumulated_ErrorCount;
	
	//����״̬�����⣬���³�ʼ��BERT
	if (GBERT_ErrorCode<0) 
	{
		errnum=0;
		do
		{
			if(!GBERT_RESET(GBERT_USBHandle)) {MessagePopup("Error", "Reset GBERT error!");return FALSE;}
			Delay(1);
			if(!GBERT_Start(GBERT_USBHandle, BERT_GatingTime)) {MessagePopup("Error", "Reset GBERT error!");return FALSE;}
			Delay(1.5); 

			count=0;*ErrorBit=1;
			do
			{
				GBERT_ErrorCode = GBERT_GetCheckStatus_DLL(GBERT_USBHandle, &BERT_Accumulated_ErrorRatio, 
									&BERT_Accumulated_BitCount, &BERT_Accumulated_ErrorCount, &BERT_Accumulated_ElapsedTime);  
		
				if (GBERT_ErrorCode==1)  Delay(1);	//GBERT_ErrorCode==1 һ����������û����� ��ʱ1s��ȴ���һ��ѭ��
		
				count++;
		
				if (count>5) break;			//��ֹ��ѭ��,ǿ��5��ѭ�����˳�
			} while (GBERT_ErrorCode==1);  	//GBERT_ErrorCode==1 һ����������û�����
			
			errnum++;
			
		}while (GBERT_ErrorCode<0 && errnum<3);

		if (GBERT_ErrorCode<0) {*ErrorBit=99;return FALSE;}
		else   				   {*ErrorBit=(unsigned int)BERT_Accumulated_ErrorCount;}
	}

	return TRUE;
}

BOOL GBERT_Check(int GBERT_USBHandle, unsigned int *ErrorBit)
{
int count, errnum;

	int GBERT_ErrorCode;  
	double BERT_Accumulated_ErrorRatio;
	double BERT_Accumulated_BitCount;
	double BERT_Accumulated_ErrorCount;
	double BERT_Accumulated_ElapsedTime;
	double BERT_GatingTime = 1.0; 

	count=0;*ErrorBit=1;
	do
	{
		GBERT_ErrorCode = GBERT_GetCheckStatus_DLL(GBERT_USBHandle, &BERT_Accumulated_ErrorRatio, 
							&BERT_Accumulated_BitCount, &BERT_Accumulated_ErrorCount, &BERT_Accumulated_ElapsedTime);  
		
		if (GBERT_ErrorCode==1)  Delay(0.5);	//GBERT_ErrorCode==1 һ����������û����� ��ʱ1s��ȴ���һ��ѭ��
		
		count++;
		
		if (count>3) break;			//��ֹ��ѭ��,ǿ��5��ѭ�����˳�
	} while (GBERT_ErrorCode==1);  	//GBERT_ErrorCode==1 һ����������û�����
	
	*ErrorBit=(unsigned int)BERT_Accumulated_ErrorCount;

	//����״̬�����⣬���³�ʼ��BERT
	if (GBERT_ErrorCode<0) 
	{
		errnum=0;
		do
		{
			if(!GBERT_RESET(GBERT_USBHandle)) {MessagePopup("Error", "Reset GBERT error!");return FALSE;}
			Delay(1);
			if(!GBERT_Start(GBERT_USBHandle, BERT_GatingTime)) {MessagePopup("Error", "Reset GBERT error!");return FALSE;}
			Delay(1.5); 

			count=0;*ErrorBit=1;
			do
			{
				GBERT_ErrorCode = GBERT_GetCheckStatus_DLL(GBERT_USBHandle, &BERT_Accumulated_ErrorRatio, 
									&BERT_Accumulated_BitCount, &BERT_Accumulated_ErrorCount, &BERT_Accumulated_ElapsedTime);  
		
				if (GBERT_ErrorCode==1)  Delay(1);	//GBERT_ErrorCode==1 һ����������û����� ��ʱ1s��ȴ���һ��ѭ��
		
				count++;
		
				if (count>5) break;			//��ֹ��ѭ��,ǿ��5��ѭ�����˳�
			} while (GBERT_ErrorCode==1);  	//GBERT_ErrorCode==1 һ����������û�����
			
			errnum++;
			
		}while (GBERT_ErrorCode<0 && errnum<3);

		if (GBERT_ErrorCode<0) {*ErrorBit=99;return FALSE;}
		else   				   {*ErrorBit=(unsigned int)BERT_Accumulated_ErrorCount;}
	}
	
	return TRUE;
}

BOOL GBERT_Stop(int GBERT_USBHandle)
{
	int GBERT_ErrorCode;    
	
	GBERT_ErrorCode = GBERT_StopCheck_DLL(GBERT_USBHandle);
	if (GBERT_ErrorCode<0) return FALSE;
	
	return TRUE;
}

BOOL GBERT_RESET(int GBERT_USBHandle)
{
	int GBERT_ErrorCode;    
	enum emum_BERT_Pattern BERT_Pattern = PRBS7;
	enum emum_BERT_Application BERT_Application=R2488Mbps;
	enum emum_BERT_CheckerMode BERT_CheckerMode = SingleRun;  
	int BERT_SFPin_en=0;
	int BERT_SFPout_en=1;
	int BERT_SMAout_en=1;
	int BERT_SFPTxDisable=0;
	int BERT_OutputPolarity=0;
	int BERT_InputPolarity=0;
	int BERT_InputEqualization=3; 
	int BERT_OutputSwing=0;
	int BERT_OutputSlewRate=0;      
	
	GBERT_ErrorCode = GBERT_Initialize_DLL(GBERT_USBHandle,
				 BERT_Pattern,  BERT_Application, BERT_CheckerMode,
				 BERT_SFPin_en,  BERT_SFPout_en, BERT_SMAout_en,  BERT_SFPTxDisable,
				 BERT_OutputPolarity,  BERT_InputPolarity,
				 BERT_InputEqualization,  BERT_OutputSwing, BERT_OutputSlewRate);
	if (GBERT_ErrorCode<0) return FALSE; 
	
	Delay(0.5);
	
	return TRUE;
}

BOOL GBERT_Check_Ratio(int GBERT_USBHandle, double *ErrorRatio, int *errorcode)
{
	int errnum;
	
	int GBERT_ErrorCode;
	double BERT_Accumulated_ErrorRatio;
	double BERT_Accumulated_BitCount;
	double BERT_Accumulated_ErrorCount;
	double BERT_Accumulated_ElapsedTime;

	GBERT_ErrorCode = GBERT_GetCheckStatus_DLL(GBERT_USBHandle, &BERT_Accumulated_ErrorRatio, 
					&BERT_Accumulated_BitCount, &BERT_Accumulated_ErrorCount, &BERT_Accumulated_ElapsedTime);  
		 
//	*ErrorBit=(unsigned int)BERT_Accumulated_ErrorCount;
	*ErrorRatio = BERT_Accumulated_ErrorRatio;
	*errorcode = GBERT_ErrorCode;
	
	return TRUE;     
}

int GBERT_GET_SN(int panel, int control)
{
	int error=0, num=0;
	
	int GMeter_USBHandle;
	char GMeter_SerialNumber[17];               
	
	ClearListCtrl (panel, control);
	num=0;
	for (GMeter_USBHandle=0; GMeter_USBHandle<16; GMeter_USBHandle++)
	{
		error = GMeter_GetSN_DLL(GMeter_USBHandle, GMeter_SerialNumber);
		if ((!error)&&(0 == strncmp(GMeter_SerialNumber, "GB", 2)))
		{   
			num++;
			InsertListItem (panel, control, -1, GMeter_SerialNumber, num);
		}
	}
	
	for (GMeter_USBHandle=0; GMeter_USBHandle<16; GMeter_USBHandle++)
	{	
		error = GBERT10000_GetSN_DLL(GMeter_USBHandle, GMeter_SerialNumber);
		if ((!error)&&(0 == strncmp(GMeter_SerialNumber, "GB", 2)))
		{   
			num++;
			InsertListItem (panel, control, -1, GMeter_SerialNumber, num);
		}
	}
	return 0;
}

int	GVPM_Init (int *GVPM_USBHandle, unsigned char *GOAM_SerialNumber, int Wavelength, float AttValue)
{
	int error;
	char info[500];
	
	int GOAM_WaveLength1, GOAM_WaveLength2, GOAM_WaveLength3, GOAM_WaveLength4;
	double GOAM_MaxAttenuateValue1, GOAM_MaxAttenuateValue2, GOAM_MaxAttenuateValue3, GOAM_MaxAttenuateValue4;
	int GOAM_OutputEnable;
	int GOAM_WaveLength;
	double GOAM_Attenuate;
	double GOAM_Temperature;
	int GOAM_ErrorCode;  
	
	error = GOAM_Selftest_DLL (GOAM_SerialNumber, GVPM_USBHandle);
	if (error<0) 
	{
		sprintf (info, "GVPM selftest fail, error code:%d", error);
		MessagePopup ("Error", info);
		return -1; 
	}
	
	error = GOAM_Initialize_DLL (*GVPM_USBHandle, &GOAM_Temperature, &GOAM_OutputEnable, &GOAM_WaveLength, &GOAM_Attenuate,
								&GOAM_WaveLength1, &GOAM_MaxAttenuateValue1, &GOAM_WaveLength2, &GOAM_MaxAttenuateValue2, 
								&GOAM_WaveLength3, &GOAM_MaxAttenuateValue3, &GOAM_WaveLength4, &GOAM_MaxAttenuateValue4);
	if (error<0) 
	{
		sprintf (info, "GVPM initial fail, error code:%d", error);
		MessagePopup ("Error", info);
		return -1; 
	}
	
	if (Wavelength==1490) Wavelength=1480; 
	GOAM_WaveLength=Wavelength;
	
	GVPM_SET_OutputEnable (*GVPM_USBHandle, 1);
		
	GOAM_Attenuate =AttValue; 
	error = GOAM_Attenuate_DLL(*GVPM_USBHandle, GOAM_WaveLength, GOAM_Attenuate);
	if(error==-5)	//��һ�����ó���-5�Ĵ��󣬱�����˥����֧��lock ģʽ��������ҵ�ǰ����lockģʽ�����������õ�freeģʽ
	{
		GOAM_ErrorCode = GVPM_SET_LockEnable (*GVPM_USBHandle, 0);
		if (GOAM_ErrorCode) return -1;
		
		//����Ϊfreeģʽ����������һ������˥����
		error = GOAM_Attenuate_DLL(*GVPM_USBHandle, GOAM_WaveLength, GOAM_Attenuate);
	}
	if (error<0) 
	{
		sprintf (info, "GVPM set attenuate fail, error code:%d", error);
		MessagePopup ("Error", info);
		return -1; 
	}	
	
	return 0;
}

int	GVPM_SET_Attenuate (int GVPM_USBHandle, float AttValue, int Wavelength)
{
	int error;
	char info[500]; 
	
	int GOAM_WaveLength;
	double GOAM_Attenuate;
	
	if (Wavelength==1490) Wavelength=1480; 
	GOAM_WaveLength=Wavelength;
	
	GOAM_Attenuate =AttValue; 
	error = GOAM_Attenuate_DLL(GVPM_USBHandle, GOAM_WaveLength, GOAM_Attenuate);
	if (error<0) 
	{
		sprintf (info, "GVPM set attenuate fail, error code:%d", error);
		MessagePopup ("Error", info);
		return -1; 
	}	
	
	error = GVPM_SET_OutputEnable (GVPM_USBHandle, 1);
	if (error<0) {MessagePopup ("Error", "GOAM set OutputEnable fail");return -1;}  
	
	return 0;
}

int	GVPM_SET_OutputEnable (int GVPM_USBHandle, int GOAM_OutputEnable)
{
	int error;
	char info[500];   
	
	error = GOAM_OutputEnable_DLL(GVPM_USBHandle, GOAM_OutputEnable);
	if (error<0) 
	{
		sprintf (info, "GVPM set OutputEnable fail, error code:%d", error);
		MessagePopup ("Error", info);
		return -1; 
	}	
	
	return 0;
}

int GVPM_SET_LockEnable (int inst_GVPM, int lockEnable)
{
	char info[500];
	
	int GOAM_ErrorCode;
	
	GOAM_ErrorCode = GOAM_LockEnable_DLL(inst_GVPM, lockEnable);
	if (GOAM_ErrorCode<0) 
	{
		sprintf (info, "GVPM set lock enable fail, error code:%d", GOAM_ErrorCode);
		MessagePopup ("Error", info);
		return -1; 
	}	
	
	return 0;
}

int GVPM_SET_LockAttenuate (int inst_GVPM, double LockAttValue, int Wavelength, int checklock)
{
	char info[500];
	int  count, error=0;
	
	int GOAM_WaveLength; 
	int GOAM_ErrorCode;    
	int GOAM_LockEnable;
	double GOAM_LockOPdBm;
	double GOAM_Attenuate;
	double GOPM_OPdBm; 
	double GOPM_OPmW;
	
	int index = 0;
	
	if (Wavelength==1490) 
		Wavelength=1480; 
	GOAM_WaveLength=Wavelength;
	//�������GOAM_LockAttenuateInit_DLLǰ��
	error = GVPM_SET_OutputEnable (inst_GVPM, 1);
	if (error<0) {MessagePopup ("Error", "GVPM set OutputEnable fail");return -1;}  
	
	GOAM_ErrorCode = GOAM_LockAttenuateInit_DLL (inst_GVPM, GOAM_WaveLength, LockAttValue);
	if (GOAM_ErrorCode<0) 
	{
		sprintf (info, "GVPM set lock attenuate fail, error code:%d", GOAM_ErrorCode);
		MessagePopup ("Error", info);
		return -1; 
	}
	
	if (checklock)
	{
		count=0;
		do
		{
			GOAM_ErrorCode = GOAM_GOPM_LockOutput_GetStatus_DLL (inst_GVPM, &GOAM_WaveLength, &GOAM_LockOPdBm, &GOAM_LockEnable, &GOAM_Attenuate, &GOPM_OPdBm, &GOPM_OPmW);
			if (GOAM_ErrorCode>0 || GOAM_ErrorCode==-5 || GOAM_ErrorCode==-6 || GOAM_ErrorCode==-7)
			{
				Delay (0.2);
				count++;
			}
		} while (count<15 && (GOAM_ErrorCode>0 || GOAM_ErrorCode==-5 || GOAM_ErrorCode==-6 || GOAM_ErrorCode==-7));
	
		if (GOAM_ErrorCode!=0) 
		{
			sprintf (info, "GVPM get lock attenuate status, error code:%d", GOAM_ErrorCode);
			MessagePopup ("Error", info);
			return -1; 
		}
	}
		
	return 0; 
}

int GVPM_GET_SN(int panel, int control)
{
	int error=0, num=0;
	
	int GMeter_USBHandle; 
	char GMeter_SerialNumber[17];         
	
	ClearListCtrl (panel, control);
	num=0;
	for (GMeter_USBHandle=0; GMeter_USBHandle<16; GMeter_USBHandle++)	   /***˥�����������16������쳣**Eric.Yao*20140102***/
	{	error = GMeter_GetSN_DLL(GMeter_USBHandle, GMeter_SerialNumber);
		if ((!error)&&(NULL != strstr(GMeter_SerialNumber, "VPM")))	   //***GVPM��VPM˥��������***wenyao.xi*20150815*/
		{   
			num++;
			InsertListItem (panel, control, -1, GMeter_SerialNumber, num);
		}
	}
	return 0;
}

int GBERT10000_Init (int *inst, char sn[], enum enum_GBERT_Pattern Pattern, enum enum_GBERT10000_Application Rate)
{
	int error;
	
	enum enum_GBERT10000_CheckerMode eGBERT10000_CheckerMode;

	error = GBERT10000_Selftest_withSN_DLL (sn, inst);
	if (error) return -1;
	
	eGBERT10000_CheckerMode = GBERT_SingleRun;
	
	error = GBERT10000_Initialize_DLL (*inst, Pattern, Rate, eGBERT10000_CheckerMode, GBERT10000_XFPin_en, GBERT10000_XFPout_en, GBERT10000_SMAout_en, GBERT10000_XFPTxDisable,
										GBERT10000_OutputPolarity, GBERT10000_InputPolarity, GBERT10000_InputEqualization, GBERT10000_OutputSwing, GBERT10000_OutputSlewRate);
	if (error) return -1;   
		
	return 0;
}

int GBERT10000_Check (int inst, double GatingTime, double *ErrorRatio, double *ErrorBit)
{
	int error;
	
	error = GBERT10000_Check_DLL (inst, GatingTime, ErrorRatio, ErrorBit);
	if (error<0) return -1;
	
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------
//   �������GBERT10000_Check_DLL����-1�����ش����������GBERT10000_Check_DLL����-2��-3����û�н��յ����ݣ�����������Ϊ1
//-----------------------------------------------------------------------------------------------------------------------------
int GBERT10000_Check_10G_ONU (int inst, double GatingTime, double *ErrorRatio, double *ErrorBit)
{
	int error;
	
	error = GBERT10000_Check_DLL (inst, GatingTime, ErrorRatio, ErrorBit);   //return   0; //Test completed with no errors
                                                                             //return  -1; //MCU/CPU/CRU access error
                                                                             //return  -2; //Test LOS or LOL at sometime before
                                                                             //return  -3; //Test lost sync at sometime before
																			 //return  -4; //Test completed with error bits 
	if (error==-1) 
		return -1;
	else if(error==-2||error==-3)
		     *ErrorRatio=1;
		 else 
		     return 0; 
	
	return 0;
}

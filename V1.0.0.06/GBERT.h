/****************************************************************************
 *
 * File:                gbert.h
 *
 * Author:              Roger Li
 *
 * Description:         gbert�������ܴ���
 *
 * Time:                2010-07-08
 *
 * version:				V2.0.0.2
 *
�汾��ʷ
V2.0.0.2:
���Ӷ���
enum enum_GBERT_Pattern
enum enum_GBERT_Application
���ļ�GBERT_DLL.h�̳ж���

V2.0.0.1 
���Ӻ��� GVPM_GET_SN
���Ӻ��� GBERT_Check_Ratio
���Ӻ��� GBERT_GET_SN

V2.0.0.0
1.���GVPM����ģʽ�������
2.GVPM_SET_Attenuate������Ӳ��������������̨GVPMͬʱʹ�ã�ʹ��ȫ�ֱ���GOAM_WaveLength���»���
3.GVPM_Init�������˥������lockģʽ�µĴ���

****************************************************************************/

#ifndef _GBERT_H_
#define _GBERT_H_ 

#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>

enum enum_GBERT_Pattern 
{   
	GBERT_PRBS7,
	GBERT_PRBS23,
	GBERT_PRBS31,
	GBERT_User_defined_64bit
}; //GBERT_Pattern;	

enum enum_GBERT_Application 
{   
	GBERT_R200Mbps,
	GBERT_R125Mbps,
	GBERT_R1250Mbps,
	GBERT_R1062Mbps,
	GBERT_R2125Mbps,
	GBERT_R4250Mbps,
	GBERT_R2500Mbps,
	GBERT_R155Mbps,
	GBERT_R622Mbps,
	GBERT_R2488Mbps
}; //GBERT_Application;	

enum enum_GBERT10000_Application 
{   
   	GBERT_R9953Mbps,
	GBERT_R10709Mbps,
	GBERT_R10312Mbps,
	GBERT_R11095Mbps,
	GBERT_R10518Mbps,
	GBERT_R11320Mbps,
	GBERT_Ruser1Mbps,	//user1 defined
	GBERT_Ruser2Mbps,	//user2 defined
	GBERT_Ruser3Mbps,	//user3 defined
	GBERT_Ruser4Mbps 	//user4 defined
};	

enum enum_GBERT10000_CheckerMode 
{   
	GBERT_SingleRun
};// eGBERT10000_CheckerMode;	

//int GBERT_ErrorCode; 

extern BOOL GBERT_Init(int *GBERT_USBHandle, unsigned char *GBERT_SerialNumber, int Pattern, int Rate, int myBERT_SFPTxDisable);
extern BOOL GBERT_Init_SFPTxDisable(int *GBERT_USBHandle, unsigned char *GBERT_SerialNumber, int myBERT_SFPTxDisable);
extern BOOL GBERT_Start(int GBERT_USBHandle, double testTime);
extern BOOL GBERT_Stop(int GBERT_USBHandle);
extern BOOL GBERT_RESET(int GBERT_USBHandle); 
extern BOOL GBERT_Check(int GBERT_USBHandle, unsigned int *ErrorBit);  		//GBERT_ErrorCode==1 һ����������û����� ��ʱ1s��ȴ���һ��ѭ��
extern BOOL GBERT_Check_Start(int GBERT_USBHandle, unsigned int *ErrorBit); //GBERT_ErrorCode <0 ״̬��ȡ�д��� ��ʱ1s��ȴ���һ�Σ������ڷ���ʼ��������󣬶�ȡ����״̬
extern int GBERT_READ_Status (int inst_GBERT, unsigned int *ErrorBit);		//��ȡGEBRT״̬, �����ݶ�ȡ��״̬����λGBERT���� 
extern BOOL GBERT_Check_Ratio(int GBERT_USBHandle, double *ErrorRatio, int *errorcode);     //������
extern int GBERT_GET_SN(int panel, int control);
//2009-04-09 add by roger for GVPM
extern int GVPM_GET_SN(int panel, int control); //100407
extern int GVPM_Init (int *GVPM_USBHandle, unsigned char *GVPM_SerialNumber, int Wavelength, float AttValue);
extern int GVPM_SET_Attenuate (int GVPM_USBHandle, float AttValue, int Wavelength);
extern int GVPM_SET_OutputEnable (int GVPM_USBHandle, int GVPM_OutputEnable); 
extern int GVPM_SET_LockEnable (int inst_GVPM, int lockEnable);	
extern int GVPM_SET_LockAttenuate (int inst_GVPM, double LockAttValue, int Wavelength, int checklock); //checklock��־�Ƿ���GVPM�ѵ����ȶ���Lock���״̬

int GBERT10000_Init (int *inst, char sn[], enum enum_GBERT_Pattern Pattern, enum enum_GBERT10000_Application Rate);
int GBERT10000_Check (int inst, double GatingTime, double *ErrorRatio, double *ErrorBit);
int GBERT10000_Check_10G_ONU (int inst, double GatingTime, double *ErrorRatio, double *ErrorBit); 
#endif  

//NOTE: if this DLL will be called by VC++, the global variables may be added "extern", and define the global variables in main C file.
//V1.0.0.1, add user definition patern check;
//V2.0.0.1, add GOAM;
//V3.0.0.1, add GOPM; update the wavelength variable type from unsigned int into int
//V4.0.0.1, add GOAM lock output functions
//V5.0.0.1, speed up the GBERT_Initialize_DLL() time
//V6.0.0.1, fix a bug on the BERT_Application
//V7.0.0.1, add the sub-functions such as GBERT_Initialize_PPG_Set_Pattern_DLL() and GBERT_Check_ED_Get_ErrorRatio_DLL() 
// which are separated from the original functions as int __stdcall GBERT_Initialize_DLL(), GBERT_StartCheck_DLL(),
// GBERT_StartCheck_DLL(), GBERT_GetCheckStatus_DLL() and GBERT_StopCheck_DLL() 
//V8.0.0.1, fix a bug under multi-thread application

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================================*/
// GMeter
/*==========================================================================================*/
int GMeter_USBHandle;
int GBERT_ErrorCode;
char GMeter_SerialNumber[17];

int __stdcall GMeter_GetSN_DLL(int GMeter_USBHandle, char *GMeter_SerialNumber);

/*==========================================================================================*/
// GBERT
/*==========================================================================================*/
int GBERT_USBHandle;
char GBERT_SerialNumber[17];   

//BERT pattern
enum emum_BERT_Pattern 
{   PRBS7,
	PRBS23,
	PRBS31,
	User_defined_64bit
};	
enum emum_BERT_Pattern BERT_Pattern; 

//BERT application and rate 
enum emum_BERT_Application 
{   R200Mbps,
	R125Mbps,
	R1250Mbps,
	R1062Mbps,
	R2125Mbps,
	R4250Mbps,
	R2500Mbps,
	R155Mbps,
	R622Mbps,
	R2488Mbps
};	
enum emum_BERT_Application BERT_Application; 

//double BERT_Rate[10] = {0.2*1E9, 0.125*1E9, 1.25*1E9, 1.0625*1E9, 2.1250*1E9, 4.2500*1E9, 2.5000*1E9, 0.15552*1E9, 0.62208*1E9, 2.48832*1E9};

//BERT check mode; //NOTE: FreeRun is not supported in this version
enum emum_BERT_CheckerMode 
{   SingleRun,
	FreeRun
};	
enum emum_BERT_CheckerMode BERT_CheckerMode; 

//BERT single run gating time, unit in second
double BERT_GatingTime;

//BERT ErrorRatio Threshold forReSync; //NOTE: re-sync is not supported in this version 
double BERT_ErrorRatioThresholdforReSync;

//BERT SFP TD+/- electronic port control, 1=enable, 0=disable 
int BERT_SFPout_en;

//BERT Checker input choice, 1=from SFP RD+/- electronic port, 0=from SMA RD+/- electronic port, 
int BERT_SFPin_en;

//BERT SMA TD+/- electronic port control, 1=enable, 0=disable 
int BERT_SMAout_en;

//BERT CLK electronic port output indicator, 1=enable, 0=disable; 
//NOTE: only when the pattern=4.25G will turn off the CLK output
int BERT_Clockout_en;

//BERT Trig electronic port output indicator, 1=enable, 0=disable; 
//NOTE: always turn on
int BERT_Triggerout_en;

//BERT Trig electronic port output indicator, 1=enable, 0=disable; 
int BERT_InputPolarity; BERT_OutputPolarity; BERT_SFPTxDisable;

//BERT electronic port control; //NOTE: don't change the values  
int BERT_InputEqualization; BERT_OutputSwing;  BERT_OutputSlewRate;

//BERT testing result, Error Bits Count
double BERT_Accumulated_ErrorCount;

//BERT testing result, Total Bits Count
double BERT_Accumulated_BitCount; 

//BERT testing result, Error Bits Ratio
double BERT_Accumulated_ErrorRatio;

//BERT testing result, Testing Elapsed Time
double BERT_Accumulated_ElapsedTime;

//BERT User defined bits
unsigned char BERT_PATCHK_UD0, BERT_PATCHK_UD1, BERT_PATCHK_UD2, 
	BERT_PATCHK_UD3, BERT_PATCHK_UD4, BERT_PATCHK_UD5, 
	BERT_PATCHK_UD6, BERT_PATCHK_UD7;


int __stdcall GBERT_Selftest_DLL (int *GBERT_USBHandle);
//return  0; //GBERT Selftest done.
//return -1; //GBERT not found!
//return -2; //MCU access error!
//return -3; //+5.0V VCC error!
//return -4; //+3.3V VCC error! 
//return -5; //+1.8V VCC error! 
//return -6; //MCU access error!
//return -7; //CPU not found!
//return -8; //CPU access error!
//
//int *GBERT_USBHandle
//=-1;       //GBERT not found!  
//=0~255;    //GBERT found! 
//NOTE: if two or more GBERT are connected with PC, only one GBERT can be software accessible.

int __stdcall GBERT_Selftest_withSN_DLL(char *GBERT_SerialNumber, int *GBERT_USBHandle);
//return  0; //GBERT Selftest done.
//return -1; //GBERT not found!
//return -2; //MCU access error!
//return -3; //+5.0V VCC error!
//return -4; //+3.3V VCC error! 
//return -5; //+1.8V VCC error! 
//return -6; //MCU access error!
//return -7; //CPU not found!
//return -8; //CPU access error!
//
//int *GBERT_USBHandle
//=-1;       //GBERT not found!  
//=0~255;    //GBERT found! 
//NOTE: if two or more GBERT are connected with PC, only the GBERT which has the expected GBERT_SerialNumber can be software accessible. 

int __stdcall GBERT_Initialize_DLL(	int GBERT_USBHandle,
				int BERT_Pattern, int BERT_Application, int BERT_CheckerMode,
				int BERT_SFPin_en, int BERT_SFPout_en, int BERT_SMAout_en, int BERT_SFPTxDisable,
				int BERT_OutputPolarity, int BERT_InputPolarity,
				int BERT_InputEqualization, int BERT_OutputSwing, int BERT_OutputSlewRate);
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!
 
int __stdcall GBERT_SetUserBits_DLL(int GBERT_USBHandle, unsigned char BERT_PATCHK_UD0, unsigned char BERT_PATCHK_UD1, 
				unsigned char BERT_PATCHK_UD2, unsigned char BERT_PATCHK_UD3, unsigned char BERT_PATCHK_UD4, 
				unsigned char BERT_PATCHK_UD5, unsigned char BERT_PATCHK_UD6, unsigned char BERT_PATCHK_UD7);
//return  0; //GBERT SetUserBits done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!


int __stdcall GBERT_StartCheck_DLL (int GBERT_USBHandle, double BERT_GatingTime);
//return  0; //GBERT star check done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error! 
//return -5; //CPU write register error!

int __stdcall GBERT_GetCheckStatus_DLL (int GBERT_USBHandle, 
				double *BERT_Accumulated_ErrorRatio, double *BERT_Accumulated_BitCount, 
				double *BERT_Accumulated_ErrorCount, double *BERT_Accumulated_ElapsedTime);
//return   0; //Test completed with no errors
//return  +1; //Test in progress, and currently no errors occured
//return  -1; //CPU access error
//return  -2; //Test completed with errors
//return  -3; //Test completed with high BER, and error counter is full
//return  -4; //Test in progress, and trying to sync to incoming pattern
//return  -5; //Test in progress, and currently no errors occured 
//return  -6; //Test in progress, and currently with errors occurred
//return  -7; //Test was stopped before pattern sync occurred


int __stdcall GBERT_StopCheck_DLL(int GBERT_USBHandle); 
//return   0; //GBERT stop check done
//return  -1; //CPU access error

/*=======================================================================================================================*/
//V7.0.0.1, add the sub-functions such as GBERT_Initialize_PPG_Set_Pattern_DLL() and GBERT_Check_ED_Get_ErrorRatio_DLL() 
// which are separated from the original functions as int __stdcall GBERT_Initialize_DLL(), GBERT_StartCheck_DLL(),
// GBERT_StartCheck_DLL(), GBERT_GetCheckStatus_DLL() and GBERT_StopCheck_DLL() 
/*=======================================================================================================================*/
int __stdcall GBERT_Initialize_PPG_Set_Pattern_DLL(int GBERT_USBHandle, int BERT_Pattern);
//����PPG���ͣ�BERT_Pattern=0��PRBS7��BERT_Pattern=1��PRBS23��BERT_Pattern=2��PRBS31��BERT_Pattern=3��User_defined_64bit
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_Application_DLL(int GBERT_USBHandle, int BERT_Application);
//����PPG���ʣ�BERT_Application=0��R200Mbps��BERT_Application=1��R125Mbps��BERT_Application=2��R1250Mbps��
//             BERT_Application=3��R1062Mbps��BERT_Application=4��R2125Mbps��BERT_Application=5��R4250Mbps��
//             BERT_Application=6��R2500Mbps��BERT_Application=7��R155Mbps��BERT_Application=8��R622Mbps��
//             BERT_Application=9��R2488Mbps
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_SFPoutEn_DLL(int GBERT_USBHandle, int BERT_SFPout_en);
//����PPG��SFP����TD+/-����ź����ʹ�ܣ�BERT_SFPout_en=0���ֹSFP����TD+/-����ź������BERT_SFPout_en=1��ʹ��SFP����TD+/-����ź����
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_SMAoutEn_DLL(int GBERT_USBHandle, int BERT_SMAout_en);
//����PPG��SMA����TD+/-����ź����ʹ�ܣ�BERT_SMAout_en=0���ֹSMA����TD+/-����ź������BERT_SMAout_en=1��ʹ��SMA����TD+/-����ź����
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_SFPTxDisable_DLL(int GBERT_USBHandle, int BERT_SFPTxDisable);
//����PPG��SFP����TxTisable�����źţ�BERT_SFPTxDisable=0��ʹ��SFP��������BERT_SFPTxDisable=1���ֹSFP������
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_OutputPolarity_DLL(int GBERT_USBHandle, int BERT_OutputPolarity);
//����PPG������źż��ԣ�BERT_OutputPolarity=0������źż���������BERT_OutputPolarity=1������źż��Է�ת
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_ED_Set_InputEqualization_DLL(int GBERT_USBHandle, int BERT_InputEqualization);
//����ED��������������BERT_InputEqualization=0�����7dB��BERT_InputEqualization=1�����4dB��BERT_InputEqualization=2�����0.5dB��BERT_InputEqualization=3�����0dB
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_OutputSwing_DLL(int GBERT_USBHandle, int BERT_OutputSwing);
//����PPG������źŷ��ȣ�BERT_OutputSwing=0�������ͨ���ȣ�BERT_OutputSwing=1�����25%���ȣ�BERT_OutputSwing=2�����50%����
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_PPG_Set_OutputSlewRate_DLL(int GBERT_USBHandle, int BERT_OutputSlewRate);
//����PPG������ź�б�ʣ�BERT_OutputSlewRate=0����ͨб�ʣ�BERT_OutputSlewRate=0��75%б��
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_ED_Set_CheckerMode_DLL(int GBERT_USBHandle, int BERT_CheckerMode);
//����ED����ģʽ��BERT_CheckerMode=0�򵥴μ��������õ�GatingTimeʱ���ֹͣ��飬BERT_CheckerMode=1��ѭ����һֱ���ֱ����������������
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_ED_Set_InputSource_DLL(int GBERT_USBHandle, int BERT_SFPin_en);
//����ED�ı����ź�Դ��BERT_SFPin_en=0�����SMA �ն˵���źţ�BERT_SFPin_en=1�����SFP���ն˹���ź�
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_ED_Set_InputPolarity_DLL(int GBERT_USBHandle, int BERT_InputPolarity);
//����ED�������źż��ԣ�BERT_InputPolarity=0������źż���������BERT_InputPolarity=1������źż��Է�ת
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error!
//return -5; //CPU write register error!; 

int __stdcall GBERT_Initialize_ED_Set_GatingTime_DLL (int GBERT_USBHandle, double BERT_GatingTime);
//����ED�ĵ��μ�������ʱ�䣬BERT_GatingTime��λ����
//return  0; //GBERT initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error! 
//return -5; //CPU write register error!  

int __stdcall GBERT_Check_ED_Set_StartCheck_DLL (int GBERT_USBHandle);
//����ED�������
//return  0; //GBERT star check done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //MCU access error!
//return -4; //CPU access error! 
//return -5; //CPU write register error!

int __stdcall GBERT_Check_ED_Get_ErrorRatio_DLL (int GBERT_USBHandle, double *BERT_Accumulated_ErrorRatio);
//�ض�ED��ǰ�ۼƵ�������
//return   0; //Test completed with no errors
//return  +1; //Test in progress, and currently no errors occured
//return  -1; //CPU access error
//return  -2; //Test completed with errors
//return  -3; //Test completed with high BER, and error counter is full
//return  -4; //Test in progress, and trying to sync to incoming pattern
//return  -5; //Test in progress, and currently no errors occured 
//return  -6; //Test in progress, and currently with errors occurred
//return  -7; //Test was stopped before pattern sync occurred

int __stdcall GBERT_Check_ED_Get_BitCount_DLL (int GBERT_USBHandle, double *BERT_Accumulated_BitCount);
//�ض�ED��ǰ�ۼƵ���Ч������ָ���
//return   0; //Test completed with no errors
//return  +1; //Test in progress, and currently no errors occured
//return  -1; //CPU access error
//return  -2; //Test completed with errors
//return  -3; //Test completed with high BER, and error counter is full
//return  -4; //Test in progress, and trying to sync to incoming pattern
//return  -5; //Test in progress, and currently no errors occured 
//return  -6; //Test in progress, and currently with errors occurred
//return  -7; //Test was stopped before pattern sync occurred

int __stdcall GBERT_Check_ED_Get_ErrorCount_DLL (int GBERT_USBHandle, double *BERT_Accumulated_ErrorCount);
//�ض�ED��ǰ�ۼƵ��������
//return   0; //Test completed with no errors
//return  +1; //Test in progress, and currently no errors occured
//return  -1; //CPU access error
//return  -2; //Test completed with errors
//return  -3; //Test completed with high BER, and error counter is full
//return  -4; //Test in progress, and trying to sync to incoming pattern
//return  -5; //Test in progress, and currently no errors occured 
//return  -6; //Test in progress, and currently with errors occurred
//return  -7; //Test was stopped before pattern sync occurred

int __stdcall GBERT_Check_ED_Get_ElapsedTime_DLL (int GBERT_USBHandle, double *BERT_Accumulated_ElapsedTime);
//�ض�ED��ǰ�ۼƵ���Ч���ʱ��
//return   0; //Test completed with no errors
//return  +1; //Test in progress, and currently no errors occured
//return  -1; //CPU access error
//return  -2; //Test completed with errors
//return  -3; //Test completed with high BER, and error counter is full
//return  -4; //Test in progress, and trying to sync to incoming pattern
//return  -5; //Test in progress, and currently no errors occured 
//return  -6; //Test in progress, and currently with errors occurred
//return  -7; //Test was stopped before pattern sync occurred

int __stdcall GBERT_Check_ED_Set_StopCheck_DLL(int GBERT_USBHandle); 
//ֹͣED�������
//return   0; //GBERT stop check done
//return  -1; //CPU access error




/*==========================================================================================*/
// GOAM
/*==========================================================================================*/
int GOAM_USBHandle;
int GOAM_ErrorCode;
char GOAM_SerialNumber[17];

int GOAM_WaveLength1, GOAM_WaveLength2, GOAM_WaveLength3, GOAM_WaveLength4;
double GOAM_MaxAttenuateValue1, GOAM_MaxAttenuateValue2, GOAM_MaxAttenuateValue3, GOAM_MaxAttenuateValue4;
int GOAM_OutputEnable;
int GOAM_WaveLength;
double GOAM_Attenuate;
double GOAM_Temperature;

int GOAM_LockEnable;
double GOAM_LockOPdBm;
int GOAM_LockErrorCode;

int __stdcall GOAM_Selftest_DLL(char *GOAM_SerialNumber, int *GOAM_USBHandle);
//return  0; //OAM Selftest done.
//return -1; //OAM SN not found!
//return -2; //MCU access error!
//return -3; //+5.0V VCC error!
//return -4; //+3.3V VCC error! 
//
//char *OAM_SerialNumber, input, the expected target OAM's SerialNumber
//int *OAM_USBHandle, output, the USBHandle value which match the SerialNumber

int __stdcall GOAM_Initialize_DLL(int GOAM_USBHandle, double *GOAM_Temperature, int *GOAM_OutputEnable,
									int *GOAM_WaveLength, double *GOAM_Attenuate,
									int *GOAM_WaveLength1, double *GOAM_MaxAttenuateValue1, 
									int *GOAM_WaveLength2, double *GOAM_MaxAttenuateValue2, 
									int *GOAM_WaveLength3, double *GOAM_MaxAttenuateValue3, 
									int *GOAM_WaveLength4, double *GOAM_MaxAttenuateValue4);
									
//return  0; //GOAM initialize done
//return -1; //MCU not found!
//return -2; //MCU access error!
//return -3; //MCU firmware not support
//return -4; //GOAM not calibrated


int __stdcall GOAM_Attenuate_DLL(int GOAM_USBHandle, int GOAM_WaveLength, double GOAM_Attenuate);
//return  0; //GOAM attenuate done
//return -2; //MCU access error!
//return -3; //the GOAM_Attenuate is beyond the max calibrated attenuation value
//return -4; //the GOAM_WaveLength not supported
//return -5; //GOAM is in Lock-Output-Mode, so Free-Attenuation-Mode is not support

int __stdcall GOAM_OutputEnable_DLL(int GOAM_USBHandle, int GOAM_OutputEnable);
//return  0; //GOAM OutputEnable done
//return -2; //MCU access error!

int __stdcall GOAM_SaveConfiguration_DLL(int GOAM_USBHandle, int GOAM_OutputEnable, 
											int GOAM_WaveLength, double GOAM_Attenuate);
//return  0; //GOAM SaveConfiguration done
//return -2; //MCU access error!

int __stdcall GOAM_GOPM_LockOutput_GetStatus_DLL(int GOAM_USBHandle, int *GOAM_WaveLength, double *GOAM_LockOPdBm, int *GOAM_LockEnable, 
										double *GOAM_Attenuate, double *GOPM_OPdBm, double *GOPM_OPmW);
//return  1; //trying to be locked
//return  0; //was locked
//return -1; //unkown error
//return -2; //MCU access error!
//return -3; //MCU firmware not support 
//return -4; //the GOPM wavelenth is not equal to that of the GOAM 
//return -5; //the expected GOAM_LockOPdBm is too low to be locked
//return -6; //the expected GOAM_LockOPdBm is too high to be locked 
//return -7; //the GOAM output has been shut down 

int __stdcall GOAM_LockAttenuateInit_DLL(int GOAM_USBHandle, int GOAM_WaveLength, double GOAM_LockOPdBm);
//return  0; //GOAM LockAttenuateInit done
//return -2; //MCU access error!
//return -3; //MCU firmware not support

int __stdcall GOAM_LockEnable_DLL(int GOAM_USBHandle, int GOAM_LockEnable);
//return  0; //GOAM lock command sent done
//return -2; //MCU access error!
//return -3; //MCU firmware not support 


/*==========================================================================================*/
// GOPM
/*==========================================================================================*/
int GOPM_USBHandle;
int GOPM_ErrorCode;
char GOPM_SerialNumber[17];   

int GOPM_ErrorCode;
double GOPM_Temperature;
double GOPM_MinimalOP;
int GOPM_WaveLength1, GOPM_WaveLength2, GOPM_WaveLength3, GOPM_WaveLength4, GOPM_WaveLength5, GOPM_WaveLength6, GOPM_WaveLength7;
int GOPM_WaveLength;
double GOPM_OPreference;
double GOPM_OPdBm, GOPM_OPmW;

int __stdcall GOPM_Selftest_DLL(char *GOPM_SerialNumber, int *GOPM_USBHandle);
//return  0; //OPM Selftest done.
//return -1; //OPM SN not found!
//return -2; //MCU access error!
//return -3; //+5.0V VCC error!
//return -4; //+3.3V VCC error! 
//
//char *GOPM_SerialNumber, input, the expected target GMeter's SerialNumber
//int *GOPM_USBHandle, output, the USBHandle value which match the SerialNumber

int __stdcall GOPM_Initialize_DLL(int GOPM_USBHandle, double *GOPM_Temperature, double *GOPM_MinimalOP,		   
									int *GOPM_WaveLength, double *GOPM_OPreference,
									int *GOPM_WaveLength1, int *GOPM_WaveLength2, int *GOPM_WaveLength3,  
									int *GOPM_WaveLength4, int *GOPM_WaveLength5, int *GOPM_WaveLength6, 
									int *GOPM_WaveLength7); 
//return  0; //GOPM initialize done
//return -1; //MCU not found!
//return -2; //MCU access error!
//return -3; //MCU firmware not support
//return -4; //GOPM not calibrated 

int __stdcall GOPM_SetOPreference_DLL(int GOPM_USBHandle, double GOPM_OPreference);
//return  0; //GOPM set optical power reference done
//return -2; //MCU access error!

int __stdcall GOPM_GetOPreference_DLL(int GOPM_USBHandle, double *GOPM_OPreference);
//return  0; //GOPM set optical power reference done
//return -2; //MCU access error!

int __stdcall GOPM_SetWaveLength_DLL(int GOPM_USBHandle, int GOPM_WaveLength);
//return  0; //GOPM set optical power reference done
//return -2; //MCU access error!
//return -4; //the GOPM_WaveLength not supported

int __stdcall GOPM_GetWaveLength_DLL(int GOPM_USBHandle, int *GOPM_WaveLength);
//return  0; //GOPM set optical power reference done
//return -2; //MCU access error!
//return -4; //the GOPM_WaveLength not supported

int __stdcall GOPM_SaveConfiguration_DLL(int GOPM_USBHandle, int GOPM_WaveLength, double GOPM_OPreference);
//return  0; //GOPM SaveConfiguration done
//return -2; //MCU access error!

int __stdcall GOPM_GetOP_DLL(int GOPM_USBHandle, double *GOPM_OPdBm, double *GOPM_OPmW);
//return  0; //GOPM get optical power done
//return -2; //MCU access error!
//return -3; //The input optical power is too small to be accurately measured
//return -4; //the GOPM_WaveLength not supported


#ifdef __cplusplus
}
#endif

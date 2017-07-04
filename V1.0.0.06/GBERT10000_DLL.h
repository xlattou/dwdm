//NOTE: if this DLL will be called by VC++, the global variables may be added "extern", and define the global variables in main C file.
//V1.0.0.0, 2011-03-02;


#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================================*/
// GMeter
/*==========================================================================================*/
int GMeter_USBHandle;
char GMeter_SerialNumber[17];

int __stdcall GBERT10000_GetSN_DLL(int GMeter_USBHandle, char *GMeter_SerialNumber);

/*==========================================================================================*/
// GBERT10000
/*==========================================================================================*/
int GBERT10000_USBHandle;
char GBERT10000_SerialNumber[17]; 
int GBERT10000_ErrorCode; 
double GBERT10000_Accumulated_ErrorRatio, GBERT10000_Accumulated_ErrorCount; 

//GBERT10000 pattern
enum emum_GBERT10000_Pattern 
{   ePRBS7,
	ePRBS23,
	ePRBS31
};	
enum emum_GBERT10000_Pattern GBERT10000_Pattern=ePRBS7; 

//GBERT10000 application 
enum emum_GBERT10000_Application 
{   R9953Mbps,
	R10709Mbps,
	R10312Mbps,
	R11095Mbps,
	R10518Mbps,
	R11320Mbps,
	Ruser1Mbps, //user1 defined
	Ruser2Mbps, //user2 defined
	Ruser3Mbps,  //user3 defined
	Ruser4Mbps  //user4 defined
};	
enum emum_GBERT10000_Application GBERT10000_Application=R9953Mbps; 

//GBERT10000 data rate. //NOTE: the last 4 user-defined-data-rates should be in the rage of [9.95Gbps..11.32Gbps]
double GBERT10000_Rate[10]    = {9953280.000*1E3, 10709225.316*1E3, 10312500.000*1E3, 11095727.848*1E3, 10518750.000*1E3, 11317642.405*1E3, 1*1E10, 1*1E10, 1*1E10, 1*1E10};
//GBERT10000 inner clock rate. //NOTE: the inner clock rate shoule be the 1/16 of the data rate
//double GBERT10000_CLKRate[10] = {622080.000*1E3,  669326.582*1E3,   644531.250*1E3,   693482.991*1E3,   657421.875*1E3,   707352.650*1E3,   1*1E6,  1*1E6,  1*1E6,  1*1E6};

//GBERT10000 check mode; //NOTE: FreeRun is not supported in this version
enum emum_GBERT10000_CheckerMode 
{   eSingleRun
};	
enum emum_GBERT10000_CheckerMode GBERT10000_CheckerMode = eSingleRun; 

//GBERT10000 single run gating time, unit in second
double GBERT10000_GatingTime=1.0;

//GBERT10000 ErrorRatio Threshold forReSync; //NOTE: re-sync is not supported in this version 
double GBERT10000_ErrorRatioThresholdforReSync=1.0E-2;

//GBERT10000 XFP TD+/- electronic port control, 1=enable, 0=disable 
int GBERT10000_XFPout_en=1;

//GBERT10000 Checker input choice, 1=from XFP RD+/- electronic port, 0=from SMA RD+/- electronic port, 
int GBERT10000_XFPin_en=0;

//GBERT10000 SMA TD+/- electronic port control, 1=enable, 0=disable 
int GBERT10000_SMAout_en=1;

//GBERT10000 CLK electronic port output indicator, 1=enable, 0=disable; 
int GBERT10000_Clockout_en=1;

//GBERT10000 Trig electronic port output indicator, 1=enable, 0=disable; 
int GBERT10000_InputPolarity=0; GBERT10000_OutputPolarity=0; GBERT10000_XFPTxDisable=0;

//GBERT10000 electronic port control; //NOTE: don't change the values  
int GBERT10000_InputEqualization=3; GBERT10000_OutputSwing=0;  GBERT10000_OutputSlewRate=0;


int __stdcall GBERT10000_Selftest_DLL (int *GBERT10000_USBHandle);
//return  0; //GBERT10000 Selftest done.
//return -1; //GBERT10000 not found!
//return -2; //MCU access error!
//return -3; //+5.0V VCC error!
//return -4; //+3.3V VCC error! 
//return -5; //+1.8V VCC error! 
//return -6; //MCU access error!
//return -7; //CPU not found!
//return -8; //CPU access error!
//
//int *GBERT10000_USBHandle
//=-1;       //GBERT10000 not found!  
//=0~255;    //GBERT10000 found! 
//NOTE: if two or more GBERT10000 are connected with PC, only one GBERT10000 can be software accessible.

int __stdcall GBERT10000_Selftest_withSN_DLL (char *GBERT10000_SerialNumber, int *GBERT10000_USBHandle);
//return  0; //GBERT10000 Selftest done.
//return -1; //GBERT10000 not found!
//return -2; //MCU access error!
//return -3; //+5.0V VCC error!
//return -4; //+3.3V VCC error! 
//return -5; //+1.8V VCC error! 
//return -6; //MCU access error!
//return -7; //CPU not found!
//return -8; //CPU access error!
//
//int *GBERT10000_USBHandle
//=-1;       //GBERT10000 not found!  
//=0~255;    //GBERT10000 found! 
//NOTE: if two or more GBERT are connected with PC, only the GBERT which has the expected GBERT_SerialNumber can be software accessible. 


int __stdcall GBERT10000_Initialize_DLL(	int GBERT10000_USBHandle,
				int GBERT10000_Pattern, int GBERT10000_Application, int GBERT10000_CheckerMode,
				int GBERT10000_XFPin_en, int GBERT10000_XFPout_en, int GBERT10000_SMAout_en, 
				int GBERT10000_XFPTxDisable, int GBERT10000_OutputPolarity, int GBERT10000_InputPolarity,
				int GBERT10000_InputEqualization, int GBERT10000_OutputSwing, int GBERT10000_OutputSlewRate);
//return  0; //GBERT10000 initialize done
//return -1; //MCU not found!
//return -2; //CPU not found!
//return -3; //CMU not found!
//return -4; //MCU access error!
//return -5; //CPU access error! 
//return -6; //CMU access error! 
 

int __stdcall GBERT10000_Check_DLL (int GBERT10000_USBHandle, double GBERT10000_GatingTime, 
									double *GBERT10000_Accumulated_ErrorRatio, double *GBERT10000_Accumulated_ErrorCount);
//return   0; //Test completed with no errors
//return  -1; //MCU/CPU/CRU access error
//return  -2; //Test LOS or LOL at sometime before
//return  -3; //Test lost sync at sometime before
//return  -4; //Test completed with error bits 


#ifdef __cplusplus
}
#endif

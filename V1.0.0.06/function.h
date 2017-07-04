/****************************************************************************
 *
 * File:                function.h
 *
 * Author:              Roger Li
 *
 * Description:         自动调测程序基本功能代码，从原有OLT ATE_Function和ATE_Function文件合并过来
 *
 * Time:                2010-01-07
 *
 * version:				v1.1.0.0
 *
****************************************************************************/
#ifndef _FUNCTION_H_
#define _FUNCTION_H_ 

#include <windows.h>
#include <utility.h>  
#include <formatio.h>
#include <visatype.h>
#include "CH341A_DLL.h"
#include "global.h"
#include "cvidll.h"  
#include "CSA8000.h" 
#include "Ag86100.h"    
#include "XGSPON OLT Parallel ATE.h"  
#include "JHFSW.h" 
#include "COFFSW.h"  
#include "FSW.h"
#include "SEVB0027_4001.h"  
#include "XFP.h" 
#define	SOFTVER  "V1.0.0.00"
#define SOFTDATE "2017.03.13"

#define DEBUG 1 

int panSN;  

char g_ConfigFileName[MAX_PATHNAME_LEN]; //for config file
char g_InstFileName[MAX_PATHNAME_LEN]; //for Instrument Add file  

static int gCtrl_BOX[CHANNEL_MAX] = {PAN_MAIN_TEXTBOX_CHAN0, PAN_MAIN_TEXTBOX_CHAN1, PAN_MAIN_TEXTBOX_CHAN2, PAN_MAIN_TEXTBOX_CHAN3, 
							 		 PAN_MAIN_TEXTBOX_CHAN4, PAN_MAIN_TEXTBOX_CHAN5, PAN_MAIN_TEXTBOX_CHAN6, PAN_MAIN_TEXTBOX_CHAN7}; 
static int gCtrl_BIN[CHANNEL_MAX] = {PAN_MAIN_BIN_CHAN0, PAN_MAIN_BIN_CHAN1, PAN_MAIN_BIN_CHAN2, PAN_MAIN_BIN_CHAN3, 
							 		 PAN_MAIN_BIN_CHAN4, PAN_MAIN_BIN_CHAN5, PAN_MAIN_BIN_CHAN6, PAN_MAIN_BIN_CHAN7}; 
static int gCtrl_LED[CHANNEL_MAX] = {PAN_MAIN_LED_CHAN0, PAN_MAIN_LED_CHAN1, PAN_MAIN_LED_CHAN2, PAN_MAIN_LED_CHAN3, 
							 		 PAN_MAIN_LED_CHAN4, PAN_MAIN_LED_CHAN5, PAN_MAIN_LED_CHAN6, PAN_MAIN_LED_CHAN7}; 

BOOL g_HZDCA_Init_Flag;

//char g_buf1[128];
//char g_buf2[128];

//定义Config结构体
struct  struConfig 
{
	int		PNIndex;
	char	PN[50];
	int 	VerIndex;
	char	Ver[50];

	BOOL	DT_Tun_10G;
	BOOL	DT_Test_10G;
	BOOL	DT_Test_1G; 
	BOOL    DT_QATest_10G;
	BOOL	DT_QATest_1G;
	BOOL    DT_DWDP_TEST;
	BOOL    DT_Tun_DWDM;   // 表示调试dwdm
	BOOL    DT_Test_DWDM;  // 表示测试dwdm

	char	DT_Flag[50];
	BOOL  	Temper_Low;
	BOOL	Temper_Room;
	BOOL	Temper_High;
	char	Temper_Flag[50];
	
	//EED Table Flag
	BOOL    A0_low;
	BOOL    A0_tab1;
	BOOL    A0_tab0;
	BOOL    A0_tab2;
	BOOL    A2_low;
	BOOL    A2_tab0;
	BOOL    A2_tab1;

	//item
	BOOL    Sel_T_AOP; 
	BOOL    Sel_T_ER;
	BOOL    Sel_T_TxDis;
	BOOL    Sel_T_TxDis_soft;  
	BOOL    Sel_T_Eye;
	BOOL	Sel_T_Mask;
	BOOL    Sel_T_Spectrum;
	BOOL 	Sel_R_Sens;
	BOOL 	Sel_R_Sens_Real;  
	BOOL 	Sel_R_Over;
	BOOL	Sel_R_SDHys;
	BOOL	Sel_Calibrate; 
	BOOL	Sel_Calibrate_Test; 
	BOOL	Sel_APD_Aging;
	BOOL    Sel_T_Cross;
	int		PN_TYPE;    //SFF=0, SFP=1
	char	Ver_E2[10];
	int 	Ver_E2_Index; 
	int		RSSI_TYPE;	//RSSI_TYPE=1,第1代；RSSI_TYPE=2,第2代；具体区别请参考版本历史说明
	int		ProductLine;	//CD=0, phtop=1
	BOOL    Sel_EE_P;	//EEPROM 写保护检查
	int     RSSI_N1;
	int     RSSI_N3;
	int     RSSI_N2;  
	enum eRSSI_TIME  RSSI_TIME;
	enum eDriverChip DriverChip;
	enum eDriverChip DriverChip10G;    /***10G驱动芯片类型**Eric.Yao**20130813***/
	enum eAmplifierChip AmplifierChip;
	enum eChiefChip  ChiefChip; 
	//for apd
	int     RSSI_Trig_Level;  
	float   RapdtoGND;
	float   Rapd;
	float   Kh;
	float   Kl;
	float   Vapd_K1;
	float   Vapd_K2;
	float   Vbr_Class1;
	float   Vbr_Class2;
	int     APD_Protect;
	int     A0_Protect;
	int     ADC3_Po;
	int		isLosDACRefVcc_flag;  
	int		isLosWithoutCompesate_flag;

	float   Temper_K;
	float   Temper_B;
	float   Temper_S;  
	float   Temper_Kl;
	float   Temper_Bl;
	int     tx10G_unit;
	int     tx1G_unit;
	enum eCUSTOMER cus_type;
	enum eServerType servertype;
	char    laser_type[10];		//光谱仪参数
	double  wave_start;
	double  wave_stop;
	double  wave_center;
	double  wave_span;
	BOOL check_sn;
	char    firstsn[50];
	char    lastsn[50];
	char    batch[50];  
	BOOL isNPI_flag;		/***增加NPI测试标志位：0，不是NPI，1是NPI**Eric.Yao**20130819***/ 
	int		PN_CLASS;    	//I档 or C档
	int     APD_DAC_MIN;	//常温APD DAC最小值
	int     APD_DAC_MAX;	//常温APD DAC最大值
	int     APD_DAC_MIN_H;	//高温APD DAC最小值
	int     APD_DAC_MAX_H;	//高温APD DAC最大值
	int     APD_DAC_MIN_L;	//低温APD DAC最小值
	int     APD_DAC_MAX_L;	//低温APD DAC最大值
	float  Los_Kh;
	float  Los_Kl;

	float Temp_unit;
	float Vol_unit;
	float Bias_unit;
	float Tx_unit;
	float Rx_unit;
} my_struConfig;

//定义控制接口结构体
struct  struControl 
{
	//InitModule
	int		Init_I2cAddr;
	int		Init_PowerWord_Index;
	char	Init_GetVersion[50];
	
	//GetBosaSN
	int		GetSN_I2cAddr;
	int		GetSN_Table; 
	
	//GetTemper
	char	GetTemper_CaseCommd[50];
	char	GetTemper_CoreCommd[50];  
	int		GetTemper_IndexOffset;
	
	//TunAOP 1G
	char	TunAOP1G_MODE_M[50];
	char	TunAOP1G_MODE_A[50];
	char	TunAOP1G_DAC_SET[50];
	int		TunAOP1G_DAC_MAX;
	int		TunAOP1G_LUT_NAME_INDEX;
	int		TunAOP1G_LUK_INDEX;
	int		TunAOP1G_LUK_SLOPE_AMP;   
	int		TunAOP1G_LUK_OFFSET_AMP;   
	char	TunAOP1G_SAVE[50];
	
	//TunAOP 10G  
	char	TunAOP10G_MODE_M[50];
	char	TunAOP10G_MODE_A[50];
	char	TunAOP10G_DAC_SET[50];
	int		TunAOP10G_DAC_MAX;
	int		TunAOP10G_LUT_NAME_INDEX;
	int		TunAOP10G_LUK_INDEX;
	int		TunAOP10G_LUK_SLOPE_AMP;   
	int		TunAOP10G_LUK_OFFSET_AMP; 
	char	TunAOP10G_SAVE[50];
	
	//TunER 1G
	char	TunER1G_MODE_M[50];
	char	TunER1G_MODE_A[50];
	char	TunER1G_DAC_SET[50];
	int		TunER1G_DAC_MAX;
	int		TunER1G_LUT_NAME_INDEX;
	int		TunER1G_LUK_INDEX;
	int		TunER1G_LUK_SLOPE_AMP;   
	int		TunER1G_LUK_OFFSET_AMP; 
	char	TunER1G_SAVE[50];
	
	//TunER 10G
	char	TunER10G_MODE_M[50];
	char	TunER10G_MODE_A[50];
	char	TunER10G_DAC_SET[50];
	int		TunER10G_DAC_MAX;
	int		TunER10G_LUT_NAME_INDEX;
	int		TunER10G_LUK_INDEX;
	int		TunER10G_LUK_SLOPE_AMP;   
	int		TunER10G_LUK_OFFSET_AMP; 
	char	TunER10G_SAVE[50];
	
	//TunCross 1G
	char	TunCross1G_MODE_M[50];
	char	TunCross1G_MODE_A[50];
	char	TunCross1G_DAC_SET[50];
	int		TunCross1G_DAC_MAX;
	int		TunCross1G_LUT_NAME_INDEX;
	int		TunCross1G_LUK_INDEX;
	int		TunCross1G_LUK_SLOPE_AMP;   
	int		TunCross1G_LUK_OFFSET_AMP; 
	char	TunCross1G_SAVE[50];
	
	//TunCross 10G
	char	TunCross10G_MODE_M[50];
	char	TunCross10G_MODE_A[50];
	char	TunCross10G_DAC_SET[50];
	int		TunCross10G_DAC_MAX;
	int		TunCross10G_LUT_NAME_INDEX;
	int		TunCross10G_LUK_INDEX;
	int		TunCross10G_LUK_SLOPE_AMP;   
	int		TunCross10G_LUK_OFFSET_AMP; 
	char	TunCross10G_SAVE[50];
	
	//TxCal 1G
	char	TxCal1G_MODE_E[50];
	char	TxCal1G_MODE_F[50];
	char	TxCal1G_ADD_MIN[50];
	char	TxCal1G_ADD_MAX[50];  
	char	TxCal1G_ADC_GET[50];
	char	TxCal1G_SLOPE_ADD[50];
	int		TxCal1G_SLOPE_AMP;   
	int		TxCal1G_OFFSET_AMP; 
	
	//TxCal 10G
	char	TxCal10G_MODE_E[50];
	char	TxCal10G_MODE_F[50];
	char	TxCal10G_ADD_MIN[50];
	char	TxCal10G_ADD_MAX[50];  
	char	TxCal10G_ADC_GET[50];
	char	TxCal10G_SLOPE_ADD[50];
	int		TxCal10G_SLOPE_AMP;   
	int		TxCal10G_OFFSET_AMP; 
	
	//TunAPD
	char	TunAPD_MODE_M[50];
	char	TunAPD_MODE_A[50];
	char	TunAPD_DAC_SET[50];
	int		TunAPD_DAC_MAX;
	char	TunAPD_DAC_SET_RANGE[50];   
	int		TunAPD_LUT_NAME_INDEX;
	int		TunAPD_LUK_INDEX;
	int		TunAPD_LUK_SLOPE_AMP;   
	int		TunAPD_LUK_OFFSET_AMP; 
	char	TunAPD_SAVE[50];
	
	//TunSD
	char	TunSD_MODE_M[50];
	char	TunSD_MODE_A[50];
	char	TunSD_DAC_SET[50]; 
	int		TunSD_DAC_MAX;
	char	TunSD_FUN_OFF[50];
	char	TunSD_FUN_ON[50];
	int		TunSD_LUT_NAME_INDEX;
	int		TunSD_LUK_INDEX;
	int		TunSD_LUK_SLOPE_AMP;   
	int		TunSD_LUK_OFFSET_AMP; 
	char	TunSD_SAVE[50];
	
	//RxCal
	char	RxCal_MODE_F[50];
	char	RxCal_MODE_E[50];
	char	RxCal_MODE_T[50];  
	char	RxCal_ADC_GET[50];
	int		RxCal_LUT_NAME_INDEX;
	int		RxCal_LUK_INDEX;
	int		RxCal_LUK_SLOPE_AMP;   
	int		RxCal_LUK_OFFSET_AMP; 
	char	RxCal_SAVE[50];
	char	RxCal_SET_KH[50];
	char	RxCal_SET_KL[50];
	int		RxCal_RSSI_K_AMP;
	char	RxCal_SET_TEMP_PIONT[50];
}my_struControl;

//OSA 波长测试
struct struDBConfig_OSATEST	  
{
 char	DAC_APC[50];
 char	DAC_MOD[50];
 char   WAVELENTH_RANGE[50];
 char   str_WaveRecorder[200];
 float  WaveLenth_Max;
 float  WaveLenth_Min;
 float Tec_Temper_Max;
 float Tec_Temper_Min;
 float Temper_Min;
 float Temper_Max;
} struDBConfig_OSATEST; 


//定义设备配置结构体
struct  struInstrument  
{
	//测试通道配置
	int     Channel;
	int     ChannelEn;
	int 	ChannelStart;
	int     ChannelEnd;
	
	//for power meter
	int 	Power_TYPE; //1=HP8153A; 0=OLP18C, 2=MT9810; -1=no power  
	int 	Power_CHAN;  
	int		OLP18C; 
	int		PMSII;
    char    SN_PM[30];
	
	//for DCA
	int 	DCA_TYPE;  	//0=CSA8000, 1=Ag86100, -1=no BERT    
	char	DCA_Addr[30]; 
	char    SN_DCA[30];
	
	//for bert
	int 	BERT_TYPE;  //0=GBERT, 1=Ag86130, -1=no BERT  
	char    SN_BERT[30]; 
	
	//for 10g bert
	int 	BERT_TYPE_10G;  //0=GBERT, 1=Ag86130, -1=no BERT  
	char	SN_10GBERT[30];

	//for att
	int 	ATT_TYPE_MAIN;  	//0=8156, 1=OLA55M, 2=8157, -1=no att 3=FVA3100 
	char    SN_ATT_MAIN[30]; 
	int     ATT_MAIN_COM;   
	
	//for att REF
	int 	ATT_REF_TYPE;  	//0=8156, 1=OLA55M, 2=8157, -1=no att 3=FVA3100 
	char    SN_ATT_REF[30]; 
	int     ATT_REF_COM; 
	
	//for 86142
	int     SPECTRUM_TYPE;
	char    Spectrum_Addr[30];
	char	SN_Spectrum[30];

	//for evb
	int 	SEVB_TYPE;	//1=SEVB1, 2=SEVB2...
	char	SEVB[30]; 
	
	//for light
	int		EVB_Light_TYPE;
	char	EVB_Light[30]; 
	
	//for fiber
	char	Fiber[30];   

	//光开关
	int 	SW_TYPE;   //0=FSW, -1=不选光开关    
	int     SW;		   //光开关端口
	int     SW_CHAN;   //光开关使用通道
	char    SW_SN[30]; //光开关序列号，10G光开关使用
	int		SW_Handle;
	
	//时钟切换
	int 	CLOCK_TYPE;//0=SEVB0027-4001, -1=不选时钟切换
	char	CLOCK[30]; //时钟切换板
	int     CLOCK_CHAN;//时钟切换通道
	
} my_struInstrument[CHANNEL_MAX];


//定义发端校准参数结构体

struct  struTxCal   my_struTxCal;
//struct  struRxCal   my_struRxCal;
struct struCSA8000  my_struCSA8000;

struct struRxCal sRxCal_Lock;	//主光路锁定模式校准变量
struct struRxCal sRxCal_Free;	//主光路衰减模式校准变量
struct struRxCal sRxCal_Lock_TDP;	//主光路锁定模式校准变量
struct struRxCal sRxCal_Free_TDP;	//主光路衰减模式校准变量
struct struRxCal sRxCal_Ref;	//参考光路衰减模式校准变量 
struct struTxCal my_struTxCal;		//发端光路校准结构
struct struTxCheck sTxCheck;		//发端光路点检结构体

//定义调测结果结构体 
struct  struTestData	  
{
	char	PN[50];				   	//PN
	char    Ver[10];			   	//产品版本
	char	TestDate[50];          	//日期
	char	ModuleSN[50];		   	//模块的序列号
	char	OpticsSN[50];		   	//BOSA的序列号 或 innersn
	float   Temperatrue;			//模块温度
	double  AmbientTemperatrue;     //环境温度
	char	TestFlag[50];			//测试工序名
	int 	ErrorCode; 				//错误代码
	char	Status[10];				//测试结果PASS FAIL   
	float   AOP;				   	//光功率
	double  ER;					   	//消光比
	double	EA;	   
	double  TxPPj;
	double	TxRSj;
	double  TxTj;
	double 	TxRise;
	double	TxFall;	  
	double  MaskMargin;  
	double  TxCross;    
	float   TxOffPower;				//TxDis光功率  	
	
	//Rx
	float   RxAmp;					//电口幅度
	double  RxTj;					//电口抖动
	double  RxPPj;
	double  RxTr;
	double  RxTf;
	double  Over;					//过载测试值
	double  OverTime;
	double  Sens; 					//灵敏度测试值 
	double  SensTime;
	double  SDA;
	double  SDD; 
	double  SDHys; 	
	int		RsdStep;
	double  TxPCTCROss;
	
	//TDP
	double  Pathpenalty;			//通道代价
	double  PathpenaltyTime;		//通道代价时间 	
	
	//光谱数据
	double	PeakWL;
	double	Sigma;
	double	Bandwidth;
	double  SMSR; 
					
	double	TxI;
	double  RxI;
	double  SE;					 
	double	TE;  
	
	//DAC
	int 	DACApc;					//APC DAC SET
	int 	DACMod;					//MOD DAC SET 
	int 	DACCross;				//Cross DAC SET
	int		DACApd;					//APD DAC SET
	int		DACLos;					//LOS DAC SET

	double	CoreTempera_Apc;
	double	CoreTempera_Mod;           
	double	CoreTempera_Cross;           
	double	CoreTempera_Apd;           
	double	CoreTempera_Los;
	
	//发端校准
	double	TxCalPower0;			//发端校准功率0
	double	TxCalPower1;			//发端校准功率1
	int		TxCalAdc0;				//发端校准ADC0
	int		TxCalAdc1;				//发端校准ADC1
	
	char	Info_APC[4000];
	char	Info_MOD[4000];  
	char	Info_CROSS[4000];  
	char	Info_APD[4000];  
	char	Info_LOS[4000]; 
	
	//监控
	double	A2_Temper;				//监控温度
	double	A2_Vcc;					//监控电压
	double	A2_Ibias;				//监控Bias电流
	double  A2_TxMon;				//监控Tx Power
	double	CaseTemper;				//记录实测壳温

	double	RSSI_Compensation;		//RSSI 温度补偿系数


	//校准测试数据 未使用
	//double  TxPow_meter;			//功率计测试数据
	//double  TxPow_mon;			//监控数据
	//double  RxPow;
	int     RxPow_num;
	double	RxPow_mon[30];

	int     FiberTime; 
	int     ProjectNumber;			/***固件项目号**Eric.Yao**20131012***/
	char 	g_buf1[128];
	char 	g_buf2[128];

	char    Comments[4000];  

	char    curve[150000];		//大小可能不确定，定义一个默认的值
	int     curvesize;
	
	BOOL	AOP_CheckFlag;		//调试ER后功率检查是否合格标志，不合格==1；合格==0；
	int 	ucRsd;					//Rsd的数字电位器设置 
		double	EaAbsorb;				//Ea吸收功率 
		int DACEa;
		char Info_EA[4000];
		double CoreTemper;
		double TecCurrent;
		double TecTemper;
		
  
	
};
int m_WLDac_normal;             // 常温DAC和核温
float m_WLTemper_normal;
int m_Value_K;
int m_Value_B;
int m_WLDac_High;               //高温DAC和核温
float m_WLTemper_High;
//定义调测参数结构体 
struct  struDBConfig	  
{
	char	PartNumber[50];   //字符串的大小必须和DBMapColumnToChar函数定义的大小一致
	char	Version[50];	  //字符串的大小必须和DBMapColumnToChar函数定义的大小一致 
	char	DT_Flag[50];	  //字符串的大小必须和DBMapColumnToChar函数定义的大小一致  
	char    Temper_Flag[50];  //字符串的大小必须和DBMapColumnToChar函数定义的大小一致 
	char    BarCode[50];
	char    Pro_Type[50];

	int     SNLength;  
	char    FirmwareVer[50]; 
	char 	SFRM_Number[50];	/***增加项目号**Eric.Yao***/
	char 	FEP_Number[50];		/***增加FEP号**Eric.Yao***/
	double  TemperMin;
	double 	TemperMax;
	double  TxPowMin;
	double  TxPowMax;
	double  TxBiasMin;
	double  TxBiasMax;
	double  TxErMin;
	double  TxErMax;
	double  TxCrossMin;
	double  TxCrossMax;

	double 	TxTjMax;
	double	TxPPjMax;
	double 	TxRiseMax;
	double	TxFallMax;
	double	TxDisPowMax;
	double	TrackingErrorMax;
	
	double	TxMargin; 
	char 	MaskName[50];
	double	PathPenaltyMax;
	double	PathPenaltyTime;
	//收端参数
	double  Over;			//过载测试值
	double	OverTime;
	double  Sens; 			//灵敏度测试值 
	double	SensTime;
	double  Sens_Center; 	//中心灵敏度测试值 
	double	SensTime_Center;
	double  SDA;
	double  SDD; 
	double  SDHysMin;
	double  SDHysMax;
	
	//光谱参数
	double	PeakWavelengthMin;
	double	PeakWavelengthMax;	
	double	PeakWavelengthMin01;
	double	PeakWavelengthMax01;
	double	PeakWavelengthMin02;
	double	PeakWavelengthMax02;
	double	PeakWavelengthMin03;
	double	PeakWavelengthMax03;
	double	PeakWavelengthMin04;
	double	PeakWavelengthMax04;
	double	Sigma;
	double	BandWidth;
	double	SMSR;
	//电压电流
	double	TxVMax;
	double	TxIMax;
	double	RxVMax;
	double	RxIMax;
	char	LTemp_Targer[50];
	char	EAVol_Target[50]; 

	double  APD_Kl;
	double  APD_Kh;
	BOOL    isSDorLOS;     /***SD、LOS标志位 = 1：SD； = 0：LOS**Eric.Yao**20130713***/
	float  TxCalMin;				   
	float  TxCalMax;
	int    TxWavelength;
	int    RxWavelength;
} my_struDBConfig;

//定义监控参数结构体 
struct  struDBConfig_Monitor	  
{
	float	Temper_Prec;
	float 	Vcc_Prec;
	float	I_Bias_Prec;
	float	Tx_Pow_Prec;
	float	Rx_Pow_Prec;
	int		CaliNumber;
	double	CaliPoint[30];
	int		CaliNumber_Test;
	double	CaliPoint_Test[30];
} my_struDBConfig_Monitor;

//定义EEPROM结构体 
struct  struDBConfig_E2	  
{
char	PN[50];   	//字符串的大小必须和DBMapColumnToChar函数定义的大小一致
char	Ver[10];	//字符串的大小必须和DBMapColumnToChar函数定义的大小一致 
int		E2_Flag;  	//A0,A2标志 0：A0[0-127], 1:A2[0-127]
char    E2_str[128][5];
int		A0[128];  
int		A0_Lower[128];
int		A0_Table0[128]; 
int		A0_Table1[128];
int		A0_Table2[128];

int		A2[128];   
int		A2_Table0[128]; 
int		A2_Table1[128];  

char	table_flag[50];
} my_struDBConfig_E2;

int m_adjustDac[2];
double m_adjustTemper[2];
BOOL m_adjustSpeed;
//定义消光比补偿结构体 
struct  struDBConfig_ERCompens	  
{
	char 	DriverChip[50];
	char	ERCompensChip[50];
	char    AmplifierChip[50]; 
	double	Rate;
	char	Pro_type[50];
	char	MCU_CMD_TYPE[50];
	int		A0_PROTECT;
	
	int		RSSIMODE;
	int		RSSI_N1;
	int		RSSI_N2;
	int		RSSI_N3;
	int		RSSI_TRIG_LEVEL;
	enum eRSSI_TIME	RSSI_TIME;
	char	LTEMP_TARGET[50];
	char	EAVOL_TARGET[50];	
	
	//Tempreature Cal
	double	A2TEMPER_S;
	double	A2TEMPER_KL;
	double	A2TEMPER_BL;
	double	A2TEMPER_KH;
	double	A2TEMPER_BH;
	
	//1G APC MOD DAC调试范围
	char	RB_ADD_1G[50];
	char	RM_ADD_1G[50];
	
	//1G AOP补偿系数 
	double	IB030_10_1G;
	double	IB10_33_1G;
	double	IB33_60_1G;
	double	IB60_82_1G;	
	double	IB82_105_1G;
	
	//1G ER补偿系数 
	double	IM030_10_1G;	
	double	IM10_33_1G;	
	double	IM33_60_1G;	
	double	IM60_82_1G;	
	double	IM82_105_1G;	
	  	
	//1G VAPD DAC三温度的范围；
	int		APDDAC_MIN_L_1G; 
	int		APDDAC_MAX_L_1G;
	int		APDDAC_MIN_R_1G; 
	int		APDDAC_MAX_R_1G;
	int		APDDAC_MIN_H_1G; 
	int		APDDAC_MAX_H_1G;
	
	//1G Vapd温度补偿系数
	double	VAPD_KL_1G;
	double	VAPD_KH_1G;
	double	VBR_CLASS_1G;
	
	//1G RSSI补偿系数
	double	RSSI_KL_1G;
	double	RSSI_KH_1G;
	
	//1G LOS  系数
	int		ISLOSORSD_1G;		//LOS Or SD 标志；
	double	LOS_KL_1G;
	double	LOS_BL_1G;
	double	LOS_KH_1G;
	double	LOS_BH_1G;
	double	LOS_TEMPER_S_1G;
	
	//10G APC MOD DAC调试范围
	char	RB_ADD_10G[50];
	char	RM_ADD_10G[50];
	
	//1G AOP补偿系数 
	double	IB030_10_10G;
	double	IB10_33_10G;
	double	IB33_60_10G;
	double	IB60_82_10G;	
	double	IB82_105_10G;
	
	//10G ER补偿系数 
	double	IM030_10_10G;	
	double	IM10_33_10G;	
	double	IM33_60_10G;	
	double	IM60_82_10G;	
	double	IM82_105_10G;	
	  	
	//10G VAPD DAC三温度的范围；
	int		APDDAC_MIN_L_10G; 
	int		APDDAC_MAX_L_10G;
	int		APDDAC_MIN_R_10G; 
	int		APDDAC_MAX_R_10G;
	int		APDDAC_MIN_H_10G; 
	int		APDDAC_MAX_H_10G;
	
	//10G Vapd温度补偿系数
	double	VAPD_KL_10G;
	double	VAPD_KH_10G;
	double	VBR_CLASS_10G;
	
	//10G RSSI补偿系数
	double	RSSI_KL_10G;
	double	RSSI_KH_10G;
	
	//10G LOS  系数
	int		ISLOSORSD_10G;		//LOS Or SD 标志；
	double	LOS_KL_10G;
	double	LOS_BL_10G;
	double	LOS_KH_10G;
	double	LOS_BH_10G;
	double	LOS_TEMPER_S_10G;

} my_struDBConfig_ERCompens;

enum LUT_MODE
{  
	LUT_MODE_MANUAL,
	LUT_MODE_AUTO,
	LUT_MODE_TABLE,
	LUT_MODE_EXT,
	LUT_MODE_FORMULA
};

enum SD_FUN
{  
	SD_FUN_OFF,
	SD_FUN_ON
};


char	g_ConfigFileName[MAX_PATHNAME_LEN]; //for config file
char	g_InstFileName[MAX_PATHNAME_LEN];   //for Instrument Add file  
char	g_CtrFileName[MAX_PATHNAME_LEN];    //for Control Command file    

struct struTxCheck 	sTxCheck;		//发端光路点检结构体  

/*******************ATE常用函数**********************************/
extern BOOL GetConfig(void);
extern BOOL SetConfig(void);

extern BOOL GetControl(void);
extern BOOL SetControl(void);

//------------------数据库函数-------------------------------//
void ShowDataBaseError(void) ;
extern BOOL DB_Initial (void);		         	//初始化数据库
extern BOOL DB_GET_Spec_BarCode_Ver (char Ver[][10]); 	//根据PN读取数据库的Version
extern BOOL DB_GET_EED_Ver (char Ver[][10]); 
extern BOOL DB_GET_ConfigInfo (void); 			//读取调测spec 和测试时的EEPROM

extern BOOL DB_GET_ProType(void);
extern BOOL DB_GET_EEPROM (void);				//读取EEPROM 
extern BOOL DB_GET_EEPROM_Ex(void);
extern BOOL DB_GET_BarCode (void);				//读取条码barcode
extern int  DB_Save_Results_ATE_New(struct struTestData data, struct struProcessLOG prolog);					//保存调测结果和生产过程标志
extern int DB_Save_Results_10G (struct struTestData data);
extern BOOL DB_Save_Process_LOG (void);			//保存生产过程标志
extern BOOL DB_GET_Firmware_Ver (void);			//获取Firmware版本信息
extern BOOL DB_GET_SPEC_CSA8000 (void);
extern BOOL DB_GET_SPEC_Ag86100 (void); 
extern BOOL DB_GET_PN_TYPE(char *PN_TYPE);  

extern BOOL DB_Search_Optics_Data_Vbr(char *OpticsSN, double *Vbr);  //查询BOSA数据中的APD_T_Vbr, 用于函数tuningVapd_SFP，由于之前已经调用函数DB_Seach_Optics_Data，保证了APD_D_Flag=1，所以此函数查询条件没有添加条件APD_D_Flag=1
//extern BOOL DB_Update_Optics_Data(void);   
extern int DB_READ_Spec_Monitor_Ex (int Flag);	
extern int DB_Search_Process_LOG_Ex(struct struTestData data, struct struProcessLOG prolog); 
extern int DB_READ_AutoDT_Spec_ATE_Item (void);
extern int DB_READ_Spec_Tracking_Ex (int *RSSIMode);	//读取RSSIMode  
extern int DB_Save_Results_Monitor (struct struTestData data);				//保存校准测试结果
extern int DB_Search_Tracking_BareBOM(char *SN, char *bom, char batch[]);	//检测BOSASN在Tracking表的BareBOM记录 
extern int DB_GET_Config_Monitor_Info (void);  	//读取监控校准配置
BOOL DB_GET_Spectrum(void); 
int DB_save_rxcali(int channel, enum enum_CALIBRATION_TYPE cali_type);
int DB_save_txcali(int channel);
int DB_get_txchecklimit(void);
int DB_save_txcheckup(char sn[], double val);
int DB_Read_Spec_TxCalThreshold (void);
int DB_Read_Spec_1GTxCal (void); 
int DB_Save_Results_10G_Ex (struct struTestData data, struct struProcessLOG prolog);
int DB_GET_EA(char *OpticsSN, float *ea);				//读取数据库Trisa EA值  
int DB_Get_BOM(int panel, int control); 
int DB_get_txcali(enum enum_CALIBRATION_TYPE cali_type);
int DB_get_cali(enum enum_CALIBRATION_TYPE cali_type, int channel, BOOL *timeOutFlag);
int DB_Get_Handle (void);
int DB_Save_Results_Image_for_Oracle(struct struTestData data, struct struProcessLOG prolog);
int DB_Close (void);						//关闭数据库		
int DB_Read_pch (char pn[], char bom[], int panel, int control_pch);
int DB_Read_Order (char order[]); 
//------------------数据库函数-------------------------------//  

extern void GetPartNo(int panel, int control);  		//从文件读取PN显示到对应的界面上
extern void GetPNVersion(int panel, int control_Ver); 	//根据PN及其他信息读取相应的产品版本，显示到界面上
extern void Get_EED_Ver(int panel, int control); 		//读取EED文件版本
/*******************结束 ATE常用函数*****************************/

/*******************仪器配置读写*****************************/ 
extern BOOL GetConfig_Inst(void);
extern BOOL SetConfig_Inst(void);
/*******************仪器配置读写*****************************/ 

/*******************仪器控制函数*****************************/ 
int Set_Att(int channel, int AttType, double AttValue);
int Set_Att_Lock (int channel, int AttType, double AttValue);
int Set_Att_Lock_Enable (int channel);
int Set_Att_Lock_Disable (int channel);
//2009-03-12 add by roger 
extern int SET_EVB_SHDN(int channel, unsigned char SHUTValue);
extern BOOL SET_EVB_TxDis(int channel, unsigned char TxDis); 
extern BOOL Set_Power_Wavelength(int channel, int Wavelength);  
extern BOOL GET_Power(int channel, double *Power);
extern BOOL GET_Er(int DCAType, double *Er); 
extern BOOL GET_Cross(int DCAType, double *Cross);
extern BOOL Set_DCA_Clear(int DCAType);  //示波器清屏
extern BOOL READ_DCA_PJ(int DCAType, double *PJ);  
extern BOOL READ_DCA_RJ(int DCAType, double *RJ); 
extern BOOL READ_DCA_Rise(int DCAType, double *Rise);
extern BOOL READ_DCA_Fall(int DCAType, double *Fall); 
extern BOOL READ_DCA_PCTCROss(int DCAType, double *PCTCROss);   
extern BOOL GET_MaskHits(int DCAType, int WaveForms, double *MaskMargin); 
extern BOOL GET10G_MaskHits(int DCAType, int WaveForms, double *MaskMargin);
/*******************结束 仪器控制函数************************/ 

/*******************拟和部分函数**********************************/
extern BOOL fit_LUT_Line(BYTE fitNumber, int *Temper_in_arr, BYTE *LUT_in_arr, int TemperLimit_H, int TemperLimit_L, int *Temper_out_arr, BYTE *LUT_out_arr);	 //温度分度值2
extern BOOL fit_LUT_Line_SFP(BYTE fitNumber, int *Temper_in_arr, INT16U *LUT_in_arr, int TemperLimit_H, int TemperLimit_L, int *Temper_out_arr, INT16U *LUT_out_arr);   //温度分度值1
//******************结束 拟和部分函数*****************************/

extern BOOL checksn(char *s);
extern int SET_EVB_Light_MODE_RSSI(int channel);		//设置光源RSSI模式参数
extern int SET_EVB_Light_BEN_Trig_Gen(int channel);  //设置光源RSSI模式触发
extern int SET_EVB_Light_MODE_Level(int channel);  	//设置光源常规模式

extern int  Initial(int panel, int panMain, int *error_channel, int ini_Flag);        //初始化函数
extern int	Init_BERT (int channel, int mode_B2BERT);
extern int	Init_10GBERT (int channel); 
extern int	Init_10GBERT_Start (int channel);

extern BOOL InitModule(int channel, struct struTestData *data, struct struProcessLOG *prolog);		//配置模块进入调测状态
extern BOOL testTemperature(int panel, int channel, struct struTestData *data); 	//获取调测温度

BOOL tuningAOP10G(int channel, struct struTestData *data);    //10G 光功率调试函数 
BOOL testDWDMAOP10G(int channel, struct struTestData *data);
BOOL tuningAOP10G_R(int channel, struct struTestData *data);    //调试ER后，功率不合格，重调10G 光功率调试函数  
BOOL tuningER10G(int panel, int channel, struct struTestData *data);     //10G 消光比	 
BOOL tuningRE10G_DWDM(int panel, int channel, struct struTestData *data); // DWDM 调试10邋G消光比

extern BOOL tuningSD(int channel, struct struTestData *data);	   		//SD调试函数
extern BOOL tuningSD2(int channel, struct struTestData *data);					   //实际为测试SD,未调试
extern BOOL testAOP10G(int channel, struct struTestData *data);	   		//10G光功率测试函数
extern BOOL testER10G(int channel, struct struTestData *data);		   	//10G消光比测试函数

extern BOOL testFixSens_1G(int channel, struct struTestData *data);
extern BOOL testFixSens_Start_1G(int channel, struct struTestData *data, int *StartTime);  //配置灵敏度测试
extern BOOL testFixSens_End_1G(int channel, struct struTestData *data, int StartTime); 	//检查灵敏度测试结果
extern int	testDetailSens_1G (int channel, struct struTestData *data);
extern int	testDetailSens_10G (int channel, struct struTestData *data);
extern BOOL testFixOver_1G(int channel, struct struTestData *data);
extern BOOL testFixOver_10G(int channel, struct struTestData *data);					//测试10G过载
extern BOOL testDetailOver(void);
extern BOOL testFixSD(int channel, struct struTestData *data);
extern BOOL testDetailSD(void);
extern BOOL testVoltage(void);
extern BOOL testSpectrum(int channel, struct struTestData *data);
extern BOOL testSpectrum1(int channel, struct struTestData *data);
extern BOOL CheckModuleSN(int panel, int channel, struct struTestData *data);
extern BOOL calTemper(void);
extern BOOL calVcc(int channel);
extern BOOL calIbias(void);

extern BOOL quittest(void);
extern BOOL SaveDataBase(struct struTestData data, struct struProcessLOG prolog);
extern BOOL GetBosaSN(int channel, struct struTestData *data); 
extern BOOL testSD(int channel);
extern BOOL testCurrent(int channel, struct struTestData *data);
extern BOOL testOEye(int channel, struct struTestData *data); 
extern BOOL testOEye_Mask(int channel, struct struTestData *data);
extern BOOL testRapd(int channel); 
extern BOOL resetdata(int channel, struct struTestData *data, struct struProcessLOG *prolog);			//初始化测试结果变量
extern BOOL teststation(struct struTestData data, struct struProcessLOG prolog); 
extern void readconfigfilename(void);
extern int Set_APDprotect(void);
extern int testTxDis_1G(int channel, struct struTestData *data); 
extern int testTxDis_10G(int channel, struct struTestData *data);      

/******************读取测试参数部分函数***********************************/ 
extern BOOL Read_AOP(int channel, float *AOP); 
extern BOOL Read_ER(int channel, double *ER);
extern BOOL Read_Cross(int channel, double *Cross);
extern BOOL Read_Temperature(int channel, float *Temperatrue);
extern BOOL Read_RX_SD(int channel, const int SD_Target, BYTE *SD);
extern BOOL Read_RX_LOS(int channel, const int LOS_Target, BYTE *LOS);
extern BOOL Read_Tx_Fail(int channel, BYTE *Fail); 
extern BOOL Read_Error_Bit(int channel, unsigned int *ErrorCount, int TestFlag, double testtime);	//添加测试标志位，表示是读取开始测试状态，还是读取最终状态， 0：开始状态； 1： 最终状态  
//extern BOOL Read_Error_Bit_Ex(int channel, unsigned int *ErrorCount, int TestFlag);  				//与Read_Error_Bit区别在于当读取到误码时，不弹出对话框提示  状态标志， 0：开始状态； 1： 最终状态 2：SDD状态下读取误码
//extern BOOL Read_Error_Bit_Ex1(int channel, unsigned int *ErrorCount, double *ERR_Ratio); 
extern BOOL Read_Bert_ERR_Ratio_1G(int channel, unsigned int *reErroCode,double *ERR_Ratio);
extern BOOL Read_Bert_ERR_Ratio_10G(int channel, unsigned int *reErroCode,double *ERR_Ratio); 
/******************结束 读取测试参数部分函数******************************/ 

//20081210 add by roger 
extern BOOL calTxPower10G(int channel, struct struTestData *data);
extern BOOL calTxPower10G_Ex(int channel, struct struTestData *data);
extern BOOL caltestTxPower10G(int channel, struct struTestData *data);

extern BOOL cal_RxPower10G(int panel, int channel, struct struTestData *data);
extern BOOL caltest_RxPower10G(int panel, int channel, struct struTestData *data);

extern int short_circuit_check (int channel, struct struTestData *data);  //短路检查函数  

extern int Read_BER(int channel, double GatingTime, double *ErrorRate,int readtype);	  //readtype为1 判断为测误码率
extern int BERT_Start (int channel, double time_test, int isTestErrbitFlag, int B2_BurstMode);
extern int BERT_Stop (int channel);
extern int BERT_Read(int channel, unsigned int *ErrorCount, int TestFlag);
extern int get_module_sn (int channel, struct struTestData *data);
extern int ReadModuleSN (int channel, struct struTestData *data); 
extern int tuningAPD_BestSen_1G (int channel, struct struTestData *data); //调试1G APD 最好灵敏
extern int tuningAPD_BestSen_10G (int channel, struct struTestData *data); //调试10G APD 最好灵敏

extern int testFixSens_apd_aging (void);
extern BOOL SaveEEPROM(int channel, struct struTestData data);

extern int Set_Att_Ex (int channel, int RSSI_Mode, double RxP);	//设置RX校准功能扩展函数，包含RSSI光源模式切换等功能
extern int BERT_Start_SDD (int channel, double time_test);		//SDD状态下测试误码状态 

extern int BERT_10G_Start (int channel, double time_test);
extern BOOL testFixSens_10G(int channel, struct struTestData *data);
extern BOOL Read_Error_Bit_10G(int channel, unsigned long long *ErrorCount, double *ErrorRate, double testtime);

BOOL tunSpectrum_WL_DWDM(int channel, struct struTestData *data, int *tekVlue);
BOOL tunSpectrum_WL_High_DWDM(int channel, struct struTestData *data, int tekVlue);


int set_los_lut (INT16U los_DAC);

int analize_1G_10G(const char *info, char *info_1G, char *info_10G);

void readconfigfilename(void);

//校准函数
int GetCaliConfigFile(void); 	//更新光路校准配置文件
int SetCaliConfigFile(void);	//读取光路校准配置文件	  

int Set_LOS_DAC (int handle, const INT16U DAC);

int display_FSW(int panel, int fsw_type);   
void Update_Config_Ver(int panel, BOOL updateVer);

void InsertTree(int panel, int control, struct struInstrument *localInst, int collapsed);
BOOL testTemper(int panel, int channel, struct struTestData *data);
BOOL testER10G(int channel, struct struTestData *data);

BOOL testOEye1G_Mask(int channel, struct struTestData *data);
BOOL testOEye10G_Mask(int channel, struct struTestData *data);
BOOL tuningCross_10G(int channel, struct struTestData *data);
BOOL testCross(int channel, struct struTestData *data);

/***光开关控制部分**Eric.Yao**20131223***/
int Fiber_SW_Init(int FSW_TYPE, int COMIndex, int *FSW_10G_Handle, char FSW_SN[30]);
int Fiber_SW_Close(int FSW_TYPE, int COMIndex, int FSW_10G_Handle);
int Fiber_SW_SetChannel(int FSW_TYPE, int COMIndex, int FSW_10G_Handle, int channel);
int set10GSwitch(int FSW_10G_Handle, int channel);
/***光开关控制部分**Eric.Yao**20131223***/

int checktestbed(int channel, int *val);

void Display_PFY(char *batch, char *PN, char *StationID, char *Log_Action, int panel, int control_pass, int control_fail, int control_total, int control_passrate);

int XFP_Get_Mon_Temperature(int EVB5Handle,double *Temp);

int XGPON_OLT_SET_BIAS_Unit_Ex(int handle,int unit);                                             //设置Bias单位    
BOOL DB_GET_DDMI_Unit(void);  
// SFP 80KM
BOOL caltestRxPower_Ex(int panel, int channel, struct struTestData *data);
BOOL testTheSens(int channel, struct struTestData *data);
BOOL testTheLoadSens(int channel, struct struTestData *data);  //带纤灵敏度
BOOL BERT_Start1(int channel, int BERTType, int settime);
int Read_BER1(int channel, int BERTType, double GatingTime, double *ErrorRate,int readtype);
BOOL XGSPON_OLT_SET_RXP_MON_E(int handle);
BOOL XGSPON_OLT_SET_RXP_MON_F(int handle, INT16U wSlop, INT16U wOffset);
BOOL XGSPON_OLT_MCU_GET_RxPOW(int handle, int *data) ;
BOOL XGSPON_OLT_MCU_Update_BASE0(int handle);
BOOL tuningSD(int channel, struct struTestData *data);				  //调SDA,测SDD
BOOL testRxLOS(int channel, struct struTestData *data) ;
BOOL XGSPON_OLT_GET_A2_110_BYTE(int handle, int *data);
BOOL XGPON_OLT_SET_LOS_Ex(int handle,int data);
BOOL XGSPON_OLT_GN2010_SAVE_FLASH(int handle) ;
BOOL XGSPON_OLT_MCU_Update_Driver0(int handle) ;
BOOL tuningSD1(int channel, struct struTestData *data);
BOOL caltestTxPower(int channel, struct struTestData *data);
BOOL caltestRxPower_Ex(int panel, int channel, struct struTestData *data);
BOOL WriteDateTables(int panSearchData_chan0,struct struTestData data);
BOOL ReadNormalTemper(int handle, struct struTestData *data);
int TestWL_TemperHigh(int channel, struct struTestData *data, int panel, int control, char *Info, char *ERInfo);
BOOL GetDBNormalCoreTemper(double *CoreTemper, char *opticssn);
int SaveFlash_Dwdm(int channel);
int Get_normal_DAC_DWDM(int channel);
int DacAdjustSpeed(double targetWL);
BOOL GetTemper_DWDM(short *temperVlue, int channel);

// ==============================================Supermast 命令配置方式的相关函数=========================================================


#define LUT_BYTE_COUNT			6		  //每段查找表使用字节数 

int ENTRY_Password (int handle);
int GET_FirmVersion (int handle,char *str);
int GET_CORE_Temper(int handle, float *CoreTemper);
int GET_CASE_Temper(int handle, float *Temper, float Unit);

int SET_1G_APC_Mode (int handle, enum LUT_MODE Mode);	//设置1G  APC 模式
int SET_10G_APC_Mode (int handle, enum LUT_MODE Mode);	//设置10G APC 模式  
int SET_1G_APC(int handle, unsigned short SET_APC);		//设置1G APC值  	
int SET_10G_APC(int handle, unsigned short SET_APC);	//设置10G APC值 
int SET_10G_DWDM(int handle, unsigned short SET_Tec);	//设置DWDM TEC值 
int SET_10G_DWDM_EA(int handle, unsigned short SET_Ea);	//设置DWDM EA值
int SET_10G_APC_OverShooting(int handle, unsigned short SET_APC);	//设置10G APC值过冲  
int GET_10G_APC(int handle,int *data);

int SET_1G_MOD_Mode (int handle, enum LUT_MODE Mode);	//设置1G  APC 模式
int SET_10G_MOD_Mode (int handle, enum LUT_MODE Mode);	//设置10G APC 模式  
int SET_1G_MOD(int handle, unsigned short SET_MOD);		//设置1G APC值  	
int SET_10G_MOD(int handle, unsigned short SET_MOD);	//设置1G APC值 
int SET_10G_DWDM_ER(int handle, unsigned short SET_MOD); //
int SET_10G_DWDM_Cross(int handle);

int SET_10g_DWDM_MOD(int handle, unsigned short SET_MOD);   // 设置DWDM的光功率

int GET_TEMPE_LUT(int handle, char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, int *Temper, float *Slope, float *Offset);	//读取查找表 
int SET_TEMPE_LUT(int handle, char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper);	//写查找表
int SET_TEMPE_LUT_High(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper);	//写查找表 High  
int SET_TEMPE_LUT_Low(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper);	//写查找表 Low  
int Update_LUK(int handle, char *LukName, int tableIndex);			           //更新表LUK

//APD LUK
int GET_TEMPE_APD_LUT(int handle, char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, int *Temper, float *Slope, float *Offset);	//读取查找表 
int SET_TEMPE_APD_LUT(int handle, char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper);	//写查找表
int SET_TEMPE_APD_LUT_High(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper);	//写查找表 High  
int SET_TEMPE_APD_LUT_Low(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper);	//写查找表 Low  

int GET_10G_Bias(int handle, float *Bias, float Unit);		//获取10G Bias  

int Save_Flash(int handle ,char *SaveCMD);
int Update_BASE(int handle, int tableIndex); 			           //更新表BASE  

//TxCal 10G
int SET_10G_TxMon(int handle, float MON_L, float MON_H, float Unit);  								//设置TxMon 门限  
int SET_10G_TXPOW_CAL(int handle, unsigned short *ADC_Array, float *PWR_Array, float Unit);			//写 Tx校准数据    
int	GET_10G_TxPOWER_ADC(int handle, unsigned short *ADC);   										//读取XGPON TX ADC   
int GET_10G_TxMon(int handle, float *TxMon, float Unit);											//计算 TxMon   

int Set_1G_TX_ON(int handle);                   //打开1G 
int Set_1G_TX_OFF(int handle);                   //关闭1G 
int Set_10G_TX_ON(int handle);                   //打开10G 
int Set_10G_TX_OFF(int handle);                   //关闭10G 

/*************************************** APD************************************/
int SET_10G_APD_Mode (int handle, enum LUT_MODE Mode);	//设置APD模式
int GET_10G_APD(int handle, unsigned short* GET_APD);	//读取APD值
int SET_10G_APD(int handle, unsigned short SET_APD);	//设置APD值
int SET_10G_APD_Range(int handle);	//设置APD最大最小值	

//RSSI
int SET_10G_RSSI_Compensation(int handle, float Kh, float Kl); 				          		//写 Rx的高低温校准参数Kh和Kl
int SET_10G_RXKL(int handle, float Kl);				                                  		//写 Rx的低温校准参数和Kl  
int SET_10G_RXKH(int handle, float Kh); 				                                  	//写 Rx的低温校准参数和KH
int GET_10G_RX_ADC(int handle, short *ADC);											  		//读取 RSSI ADC 
int SET_10G_RXP_MON(int handle, enum LUT_MODE Mode);								  		//设置 RSSI模式 
int SET_RXPOW_TABLE(int handle, short *ADC_Array, double *PWR_Array, int CaliNumber);	  	//设置RSSI查找表  
int GET_10G_RxMon(int handle, float *RxMon, float Unit);									//计算 RxMon  

//SD
int SET_SD_Function_State(int handle, enum SD_FUN State);									  		//设置 RSSI模式  

float calculate_temperature_8077(INT16S data, float slope, float offset, float unit);
float calculate_voltage_8077(INT16U data, float slope, float offset, float unit);
float calculate_biascurrent_8077(INT16U data, float slope, float offset, float unit); 
float calculate_txpower_8077(INT16U data, float slope, float offset, float unit);
float calculate_rxpower_8077(INT16U data, float slope, float offset, float unit); 

BOOL UpdateTemperPoint(int channel, struct struTestData data);									  		//修改温度校准分界点  

//Cross 10G

BOOL SET_TXCross_DAC_10G(int handle,int data);
BOOL GET_TXCross_DAC_10G(int handle,int *data);

int XGPON_OLT_SET_LOS_DAC(int handle);                                               //设置LOS DAC  
BOOL tuningAPD(int channel, struct struTestData *data);

#endif

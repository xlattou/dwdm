/****************************************************************************
 *
 * File:                EVB5.h
 *
 * Author:              SUPERXON INC.(Roger Li)
 *
 * Description:         EVB5ͷ�ļ�
 *
 * Time:                2010-04-09
 *
 * version:				V1.2.0.2
 * 
�汾��ʷ 
V1.2.0.2 
1.
	INT8U   reserved56to73[18];		// F0H[56~73], 18byte 
��Ϊ
	INT16U  ADC15_SC1_MON;			// F0H[56~57], 2byte
	INT16U  ADC15_SC2_MON;			// F0H[58~59], 2byte
	float   Iapd_compensate;		// F0H[60~63], 4byte
	float   IT_compensate;			// F0H[64~67], 4byte
	float   IR_compensate;			// F0H[68~71], 4byte
	struct  sTxSD_Detect_DEF sTxSD_Detect;  //F0H[72], 1byte
	INT8U   reserved73[1];			// F0H[73], 1byte
2. ���Ӻ���	
EVB5_SET_TxSD_Detect
EVB5_READ_TxSD
EVB5_CLEAR_TxSD
EVB5_READ_FirmwareVersion

V1.2.0.1
����ģ���ѹ��ƫ���ú���

V1.2.0.0 
���Ӻ���
MEGA88_Programming_Enable
MEGA88_Programming_Disable
EVB5_GET_SPI_MEGA88

V1.1.0.6 
1. ��Ӷ���
#define EVB5_BEN_MODE_L1L2		7

V1.1.0.5 
��Ӷ���
#define EVB5_BEN_MODE_L2L3		6

V1.1.0.4
���ͺ�����ö�ٱ�����ǰ��e
SBERT_Pattern -> eSBERT_Pattern
SBERT_Rate    -> eSBERT_Rate

V1.1.0.3
Ӳ���汾������D�汾����Ӷ�·��鹦��

V1.1.0.2 
���²��ֱ�������λ��

V1.1.0.1
1.EVB5 C�汾�����ļ������LIV���ܺ���
2.���²���ADC DAC���壬������ο�Jack EVB5�ײ�������˵����EVB5C Tester, V2.0.0.0, 2009/Apr/02
****************************************************************************/
#ifndef _EVB5_H_
#define _EVB5_H_

#include "CH341A_DLL.h"
#include <windows.h>  

int	EVB5_Addr;
int SBERT_Addr;

#pragma pack (1) /* set maximum alignment to 1 */

//////////////////////////////////////////////////////////////
/*DAC*/
/////////////////////////////////////////////////////////////////

union uDAC										  
{	INT8U  DAC_LUT[12][128];
	INT16U DAC_ARR[4][192];
};

union uDAC myDAC;
//////////////////////////////////////////////////////////////
/*F0*/
/////////////////////////////////////////////////////////////////
struct KB_DEF
{
  float K;	     
  float B; 
};
   
struct sInput_DEF
{
INT8U TOSA_FAIL:1;	// ROSA, P1.4
INT8U ONU_RxSD:1;	// ONU,  P1.5
INT8U ONU_TxFAULT:1;// ONU,  P1.6
INT8U ONU_TxSD:1;	// ONU,  P0.4
INT8U OLT_RxLOS:1;	// OLT,  P1.2
INT8U OLT_TxFAULT:1;// ONU,  P0.5
//INT8U reserved:2;	// reserved  2009-07-14
INT8U ROSA_LOS:1;	// ROSA, X5
INT8U MOD_DEF0:1;	// SFP,  X7
};

struct sOutput_DEF
{
INT8U SHDN_VTR:1;	// TRx,  P0.7
INT8U SHDN_VOSA:1;	// ROSA, P0.6
INT8U VPD_MON_EN:1; // ROSA, P0.0
INT8U ONU_TxBEN:1;  // ONU,  P1.7, PWM�����pulse��level
INT8U OLT_Trig:1;	// OLT,  P2.0
INT8U OLT_RxRST:1;	// OLT,  P1.3
INT8U OLT_TxDIS:1;  // OLT,  P0.3
//INT8U SEL_IPDSR:1;// reserved  2009-07-14
INT8U ONU_TxDIS:1;
};

struct sFPGA_R7_SC_DEF
{
INT8U SC_GND:1;  // SC_GND�� P1.4
INT8U SC_VCC:1;  // SC_VCC,  P1.5
INT8U SC_VAPD:1; // SC_VAPD, P1.6
INT8U BELL:1;    // BELL,    P4.2
INT8U reserved:4;  // 
};

struct sTxSD_Detect_DEF
{
INT8U TxSD_ENABLE:1;  // Enable Test, 1: Enable, 0: Disable, Self Clear
INT8U TxSD_ERROR :1;  // Test Result, 1: Error,  0: No Error
INT8U reserved:6;  // 
};

struct strF0_EVB5
{                       
	INT8U	FirstByte;				// F0H[0], 1byte
//="E"=clear flash��
	INT8U	FirmwareVersionSuffix;	// F0H[1], 1byte, 
	INT8U	FirmwareVersion;		// F0H[2], 1byte 										
	INT8U	CPLDVersion;			// F0H[3], 1byte,
	INT16U	Core_Voltage;			// F0H[4~5], 2byte 							
	INT16S	Core_Temperature;		// F0H[6~7], 2byte
	struct	sOutput_DEF sOutput;	// F0H[8], 1byte, 
	struct	sInput_DEF  sInput;		// F0H[9], 1byte, 
	INT16U  ADC0_VPD_MON;			// F0H[10~11], 2byte��SEL_MON=0ʱ��Vapd_MON�������VapdM_MON                  
	INT16U  ADC1_IPD_MON;			// F0H[12~13], 2byte��APD������AD8651ת�����ĵ�ѹ                             
	INT16U  ADC2_Vx_MON;			// F0H[14~15], 2byte��SEL_MON=0ʱ��+3.3V_TRx_MON�������+5V_MON               
	INT16U  ADC3_ITRx_MON;			// F0H[16~17], 2byte��SEL_MON=0ʱ��IT_MON�������IR_MON                       
	INT16U  ADC4_LD_MON;			// F0H[18~19], 2byte��TOSA-LDǰ���ѹ                                         
	INT16U  DAC0_SET_VPD;			// F0H[20~21], 2byte��3482��3571��boost-up��ѹ����                            
	INT16U  DAC1_SET_VTR;			// F0H[22~23], 2byte��VTR��ѹ����                                             
	INT16U  ADC14_PD_MON;			// F0H[24~25], 2byte��PD�ܵ�����ѹ                                            
//	INT16U  DAC3_SEL_MON;			// modify 2009-07-14 
	INT16U	ADC15_MON;				// F0H[26~27], 2byte��δ�� 
	
	INT16U  ADC_VPD_MON;			// F0H[28~29], 2byte
	INT16U  ADC_IPD_MON;			// F0H[30~31], 2byte
	INT16U  ADC_IR_MON;				// F0H[32~33], 2byte
	INT16U  ADC_IT_MON;				// F0H[34~35], 2byte
	INT16U  ADC_VPDM_MON;			// F0H[36~37], 2byte
	INT16U  ADC_VTRx_MON;			// F0H[38~39], 2byte
	INT16U  ADC_VBUS_MON;			// F0H[40~41], 2byte
	INT16U  ADC_LD_MON;				// F0H[42~43], 2byte
	INT16U  ADC_PD_MON;				// F0H[44~45], 2byte
	
	INT16U  ADC0_VPDM_MON;			// F0H[46~47], 2byte
	INT16U  DAC_ADN8810_SET;	    // F0H[48~49], 2byte

//	INT8U   reserved50to73[24];		// modify 2009-07-14  
	INT16U  ADC15_SC_MON;			// F0H[50~51], 2byte
	INT16U  ADC_SC_MON;				// F0H[52~53], 2byte
	struct  sFPGA_R7_SC_DEF sFPGA_R7_SC;// F0H[54],1byte
	INT8U   Vapd_3571LT3482_sel;	// F0H[55], 1byte; =0, 3571, Vapd������DAC
	
//	INT8U   reserved56to73[18];		// F0H[56~73], 18byte
	INT16U  ADC15_SC1_MON;			// F0H[56~57], 2byte
	INT16U  ADC15_SC2_MON;			// F0H[58~59], 2byte
	float   Iapd_compensate;		// F0H[60~63], 4byte
	float   IT_compensate;			// F0H[64~67], 4byte
	float   IR_compensate;			// F0H[68~71], 4byte
	struct  sTxSD_Detect_DEF sTxSD_Detect;  //F0H[72], 1byte
	INT8U   reserved73[1];			// F0H[73], 1byte

	INT8U	BEN_mode;				// F0H[74],    1byte 
	INT8U   RSSI_mode;				// F0H[75],    1byte 

	INT16U  Tune_VPD_MON;			// F0H[76~77], 2byte
	INT8U   VapdAdj_loop;			// F0H[78],    1byte 
	INT8U   VapdAdj_en;				// F0H[79],    1byte
	float	Vapd_expect;			// F0H[80~83], 4byte 
	
	INT16U  Tune_IPD_MON;			// F0H[84~85], 2byte
	INT8U   IapdAdj_loop;			// F0H[86],    1byte 
	INT8U   IapdAdj_en;				// F0H[87],    1byte
	float	Iapd_expectMIN;			// F0H[88~91], 4byte 
	float	Iapd_expectMAX;			// F0H[92~95], 4byte 
	
	INT16U	Vapd20V_loop;			// F0H[96~97], 2byte 
	INT16U	Vapd20V_DAC;			// F0H[98~99], 2byte 
	INT8U   Vapd20V_en;				// F0H[100],   1byte
	
//	INT8U	Vbr_4V_en;				// modify 2009-07-14 
	INT8U	Vbr_3V4V_sel;			// F0H[101],   1byte, =0 Vbr-3V; =1 Vbr-4V;   	
	INT8U	Vbr_3V_en;				// F0H[102],   1byte
	INT8U	Vbr_3V_status;			// F0H[103],   1byte
	INT16U	ADC_Vbr_MON;			// F0H[104~105],2byte

	INT8U   Vapd_30V_en;			// F0H[106],   1byte
	INT8U   Vapd_30V_status;		// F0H[107],   1byte
	INT16U	ADC_V30V_MON;			// F0H[108~109],2byte

	INT16U	myPWMDAT0;				// F0H[110~111],2byte
	
	INT16U  Tune_VTR_MON;			// F0H[112~113], 2byte
	INT8U   VTRAdj_loop;			// 82H[114],    1byte 
	INT8U   VTRAdj_en;				// 82H[115],    1byte
	float	VTR_expect;				// 82H[116~119], 4byte 
	
	INT8U   delay1;					// F0H[120],    1byte
	INT8U   delay2;					// F0H[121],    1byte
	INT8U   delay3;					// F0H[122],    1byte
	INT32U  Password;  				// F0H[123~126]   										
	INT8U   LUTIndex;				// F0H[127] 

//FLASH
	INT8U	I2CSlaveAdd2;			// F0H[128], 1byte
	INT8U	I2CSlaveAdd0;			// F0H[129], 1byte
	INT8U	I2CSlaveAdd1;			// F0H[130], 1byte
	INT8U	CheckSumPNSN;			// F0H[131], 1byte
	INT8U   vendorPN[16];			// F0H[132~147], 16byte
	INT8U   vendorSN[16];			// F0H[148~163], 16byte
	INT8U	reserved164to171[8];	// F0H[164~171], 8byte           
	struct	KB_DEF RxVapd_MON_KB;	// F0H[172~179], 8byte           
	struct	KB_DEF VAPD_MON_KB;		// F0H[180~187], 8byte           
	INT8U	reserved188to227[40];	// F0H[188~227], 40byte           
	INT16U	RSSI_PW;				// F0H[228~229], 2byte 
	INT8U	RSSI_polarity;			// F0H[230], 1byte 
	INT8U	Reserved231;			// F0H[231], 1byte 
	float	Vapd_expectMIN;			// F0H[232~235], 4byte 
	float	Vapd_expectMAX;			// F0H[236~239], 4byte 
	float	Vapd_DeltaMAX;			// F0H[240~243], 4byte,
	INT16U	Vapd20V_loop_ini;		// F0H[244~245], 2byte 
	INT16U	Vapd20V_DAC_ini;		// F0H[246~247], 2byte 
	INT8U   Vapd20V_en_ini;			// F0H[248],   1byte
	INT8U	BEN_polarity;			// F0H[249], 1byte 
	INT16U	BEN_PW;					// F0H[250~251], 2byte 
	INT8U	Reserved252to255[4];	// F0H[252~255], 4byte 
};

struct str80 
{                       
	INT8U	FirstByte;						// 8EH[0], 1byte //CMU_STATUS��
	INT8U	RX_STATUS;						// 8EH[1], 1byte
	INT8U   TX_STATUS;						// 8EH[2], 1byte
	INT8U   RESERVED1[5];					// 8EH[3~7], 5byte
	INT8U   FIFO_STATUS;					// 8EH[8], 1byte 
	INT8U   RESERVED2[7];					// 8EH[9~15], 6byte
	INT8U   RX_SDU_CTRL;					// 8EH[16], 1byte
	INT8U   RX_ASD_CTRL;					// 8EH[17], 1byte
	INT8U	TX_SDU_CTRL;					// 8EH[18], 1byte
	INT8U	TX_ASD_CTRL;					// 8EH[19], 1byte
	INT8U   RESERVED3[3];					// 8EH[20~22], 3byte
	INT8U	MODE_CTRL_0;					// 8EH[23], 1byte
	INT8U   RESERVED4[5];					// 8EH[24~28], 5byte
	INT8U	RX_FIFO_CTRL;					// 8EH[29], 1byte
	INT8U	TX_FIFO_CTRL;					// 8EH[30], 1byte
	INT8U	RX_FT_MODE;						// 8EH[31], 1byte
	INT8U	TX_FT_MODE;						// 8EH[32], 1byte
	INT8U	RX_DATA_CTRL; 					// 8EH[33], 1byte
	INT8U	TX_DATA_CTRL; 					// 8EH[34], 1byte
	INT8U	CMU_RATE_CTRL;					// 8EH[35], 1byte
	INT8U	RX_RATE_CTRL; 					// 8EH[36], 1byte
	INT8U	TX_RATE_CTRL; 					// 8EH[37], 1byte
	INT8U	REFCLK_CTRL;					// 8EH[38], 1byte
	INT8U   RESERVED5;						// 8EH[39], 1byte
	INT8U	RXIN_CTRL;						// 8EH[40], 1byte
	INT8U	RX_OUT_CTRL; 					// 8EH[41], 1byte
	INT8U	TXIN_CTRL;						// 8EH[42], 1byte
	INT8U	TX_OUT_CTRL;					// 8EH[43], 1byte
	INT8U	RXCLK_CTRL;						// 8EH[44], 1byte
	INT8U   RESERVED6[13];					// 8EH[45~57], 13byte
	INT8U	PATCHK_UD7;						// 8EH[58], 1byte
	INT8U	PATCHK_UD6;						// 8EH[59], 1byte
	INT8U	PATCHK_UD5;						// 8EH[60], 1byte
	INT8U	PATCHK_UD4;						// 8EH[61], 1byte
	INT8U	PATCHK_UD3;						// 8EH[62], 1byte
	INT8U	PATCHK_UD2;						// 8EH[63], 1byte
	INT8U	PATCHK_UD1;						// 8EH[64], 1byte
	INT8U	PATCHK_UD0;						// 8EH[65], 1byte
	INT8U	PATCHK_TIMER_SET4;				// 8EH[66], 1byte
	INT8U	PATCHK_TIMER_SET3;				// 8EH[67], 1byte
	INT8U	PATCHK_TIMER_SET2;				// 8EH[68], 1byte
	INT8U	PATCHK_TIMER_SET1;				// 8EH[69], 1byte
	INT8U	PATCHK_TIMER_SET0;				// 8EH[70], 1byte
	INT8U	PATCHK_TIMER_GET4;				// 8EH[71], 1byte
	INT8U	PATCHK_TIMER_GET3;				// 8EH[72], 1byte
	INT8U	PATCHK_TIMER_GET2;				// 8EH[73], 1byte
	INT8U	PATCHK_TIMER_GET1;				// 8EH[74], 1byte
	INT8U	PATCHK_TIMER_GET0;				// 8EH[75], 1byte
	INT8U   RESERVED7[2];					// 8EH[76~77], 1byte
	INT8U	ERRCNT3;						// 8EH[78], 1byte
	INT8U	ERRCNT2;						// 8EH[79], 1byte
	INT8U	ERRCNT1; 						// 8EH[80], 1byte
	INT8U	ERRCNT0;						// 8EH[81], 1byte
	INT8U	PATCHK_CTRL;					// 8EH[82], 1byte
	INT8U   RESERVED8;						// 8EH[83], 1byte
	INT8U	PATCHK_STATUS;					// 8EH[84], 1byte 
	INT8U   RESERVED9[5];					// 8EH[85~89], 5byte
	INT8U	PATGEN_UD7;						// 8EH[90], 1byte
	INT8U	PATGEN_UD6;						// 8EH[91], 1byte
	INT8U	PATGEN_UD5;						// 8EH[92], 1byte
	INT8U	PATGEN_UD4;						// 8EH[93], 1byte
	INT8U	PATGEN_UD3;						// 8EH[94], 1byte
	INT8U	PATGEN_UD2;						// 8EH[95], 1byte
	INT8U	PATGEN_UD1;						// 8EH[96], 1byte
	INT8U	PATGEN_UD0;						// 8EH[97], 1byte
	INT8U	PATGEN_CTRL;					// 8EH[98], 1byte
	INT8U   RESERVED10[18];					// 8EH[99~116], 18byte
	INT8U	CHIP_ID;						// 8EH[117], 1byte
	INT8U   RESERVED11[9];					// 8EH[118~126], 9byte
	INT8U	RESET;							// 8EH[127], 1byte 
	INT8U	Reserved128to255[128];			// A6H[128~255], 128byte 
};

#pragma pack () /* set maximum alignment to 1 */  

union uF0_EVB5										  
{ struct strF0_EVB5  sStrF0;
  INT8U  pStrF0[256];
};

//union uF0_EVB5 EVB5;

union u80
{ struct str80  sStr80;
  INT8U  pStr80[256];
};
union u80 sBERT; 

//BERT pattern
enum enum_SBERT_Pattern 
{   
	SBERT_P_PRBS7,
	SBERT_P_PRBS23
} eSBERT_Pattern; 

//BERT rate 
enum enum_SBERT_Application 
{  
	SBERT_R_R1244Mbps,
	SBERT_R_R2488Mbps
} eSBERT_Rate;

//GBE ����Ҫ����BURST״̬
struct  struEVB5	  
{
int  BURST_ON;
int  BURST_OFF;
} my_struEVB5;

#define EVB5_BURST_H		1	//����burst�ߵ�ƽ��ֵ
#define EVB5_BURST_L		0	//����burst�͵�ƽ��ֵ

#define EVB5_BEN_MODE_LEVEL		0	//����BEN�̶���ƽģʽ=
#define EVB5_BEN_MODE_PULSE		1	//����BEN������ģʽ
#define EVB5_BEN_MODE_CLOCK		2	//����BENʱ��ģʽ
#define EVB5_BEN_MODE_N1N2N3	3   //ONU_TxBEN with OLT_Trig, N1N2N3
#define EVB5_BEN_MODE_N1N3		4   //ONU_TxBEN with OLT_Trig, N1N3
#define EVB5_BEN_MODE_N2		5   //ONU_TxBEN with OLT_Trig, N2
#define EVB5_BEN_MODE_L2L3		6   //ONU_TxBEN with OLT_Trig, L2L3
#define EVB5_BEN_MODE_L1L2		7   //ONU_TxBEN with OLT_Trig, L1L2    

#define TINY13_MODE_MANUAL	1	//����TINY13�˹�ģʽ
#define TINY13_MODE_AUTO	0	//����TINY13�Զ�ģʽ  

//for SBERT
extern int EVB5_SBERT_Init (int EVB5Handle, int sBERT_Rate, int sBERT_PRBS); 
extern int EVB5_SBERT_Start (int EVB5Handle, int sBERT_Rate, double sBERT_Time); 
extern int EVB5_SBERT_Read (int EVB5Handle, int sBERT_Rate, int sBERT_PRBS, int *sBERT_Status_Code, unsigned char *sBERT_Status_String, double *sBERT_ERR_Number, double *sBERT_ERR_Ratio, double *sBERT_ElapsedTime);
extern int EVB5_SBERT_End (int EVB5Handle);
extern int EVB5_SBERT_Close (int inst_SBERT);
extern int EVB5_SBERT_Open (int inst_SBERT);   

//20090116 add by roger
//-----------------�ڲ�����--------------------// 
int TINY13_Programming_Disable(int EVB5Handle);
int TINY13_Programming_Enable(int EVB5Handle);
int TINY13_Read_EEPROM(int EVB5Handle, unsigned char *EEPROM_Array);
int TINY13_Write_EEPROM(int EVB5Handle, unsigned char *EEPROM_Array);
int TINY13_Write_EEPROM_BYTE(int EVB5Handle, unsigned char NUM_EEPROM_ADD, unsigned char NUM_EEPROM_DAT);	//���ֽ�д
int TINY13_Chip_Erase(int EVB5Handle);
int TINY13_OnePage_WRITE_FLASH(int EVB5Handle, int CurrentPageNumner, unsigned char *Data_Array);
int TINY13_OnePage_READ_FLASH(int EVB5Handle, int CurrentPageNumner, unsigned char *Data_Array);
int TINY13_Check_Signature_Byte(int EVB5Handle); //���������MEGA88�ı�־�֣����ش������   
int TINY13_Write_Fuse_bits(int EVB5Handle, unsigned char FUSE, unsigned char FUSE_HIGH); //����������˿λ 
int TINY13_Read_Fuse_bits(int EVB5Handle, unsigned char *FUSE, unsigned char *FUSE_HIGH, unsigned char *FUSE_Extended); //�ض�������˿λ  
int MEGA88_Programming_Enable (int inst_EVB5);
int MEGA88_Programming_Disable(int inst_EVB5);
//-----------------�ڲ�����--------------------//

//-----------------�ⲿ����--------------------//
//EVB5��TINY13�Ĳ���
extern int EVB5_WRITE_TINY13_2BYTEs(int EVB5Handle, BYTE Addr1, BYTE Data1, BYTE Addr2, BYTE Data2);//Tiny13 EEPROM ˫�ֽ�д 
extern int EVB5_WRITE_TINY13_BYTE(int EVB5Handle, BYTE Addr, BYTE Data);	//Tiny13 EEPROM ���ֽ�д 
extern int EVB5_READ_TINY13_PAGE(int EVB5Handle, BYTE *rom_value);			//Tiny13 EEPROM ҳ��
extern int EVB5_READ_TINY13_Temper(int EVB5Handle, double *Temper);			//Tiny13 �¶ȶ�ȡ
extern int EVB5_READ_TINY13_Ibias(int EVB5Handle, double *Ibias);			//Tiny13 ƫ�õ�����ȡ 
extern int EVB5_WRITE_TINY13_Firmware(int EVB5Handle, BYTE *FirmwareData);	//Tiny13 ����Firmware
extern int EVB5_READ_TINY13_Firmware(int EVB5Handle, BYTE *FirmwareData);   //Tiny13 ��ȡFirmware
extern int EVB5_WRITE_TINY13_FuseBit(int EVB5Handle);						//Tiny13 ������˿λ
extern int EVB5_READ_TINY13_FuseBit(int EVB5Handle);						//Tiny13 ��ȡ��˿λ 
extern int EVB5_WRITE_TINY13_PAGE(int EVB5Handle, BYTE *rom_value);			//Tiny13 EEPROM ҳд 
	
//EVB5���ƴ���
//20090107 ���º��� ���SN����������ֵ��Ϊint
extern int EVB5_Init (int *EVB5Handle, char *SN);
extern int EVB5_Init_Ex (int *EVB5Handle); 
extern int EVB5_GET_SN (int EVB5Handle, char SN[16]);
extern int EVB5_SET_SHDN_VTR(int EVB5Handle, unsigned char SHUTValue);	//1:ON 0:OFF 
extern int EVB5_SET_BEN_Mode(int EVB5Handle, unsigned char BEN_Mode);	//0:level 1:pulse 2:clock  
extern int EVB5_SET_BEN_Level(int EVB5Handle, unsigned char BEN_Level);	//0:low 1:high 
extern int EVB5_SET_BEN_PulsePolarity(int EVB5Handle, unsigned char BEN_PulsePolarity);	//0:Normal 1:Invert 
extern int EVB5_SET_BEN_DELAY(int EVB5Handle, INT8U N1_Delay, INT8U N2_Delay, INT8U N3_Delay);	//����BEN delay    
extern int EVB5_READ_TxI (int EVB5Handle, double *TxI); 
extern int EVB5_READ_RxI (int EVB5Handle, double *RxI); 
extern int EVB5_READ_ONU_RxSD (int EVB5Handle, BYTE *SD); 
extern int EVB5_READ_ONU_TxSD (int EVB5Handle, BYTE *SD);
extern int EVB5_READ_ONU_TxFAULT (int EVB5Handle, BYTE *TxFAULT); 
extern int EVB5_READ_Temper(int EVB5Handle, double *Temper);
extern int EVB5_READ_OLT_TxFAULT (int EVB5Handle, BYTE *TxFAULT);
extern int EVB5_READ_OLT_RxLOS (int EVB5Handle, BYTE *RxLOS); 
extern int EVB5_SET_OLT_TxDIS (int EVB5Handle, BYTE TxDIS); 
extern int EVB5_SET_SHDN_VOSA (int EVB5Handle, unsigned char SHUTValue);
extern int EVB5_SET_ADN8810 (int EVB5Handle, float If);							//If�������mA
extern int EVB5_GET_ADN8810 (int EVB5Handle, float *If, float *Vf, float *Im); 
extern int EVB5_SET_VAPD (int EVB5Handle, const double Vapd);
extern int EVB5_SET_MODE (int EVB5Handle, const char Mode);
extern int EVB5_READ_IPD_MON (int EVB5Handle, double *Iapd);					//OSA��Iapd���� ��λuA
extern int EVB5_READ_VPD_MON (int EVB5Handle, double *Vapd);					//OSA��Vapd��ѹ ��λV
extern int EVB5_SET_Vbr_3V(int EVB5Handle, float *Vbr);							//Vbr-3V���Ժ�����ͨ��EVB5��MCU��� 
extern int EVB5_SET_VAPD_30V(int EVB5Handle);									//����30V��ѹֵ
extern int EVB5_SET_VPD_MOD_EN (int EVB5Handle, const char Mode); 				//����OSA��Vapd��ѹ�ɼ�ģʽ��������ɼ�ģʽ
extern int EVB5_SET_IAPD_EXPECT (int EVB5Handle, const float Iapd_expectMIN, const float Iapd_expectMAX);
extern int EVB5_SET_IAPD(int EVB5Handle, float *Vapd, float *Iapd);				//���������Iapd���õ�Vapd
extern int EVB5_READ_VAPDM_MON (int EVB5Handle, double *Vapd);					//TR��Vapd��ѹ ��λV 
extern int EVB5_SET_CLOCK (int EVB5Handle, double Clock);						//���÷���ʱ��Ƶ�ʣ� ��λMHz, �������������ԭ��ʵ�ʵķ���Ƶ����һ����� 
extern int EVB5_SET_BEN_PW(int EVB5Handle, int BEN_PW);							//���õ������ȣ���λuS t = 1.6 + EVB.sStrF0.RSSI_PW * 0.26; 
extern int EVB5_GET_R7_SC (int EVB5Handle, int *ErrorCode, char *ErrorStr);		//��·���Ժ��� 
extern int EVB5_GET_SPI_MEGA88 (int inst_EVB5);									//��ȡSPI�����豸 ����0����ʾSPI��û�������豸mega88�� -1��ʾSPI���������������豸mega88
extern int EVB5_SET_TRIG_LEVEL (int EVB5Handle, int tigger_level);				//RSSI trigger ��ƽ����
extern int EVB5_SET_ONU_TxDIS (int inst_EVB5, BYTE TxDIS); 
extern int EVB5_GET_ONU_TxDIS (int inst_EVB5, BYTE *TxDIS);
extern int EVB5_SET_VTR (int inst_EVB5, float VTR);								//����ģ��Vcc��ѹ
extern int EVB5_SET_TxSD_Detect (int inst_EVB5);								//����TxSD�о�ʹ��
extern int EVB5_READ_TxSD (int inst_EVB5, BYTE *TxSD_Error);					//��ȡTxSD״̬
extern int EVB5_CLEAR_TxSD (int inst_EVB5);										//���TxSD״̬
extern int EVB5_READ_FirmwareVersion (int inst_EVB5, int *version);				//��ȡFirmware�汾����ʽFirmwareVersion*10+FirmwareVersionSuffix
extern int EVB5_Get_MOD_DEF0(int inst_EVB5, int *val); 
//-----------------�ⲿ����--------------------// 
#endif

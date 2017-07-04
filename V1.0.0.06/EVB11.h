#ifndef _EVB11_H_
#define _EVB11_H_

#include "CH341A_DLL.h"
#include <windows.h> 

#pragma pack (1) /* set maximum alignment to 1 */

struct sDin1_DEF
{
INT8U ONU1_ABS:1; 
INT8U ONU1_NR:1;	
INT8U ONU1_RxLOS:1;
INT8U ONU1_INT:1;	
INT8U ONU2_INT:1;	
INT8U ONU2_RxLOS:1;	
INT8U ONU2_ABS:1;	
INT8U ONU2_NR:1;	
};
struct sDin2_DEF
{
INT8U BERT_LOL:1;	// 
INT8U reserved:2;
INT8U BERT_LOS:1;	
INT8U ONU1_TxSD:1;	
INT8U ONU2_TxSD:1;	
INT8U OLT_RxLOS:1;	
INT8U OLT1G_RxLOS:1;	
};

struct sInput_DEF10G
{
INT8U Din1:1;	    // P1.2
INT8U Din2:1;	    // P0.6
INT8U reserved:1;
INT8U ONU1_PDRST:1;	// ONU， P4.2
INT8U SEL:3;		// SEL[0~2]:P[1.4~1.6]
INT8U SW_CASE:1;	// DAC3
};

struct sOutput_DEF10G
{
INT8U OLT_TxDIS:1;	// OLT, P0.3
INT8U OLT_RxRST:1;  // OLT, P1.3
INT8U OLT1G_TxDIS:1;// OLT, P0.4
INT8U OLT1G_RxRST:1;// OLT, P0.5
INT8U OLT_RxTRIG:1;	// OLT, P0.0
INT8U ONU1_TxDIS:1;	// ONU, P0.7
INT8U ONU1_DESEL:1; // ONU, P2.0
INT8U ONU1_TxBEN:1;	// ONU, P1.7
};


struct sSC_DEF
{
INT8U OLT_SC_GND:1;  // SC_GND
INT8U OLT_SC_VCC:1;  // SC_VCC
INT8U OLT_SC_VAPD:1; // SC_VAPD
INT8U OLT_BELL:1;    // BELL
INT8U ONU_SC_GND:1;  // SC_GND
INT8U ONU_SC_VCC:1;  // SC_VCC
INT8U ONU_SC_VAPD:1; // SC_VAPD
INT8U ONU_BELL:1;    // BELL
};
     
struct strF0_EVB11
{                       
		INT8U	FirstByte;				// F0H[0], 1byte
		//="E"=clear flash；

		INT8U	FirmwareVersionSuffix;	// F0H[1], 1byte, 

		INT8U	FirmwareVersion;		// F0H[2], 1byte 										
		INT8U	CPLDVersion;			// F0H[3], 1byte,
		INT16U	Core_Voltage;			// F0H[4~5], 2byte 							
		INT16S	Core_Temperature;		// F0H[6~7], 2byte
		struct	sOutput_DEF10G sOutput;	// F0H[8], 1byte 
		struct	sInput_DEF10G  sInput;		// F0H[9], 1byte 

		INT16U  ADC0_MON;				// F0H[10~11], 2byte，
		INT16U  ADC1_MON;				// F0H[12~13], 2byte，
		INT16U  ADC2_Vx_MON;			// F0H[14~15], 2byte，
		INT16U  ADC3_Ix_MON;			// F0H[16~17], 2byte，
		INT16U  ADC4_MON;				// F0H[18~19], 2byte，
		INT16U  DAC0_SET_OLT5V;			// F0H[20~21], 2byte，
		INT16U  DAC1_SET_OLT3V;			// F0H[22~23], 2byte，
		INT16U  DAC2_SET_ONU3V;			// F0H[24~25], 2byte，
		INT16U  DAC3_SW_CASE;			// F0H[26~27], 2byte，

		INT16U	myPWMDAT0;				// F0H[28~29], 2byte //PWM switching freq
		INT16U  myPWMDAT1;				// F0H[30~31], 2byte //Dead time
		INT16U  myPWMCH0; 				// F0H[32~33], 2byte //Duty Cycle
		INT16U  myPWMCH2; 				// F0H[34~35], 2byte //Duty Cycle

		INT8U   SPI_ADD;                // F2H[36]
		INT8U   SPI_DAT;                // F2H[37]
		INT8U   SPI_WE;                 // F2H[38]//SPI写使能，自清零 
		INT8U   SPI_RE;                 // F2H[39]//SPI读使能，自清零 

		INT8U	I2Ccommand;				// F2H[40], 1byte
		// ="R" if start reading; ="W" if start writing; 
		// return the error code after operation completed:
		// error=0 if success; =1 if error occurs; 
		INT8U	I2Cslave_address;		// F2H[41], 1byte
		INT8U	I2Creg_start_add;		// F2H[42], 1byte
		INT8U	I2Creg_length;			// F2H[43], 1byte
		INT8U	I2Creg_dat_arr[8];		// F2H[44..51], 8byte


		INT16U  ADC_12V_MON;			// F0H[52~53], 2byte, 1LSB = 0.2[mV]
		INT16U  ADC_5VOLT_MON;			// F0H[54~55], 2byte
		INT16U  ADC_3VOLT_MON;			// F0H[56~57], 2byte
		INT16U  ADC_3VONU_MON;			// F0H[58~59], 2byte
		INT16U  ADC_3VMCU_MON;			// F0H[60~61], 2byte
		INT16U  ADC_SCOLT_MON;			// F0H[62~63], 2byte
		INT16U  ADC_2VCDR_MON;			// F0H[64~65], 2byte
		INT16U  ADC_SCONU_MON;			// F0H[66~67], 2byte
		INT16U  ADC_IOLT5V_MON;			// F0H[68~69], 2byte
		INT16U  ADC_ITOLT_MON;			// F0H[70~71], 2byte
		INT16U  ADC_IROLT_MON;			// F0H[72~73], 2byte
		INT16U  ADC_ITONU1_MON;			// F0H[74~75], 2byte
		INT16U  ADC_IRONU1_MON;			// F0H[76~77], 2byte

		struct	sDin1_DEF   sDin1;		// F0H[78], 1byte 
		struct	sDin2_DEF   sDin2;		// F0H[79], 1byte 

		INT16U  Tune_OLT5V_MON;			// F0H[80~81], 2byte
 		INT8U   OLT5VAdj_loop;			// 82H[82],    1byte 
		INT8U   OLT5VAdj_en;			// 82H[83],    1byte
		float	OLT5V_expect;			// 82H[84~87], 4byte 

		INT16U  Tune_OLT3V_MON;			// F0H[88~89], 2byte
 		INT8U   OLT3VAdj_loop;			// 82H[90],    1byte 
		INT8U   OLT3VAdj_en;			// 82H[91],    1byte
		float	OLT3V_expect;			// 82H[92~95], 4byte 

		INT16U  Tune_ONU3V_MON;			// F0H[96~97], 2byte
 		INT8U   ONU3VAdj_loop;			// 82H[98],    1byte 
		INT8U   ONU3VAdj_en;			// 82H[99],    1byte
		float	ONU3V_expect;			// 82H[100~103], 4byte 
		
		struct  sSC_DEF     sSC;		// F0H[104],    1byte
  		INT8U   reserved105;				// F0H[105],    1byte
		INT16U  ADC_SCOLT_MON_O;		// F0H[106~107], 2byte
		INT16U  ADC_SCOLT_MON_C;		// F0H[108~109], 2byte
		INT16U  ADC_SCONU_MON_O;		// F0H[110~111], 2byte
		INT16U  ADC_SCONU_MON_C;		// F0H[112~113], 2byte

  		INT8U   reserved114to122[9];	// F0H[114~122], 9byte

		INT32U  Password;  				// F0H[123~126]   										
		INT8U   LUTIndex;				// F0H[127] 

//FLASH
		INT8U	I2CSlaveAdd2;			// F0H[128], 1byte
		INT8U	I2CSlaveAdd0;			// F0H[129], 1byte
		INT8U	I2CSlaveAdd1;			// F0H[130], 1byte
		INT8U	CheckSumPNSN;			// F0H[131], 1byte

		INT8U   vendorPN[16];			// F0H[132~147], 16byte
		INT8U   vendorSN[16];			// F0H[148~163], 16byte

		INT8U   RSSI_mode;				// F0H[164],    1byte 
		INT8U	RSSI_polarity;			// F0H[165], 1byte 
		INT16U	RSSI_PW;				// F0H[166~167], 2byte 
		INT8U	BEN_mode;				// F0H[168],    1byte 
		INT8U	BEN_polarity;			// F0H[169], 1byte 
		INT16U	BEN_PW;					// F0H[170~171], 2byte 
		INT8U   delay1;					// F0H[172],    1byte
		INT8U   delay2;					// F0H[173],    1byte
		INT8U   delay3;					// F0H[174],    1byte

		INT8U	Reserved175to255[81];	// F0H[175~255], 81byte 
};

#pragma pack () /* set maximum alignment to 1 */  

union uF0_EVB11										  
{ 
	struct strF0_EVB11  sStrF0;
    INT8U  pStrF0[256];
};
   
union uF0_EVB11 EVB11;
int EVB11_Addr;
unsigned char USBVersion;


extern int EVB11_Init (int *EVB11Handle, char *SN);
extern int EVB11_GET_SN (int panel, int control);
extern int EVB11_SET_OLT_TxDIS (int EVB11Handle, BYTE TxDIS);
extern int EVB11_SET_OLT1G_TxDIS (int EVB11Handle, BYTE TxDIS);
extern int EVB11_SET_TRIG_LEVEL (int EVB11Handle, int tigger_level);
extern int EVB11_READ_OLT1G_RxLOS (int EVB11Handle, BYTE *RxLOS);
extern int EVB11_READ_OLT_RxLOS (int EVB11Handle, BYTE *RxLOS);
extern int EVB11_SET_OLT_VTR(int EVB11Handle, unsigned char SHUTValue);

#endif 
